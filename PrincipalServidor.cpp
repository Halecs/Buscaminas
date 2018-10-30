#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <signal.h>
#include <unistd.h>
#include <ctime>
#include <arpa/inet.h>
#include <vector>
#include <regex>
#include <iostream>
#include <fstream>
#include <pthread.h>
#include "Jugador.hpp"
#include "Partida.hpp"

using namespace std;

bool ExisteJugador(std::vector<Jugador> v, Jugador j);
void imprimirJugadores(std::vector<Jugador> v);
void salirCliente(int socket, fd_set * readfds, std::vector<Jugador> v);
int localizaJugador(int socket, std::vector<Jugador> v);
bool ExisteJugador(std::vector<Jugador> v, string j);
int buscarJugadorPartida(Jugador j, std::vector<Partida> partidas);
int localizaJugador(string nombre, std::vector<Jugador> v);
void escribirfichero(string nombrefich, Jugador j);
void eliminar_partida(std::vector<Partida> &p,int partida);


static pthread_mutex_t sem = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char const *argv[])
{
	int sd, new_sd; //Sockets 
	struct sockaddr_in sockname,from;
	fd_set readfds, auxfds;
    socklen_t from_len;
	int salida,on,ret, busca;
    string nombrefich="jugadores.txt";
    bool entra=false;
	std::vector<Jugador> Jugadores;
	std::vector<Partida> Partidas;
    //Necesitamos un vector de cola de emparejamiento
    std::vector<Jugador> Cola;

	string mensajeError = "-Err. No se puede realizar la accion\n";
    string mensajeDesconexionPartida = "Un jugador se ha desconectado de la partida";
    string mensajeDesconexionPartida2 = "Te has desconectado de la partida";
	/* Abrimos socket*/
	sd = socket (AF_INET, SOCK_STREAM, 0);
	if (sd == -1)
	{
		perror("No se puede abrir el socket cliente\n");
    		exit (1);	
	}

	/* Habilitamos la reutilización de sockets*/
	on=1;
    ret = setsockopt( sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    /* Rellenamos la struct sockname */
    sockname.sin_family = AF_INET;
	sockname.sin_port = htons(2000);
	sockname.sin_addr.s_addr =  INADDR_ANY;

	if (bind (sd, (struct sockaddr *) &sockname, sizeof (sockname)) == -1)
	{
		perror("Error en la operación bind");
		exit(1);
	}

		from_len = sizeof (from);

	/* Hacemos listen en el puerto que hemos bindeado */
	if(listen(sd,1) == -1){
		perror("Error en la operación de listen");
		exit(1);
	}

	/* Iniciamos el conjunto fd_set*/
	FD_ZERO(&readfds);
    FD_ZERO(&auxfds);
    FD_SET(sd,&readfds);
    FD_SET(0,&readfds);

    //Aqui vamos a leer desde el fichero

    string s,d,f;
    ifstream fich(nombrefich.c_str());
    Jugador aux;
    if(fich.is_open())
    {
        getline(fich,s,' ');
        getline(fich,d,'\n');
        while(!fich.eof())
        {
            if(s.length() != 0 and s.length() != 0)
            {
                aux.setNombre(s);
                aux.setSocket(-1);
                aux.setPassword(d);
                aux.setEstado(5);
                Jugadores.push_back(aux);
            }
            getline(fich,s,' ');
            getline(fich,d,'\n');
        }
        fich.close();
    }
    else
        std::cout<<"No hay registro de jugadores\n";

    while(1)
    {
    	auxfds = readfds;
        salida = select(FD_SETSIZE,&auxfds,NULL,NULL,NULL);

        if(salida > 0)
        {
            for(int i=0; i<FD_SETSIZE; i++){
                
        	/* Buscamos el socket que se ha establecido conexion*/
            if(FD_ISSET(i, &auxfds))
            {

            	if(i == sd)
            	{
            		if((new_sd = accept(sd, (struct sockaddr *)&from, &from_len)) == -1){
                        perror("Error aceptando peticiones\n");
                    }
                    else
                    {
                    	/* Es un jugador nuevo, creamos un jugador */
                    	Jugador aux;
                    	aux.setSocket(new_sd);
                    	aux.setEstado(0);
                    	string mensaje = "+Ok. Conectado al servidor\n";
                    	send(aux.getSocket(),mensaje.c_str(),mensaje.size(),0);
                        string ayuda ="Para ver todos los comandos escriba \"HELP\"\n";
                        send(aux.getSocket(),ayuda.c_str(),ayuda.size(),0);
                        Jugadores.push_back(aux);
 			            FD_SET(new_sd,&readfds);
                    }

            	}
            	/* Si ha introducido el servidor algo por teclado */
            	else if(i==0)
            	{
            		/*Cogemos la cadena y comparamos con distintas opciones (SALIR, VER LISTA DE JUGADORES...) */
            		/* salir */
            		string entrada;
                    getline(std::cin, entrada,'\n');
            		if("SALIR" == entrada)
            		{
            			for (int i = 0; i < Jugadores.size(); ++i)
            			{
            				send(Jugadores[i].getSocket(), "+Ok. Desconexion servidor\n", sizeof("+Ok. Desconexion servidor\n"),0);
                            close(Jugadores[i].getSocket());
                            FD_CLR(Jugadores[i].getSocket(),&readfds);
            			}
                        
            			close(sd);
            			exit(-1);
            		}
                    else if("Lista de jugadores" == entrada)
                    {
                        imprimirJugadores(Jugadores);
                    }
            	}
            	else
            	{
            		/*El usuario ha hecho algo*/
                    char buffer[250];
                    bzero(buffer,sizeof(buffer));
                    int recibido = recv(i,buffer,sizeof(buffer),0);

                    if(recibido > 0)
                    {
                        std::cout<<buffer;
                        entra = false;
                        /*Queda ver como se desconecta si esta en partida*/
                        if(strncmp("Salir", buffer,5) == 0)
                        {
                            entra=true;
                            busca = localizaJugador(i,Jugadores);
                            if(Jugadores[busca].getEstado() != 3 && Jugadores[busca].getEstado() != 4)
                            {
                                salirCliente(i,&readfds,Jugadores);
                
                                Jugadores[busca].setSocket(-1); 
                                Jugadores[busca].setEstado(5); 
                            }
                            else // Si estan en partida o buscando
                            {
                                //Registrado buscando partida
                                if(Jugadores[busca].getEstado() == 3)
                                {   
                                    for (int l = 0; l < Cola.size(); ++l)
                                    {
                                        if(Cola[l].getSocket() == Jugadores[busca].getSocket())
                                            Cola.erase(Cola.begin() + l); //??? Es sin el -1
                                        salirCliente(i,&readfds,Jugadores);
                                        Jugadores[busca].setEstado(5); 
                                    }
                                }
                                //En partida
                                if(Jugadores[busca].getEstado() == 4)
                                {
                                    pthread_mutex_lock (&sem);
                                    int partida = buscarJugadorPartida(Jugadores[busca], Partidas); //Encontramos la partida
                                    int elotro = Partidas[partida].encontrarJugadorOponente(Jugadores[busca].getSocket()); //Buscamos el socket del otro jugador de la partida
                                    if(elotro == 0) // El jugador 1 es el que se va a salir
                                    {                            pthread_mutex_lock (&sem);
                                        send(Jugadores[busca].getSocket(),"+Ok. Te has desconectado de la partida\n",sizeof("+Ok. Te has desconectado de la partida\n"),0);
                                        send(Partidas[partida].socketJugador1(),"-Err. Un jugador se ha desconectado de la partida, se ha cancelado la partida, para volver a buscar 'INICIAR PARTIDA'\n",sizeof("-Err. Un jugador se ha desconectado de la partida, se ha cancelado la partida, para volver a buscar 'INICIAR PARTIDA'\n"),0);
                                        salirCliente(i,&readfds,Jugadores);
                                        Jugadores[busca].setEstado(5); 
                                        Jugadores[localizaJugador(Partidas[partida].Jugador1().getSocket(), Jugadores)].setEstado(REGISTRADO_SIN_PARTIDA);
                            
                                    }
                                    else
                                    {
                                        send(Jugadores[busca].getSocket(),"+Ok. Te has desconectado de la partida\n",sizeof("+Ok. Te has desconectado de la partida\n"),0);
                                        send(Partidas[partida].socketJugador2(),"-Err. Un jugador se ha desconectado de la partida\n",sizeof("-Err. Un jugador se ha desconectado de la partida\n"),0);
                                        salirCliente(i,&readfds,Jugadores);
                                        Jugadores[busca].setEstado(5); 
                                        Jugadores[localizaJugador(Partidas[partida].Jugador2().getSocket(), Jugadores)].setEstado(REGISTRADO_SIN_PARTIDA);
                                    }
                                    pthread_mutex_unlock (&sem);
                                }
                            }
                        }



                        if(strncmp("USUARIO ",buffer,7) == 0)
                        {
                            entra=true;
                            string user=buffer;
			                string usuario=user.substr(8,string::npos);
                            busca = localizaJugador(i,Jugadores);
                            int pos = localizaJugador(usuario,Jugadores);
                            //Si se encuentra  al jugador 
                            if(pos != -1)
                            {
                                //Si la cadena del nombre esta vacia
                                if(Jugadores[busca].getEstado() == 0 or Jugadores[busca].getEstado() == 5)
                                {
			                        if(Jugadores[pos].getEstado()==DESCONECTADO )    //Se comprueba que esta OFF el usuario que hemos encontrado
                                    {
                                        Jugadores[busca].setAux(pos);               //Se guarda en el usuario actual la posicion en el vector del usuario igual encontrado
                                        send(Jugadores[busca].getSocket(),"+Ok. Usuario correcto\n",sizeof("+Ok. Usuario correcto\n"),0);
                                        Jugadores[pos].setEstado(ESPERANDO_PASSWORD);
				                    }
				                    else
                                        send(Jugadores[busca].getSocket(),"-Err. No se puede realizar la accion, ese usuario esta conectado\n",sizeof("-Err. No se puede realizar la accion, ese usuario esta conectado\n"),0);
                                }
                               else 
                               {
                                   send(Jugadores[busca].getSocket(),"-Err. Este jugador ya tiene un nombre de usuario asignado\n",sizeof("-Err. Este jugador ya tiene un nombre de usuario asignado\n"),0);
                                }
                            } 
                            else
                               send(Jugadores[busca].getSocket(),"-Err. Accion invalida, nombre de jugador no registrado\n",sizeof("-Err. Accion invalida, nombre de jugador no registrado\n"),0);
                        }

                        if(strncmp("PASSWORD ",buffer,8)==0)
                        {
                            entra=true;
                            char passwordAux[250];
                            string pass=buffer;
                            string asd = pass.substr(9,string::npos);
                            busca = localizaJugador(i,Jugadores);
                            if(Jugadores[Jugadores[busca].getAux()].getEstado() == ESPERANDO_PASSWORD or Jugadores[busca].getEstado() == ESPERANDO_PASSWORD)
                            {
                                if(Jugadores[busca].getPassword().length() == 0)
                                {
                                    if(asd.compare(0,asd.size()-1,Jugadores[Jugadores[busca].getAux()].getPassword()) == 0)
                                    {
                                       //std::cout<<Jugadores[busca].getAux()<<"xdxd"<<busca<<endl; //Son iguales 
                                       int r=Jugadores[busca].getAux(); //Se guarda en r la posicion del jugador que va a reemplazar al actual
                                       send(Jugadores[busca].getSocket(),"+Ok. Usuario validado\n",sizeof("+Ok. Usuario validado\n"),0);
                                       Jugadores[r].setSocket(Jugadores[busca].getSocket());
                                       Jugadores.erase(Jugadores.begin()+busca);
                                       Jugadores[r].setEstado(REGISTRADO_SIN_PARTIDA);   
                                    }
                                    else 
                                        send(Jugadores[busca].getSocket(),"-Err. Password incorrecta\n",sizeof("-Err. Password incorrecta\n"),0); 
                                }
                                else 
                                   send(Jugadores[busca].getSocket(),"-Err. No se puede realizar la accion\n",sizeof("-Err. No se puede realizar la accion\n"),0); 
                            }
                            else 
                               send(Jugadores[busca].getSocket(),"-Err. Accion invalida\n",sizeof("-Err. Accion invalida\n"),0); 
                        }
                         
                        if(strncmp("REGISTRO ",buffer,9)==0)
                        {
                            entra=true;
                            busca = localizaJugador(i,Jugadores);
                            string aux=buffer; 
                            size_t foundName=aux.find("-u ");
                            size_t foundPass=aux.find("-p ");
                            if(Jugadores[busca].getEstado() == 0 or Jugadores[busca].getEstado() == 5)
                            {
                                //Comprobamos que hemos encontrado -u y -p en el mensaje del cliente
                                if((foundName==string::npos)||(foundPass==string::npos))
                                    send(Jugadores[busca].getSocket(),"-Err. Formato incorrecto, recuerde que es:REGISTRO -u <usuario> -p <contraseña>\n",sizeof("-Err. Formato incorrecto, recuerde que es:REGISTRO -u <usuario> -p <contraseña>\n"),0); 
                                else
                                {
                                //Comprobamos que -u está antes que -p en el mensaje del cliente
                                    if(foundName<foundPass)
                                    {
                                       string nombre,password;
                                       regex e("(^REGISTRO )(.*)"); 
                                       aux=regex_replace(aux,e,"$2");
                                       string aux2=aux;
                                       std::regex u("-u ([a-zA-Z0-9]*)"); 
                                       aux2=regex_replace(aux2,u,"$1");
                                       regex x("(^[a-zA-Z0-9]*)( .*)");
                                       nombre=regex_replace(aux2,x,"$1");
                                       nombre=nombre.substr(0,nombre.size()-1);
                                       std::regex v("^[a-zA-Z0-9]* -p ([a-zA-Z0-9]*)"); 
                                       password=regex_replace(aux2,v,"$1");
                                       password=password.substr(0,password.size()-1);
                                        if(ExisteJugador(Jugadores,nombre))
                                            send(Jugadores[busca].getSocket(),"-Err. Nombre no disponible, ya en uso\n",sizeof("-Err. Nombre no disponible,ya en uso\n"),0);  
                                        else
                                        {
                                            Jugadores[busca].setNombre(nombre);
                                            Jugadores[busca].setPassword(password);
                                            Jugadores[busca].setEstado(REGISTRADO_SIN_PARTIDA);
                                            send(Jugadores[busca].getSocket(),"+Ok. Jugador registrado correctamente\n",sizeof("+Ok. Jugador registrado correctamente\n"),0); 
                                            escribirfichero(nombrefich,Jugadores[busca]);
                                        }                                 
                                    }
                                }
                            }
                            else
                                send(Jugadores[busca].getSocket(),"-Err. Jugador ya registrado\n",sizeof("-Err. Jugador ya registrado\n"),0);  
                           imprimirJugadores(Jugadores);
                        }
                        if(strncmp("INICIAR PARTIDA",buffer,14)== 0)
                        {
                            entra=true;
                            int jugador2 = localizaJugador(i,Jugadores);
                            if(Jugadores[jugador2].getEstado() == REGISTRADO_SIN_PARTIDA)
                            {
                                Jugadores[jugador2].setEstado(REGISTRADO_BUSCANDO_PARTIDA);
                                Cola.push_back(Jugadores[jugador2]);
                                if(Cola.size() == 2)    //Si hay emparejamiento
                                {
                                    int jugador1 = localizaJugador(Cola[0].getSocket(), Jugadores);
                                    Jugadores[jugador2].setEstado(REGISTRADO_JUGANDO);
                                    Jugadores[jugador1].setEstado(REGISTRADO_JUGANDO);
                                    Cola.clear();
                                    if(Cola.size() != 0)
                                        std::cout<<"Error borrando vector Cola, tamaño: "<<Cola.size()<<std::endl;
                                    Partida nueva(Jugadores[jugador1],Jugadores[jugador2]);
                                    Partidas.push_back(nueva);
                                    int partidaActual=Partidas.size() - 1;
                                    Jugadores[jugador2].setPartida(partidaActual);
                                    Jugadores[jugador1].setPartida(partidaActual);      
                                    Partidas[partidaActual].getTablero().generarPartida();

                                    char xddd[255];
                                    bzero(xddd,sizeof(xddd));

                                    strcpy(xddd,"+Ok. Se ha encontrado un jugador oponente, empezando partida\n");
                                    send(Partidas[partidaActual].getJugadorTurno().getSocket(),xddd,sizeof(xddd),0);  
                                    
                                    strcpy(xddd,Partidas[partidaActual].impresoPart());
                                    send(Partidas[partidaActual].getJugadorTurno().getSocket(),xddd,sizeof(xddd),0);
                                    
                                    strcpy(xddd,"+Ok. Se ha encontrado un jugador oponente, empezando partida\n");
                                    send(Partidas[partidaActual].getJugadorNoTurno().getSocket(),xddd,sizeof(xddd),0);  
                                    
                                    strcpy(xddd,Partidas[partidaActual].impresoPart());
                                    send(Partidas[partidaActual].getJugadorNoTurno().getSocket(),xddd,sizeof(xddd),0);
                                    
                                    strcpy(xddd,"+Ok. Es tu turno\n");
                                    send(Partidas[partidaActual].getJugadorTurno().getSocket(),xddd,sizeof(xddd),0);  

                                    strcpy(xddd,"+Ok. Esperando a que el oponente acabe su turno\n");
                                    send(Partidas[partidaActual].getJugadorNoTurno().getSocket(),xddd,sizeof(xddd),0);  
                                }
                                else    //Si no hay gente en cola para partida
                                    send(Jugadores[jugador2].getSocket(),"+Ok. Jugador puesto en cola correctamente, a la espera de otro jugador\n",sizeof("+Ok. Jugador puesto en cola correctamente, a la espera de otro jugador\n"),0);  
                            }
                            else
                               send(Jugadores[jugador2].getSocket(),"-Err. Accion invalida, no estas logeado o estas en partida\n",sizeof("-Err. Accion invalida, no estas logeado o estas en partida\n"),0); 
                        }

                        if(strncmp("DESCUBRIR ",buffer,9)== 0)
                        {
                            pthread_mutex_lock (&sem);
                            entra=true;
                            string pattern="(DESCUBRIR )([A-Z],[0-9]$)";
                            regex coincidencia2("DESCUBRIR [A-Z],[0-9]$");
                            regex coincidencia(pattern);
                            int jugador = localizaJugador(i,Jugadores);
                            if(Jugadores[jugador].getEstado() == REGISTRADO_JUGANDO) //Si estas en partida
                            {
                                int partida = Jugadores[jugador].getPartida();
                                if(Partidas[partida].esTurno(Jugadores[jugador])) //Si es su turno
                                {
                                    string descubre=buffer;
                                    descubre=descubre.substr(0,descubre.size()-1);
                                    if(!regex_match(descubre,coincidencia2)) send(Jugadores[jugador].getSocket(),"-Err. Mensaje erroneo\n",sizeof("-Err. Mensaje erroneo\n"),0); 
                                    else{
                                    string numerocas=regex_replace(descubre,coincidencia,"$2");
                                    string numero  = numerocas.substr(2);             //La letra
                                    string letra = numerocas.substr(0,1);               //El numero
                                    int num = std::stoi(numero);
                                    if((num >= 0 && num < 10)) //Casilla valida
                                    {
                                        if(!Partidas[partida].getTablero().CasillaDescubierta(letra,num))        //Si ya esta descubierta illo pasar de letra a numero joder
                                        {
                                            int status = Partidas[partida].descubrirCasilla(letra,num);
                                            if(status == 1) //Ha perdido
                                            {
                                               send(Partidas[partida].getJugadorTurno().getSocket(),"+Ok.Has pisado una mina. Has perdido y problablemente has muerto\n",sizeof("+Ok.Has pisado una mina. Has perdido y problablemente has muerto\n"),0);
                                               send(Partidas[partida].getJugadorNoTurno().getSocket(),"+Ok.El otro jugador ha pisado una mina y ahora esta muerto. Has ganado\n",sizeof("+Ok.El otro jugador ha pisado una mina y ahora esta muerto. Has ganado\n"),0);
                                               Jugadores[localizaJugador(Partidas[partida].getJugadorNoTurno().getSocket(), Jugadores)].setEstado(REGISTRADO_SIN_PARTIDA);
                                               Jugadores[localizaJugador(Partidas[partida].getJugadorTurno().getSocket(), Jugadores)].setEstado(REGISTRADO_SIN_PARTIDA);
                                               send(Partidas[partida].getJugadorTurno().getSocket(),"+Fin del juego, podeis volveros a poner en cola con 'INICIAR PARTIDA'\n",sizeof("+Fin del juego, podemos volveros a poner en cola con 'INICIAR PARTIDA'\n"),0);
                                               send(Partidas[partida].getJugadorNoTurno().getSocket(),"+Fin del juego, podeis volveros a poner en cola con 'INICIAR PARTIDA'\n",sizeof("+Fin del juego, podemos volveros a poner en cola con 'INICIAR PARTIDA'\n"),0);
                                               eliminar_partida(Partidas,partida);
                                            }
                                            if(status==0)
                                            {
                                               char xdd[255];
                                               bzero(xdd,sizeof(xdd));

                                               std::cout<<"1"<<Partidas[partida].impresoPart()<<endl;
                                               strcpy(xdd,Partidas[partida].impresoPart());
                                               send(Partidas[partida].getJugadorTurno().getSocket(),xdd,sizeof(xdd),0);

                                               strcpy(xdd,Partidas[partida].impresoPart());
                                               send(Partidas[partida].getJugadorNoTurno().getSocket(),xdd,sizeof(xdd),0);

                                               strcpy(xdd,"+Ok. Esperando a que el oponente acabe su turno\n");
                                               send(Partidas[partida].getJugadorNoTurno().getSocket(),xdd,sizeof(xdd),0); 

                                               strcpy(xdd,"+Ok. Es tu turno\n");
                                               send(Partidas[partida].getJugadorTurno().getSocket(),xdd,sizeof(xdd),0);  
                                            }
                                            if(status == -1)
                                            {
                                                char aviso[255];
                                                strcpy(aviso,"+Err. Casilla ya descubierta, prueba con otra crack");
                                                send(Partidas[partida].getJugadorTurno().getSocket(),aviso,sizeof(aviso),0);
                                            }
                                        }
                                        else
                                            send(Jugadores[jugador].getSocket(),"-Err. Accion invalida, casilla ya descubierta\n",sizeof("-Err. Accion invalida, casilla ya descubierta\n"),0); 
                                    }
                                    
                                    else
                                        send(Jugadores[jugador].getSocket(),"-Err. Accion invalida, casilla invalida\n",sizeof("-Err. Accion invalida, casilla invalida\n"),0); 
                                    }
                                }
                                else
                                    send(Jugadores[jugador].getSocket(),"-Err. Accion invalida, no es tu turno\n",sizeof("-Err. Accion invalida, no es tu turno\n"),0); 
                            }
                            else
                               send(Jugadores[jugador].getSocket(),"-Err. Accion invalida, no estas en partida\n",sizeof("-Err. Accion invalida, no estas en partida\n"),0); 
                          pthread_mutex_unlock (&sem);
                          
                        }
                        if(strncmp("BANDERA ",buffer,7)== 0)
                        {
                            pthread_mutex_lock (&sem);
                            entra=true;
                            string pattern="(BANDERA )([A-Z],[0-9]$)";
                            regex coincidencia2("BANDERA [A-Z],[0-9]$");
                            regex coincidencia(pattern);
                            int jugador = localizaJugador(i,Jugadores);
                            if(Jugadores[jugador].getEstado() == REGISTRADO_JUGANDO) //Si estas en partida
                            {
                                int partida = Jugadores[jugador].getPartida();
                                if(Partidas[partida].esTurno(Jugadores[jugador])) //Si es su turno
                                {
                                    string bandera=buffer;
                                    bandera=bandera.substr(0,bandera.size()-1);
                                    if(!regex_match(bandera,coincidencia2)) send(Jugadores[jugador].getSocket(),"-Err. Mensaje erroneo\n",sizeof("-Err. Mensaje erroneo\n"),0); 
                                    else
                                    {
                                        string numerocas=regex_replace(bandera,coincidencia,"$2");
                                        string numero  = numerocas.substr(2);             //La letra
                                        string letra = numerocas.substr(0,1);               //El numero
                                        int num = std::stoi(numero);
                                        if((num >= 0 && num < 10)) //Casilla valida
                                        {
                                            std::cout<<"casilla "<<letra<<" "<<num<<endl;
                                            int status = Partidas[partida].ponerBandera(letra,num,Partidas[partida].getTurno());
                                            std::cout<<"return "<<status<<endl;
                                            if(status == 1) //Tiene 10 banderas
                                            {
                                                char resultado[255];
                                                bzero(resultado,sizeof(resultado));

                                                strcpy(resultado,Partidas[partida].impresoPart());
                                                send(Partidas[partida].getJugadorTurno().getSocket(),resultado,sizeof(resultado),0);
                                                send(Partidas[partida].getJugadorNoTurno().getSocket(),resultado,sizeof(resultado),0);

                                                strcpy(resultado,"Ok. Ya has introducido 10 banderas, comprobando si tu solucion es correcta");
                                                send(Partidas[partida].getJugadorTurno().getSocket(),resultado,sizeof(resultado),0);

                                                if(Partidas[partida].comprobarBanderas(Partidas[partida].getTurno())) //Ha ganado
                                                {
                                                    strcpy(resultado,"+Ok. Has ganado, todas las banderas son correctas");
                                                    send(Partidas[partida].getJugadorTurno().getSocket(),resultado,sizeof(resultado),0);
                                                    strcpy(resultado,"+Ok. Has perdido, el jugador oponente ha acertado todas las banderas");
                                                    send(Partidas[partida].getJugadorNoTurno().getSocket(),resultado,sizeof(resultado),0);
                                                }
                                                else //Ha perdido
                                                {
                                                    strcpy(resultado,"+Ok. Has ganado, todas las banderas son correctas");
                                                    send(Partidas[partida].getJugadorNoTurno().getSocket(),resultado,sizeof(resultado),0);
                                                    strcpy(resultado,"+Ok. Has perdido, el jugador oponente ha acertado todas las banderas");
                                                    send(Partidas[partida].getJugadorTurno().getSocket(),resultado,sizeof(resultado),0);
                                                }
                                                send(Partidas[partida].getJugadorTurno().getSocket(),"+Fin del juego, podeis volveros a poner en cola con 'INICIAR PARTIDA'\n",sizeof("+Fin del juego, podemos volveros a poner en cola con 'INICIAR PARTIDA'\n"),0);
                                                send(Partidas[partida].getJugadorNoTurno().getSocket(),"+Fin del juego, podeis volveros a poner en cola con 'INICIAR PARTIDA'\n",sizeof("+Fin del juego, podemos volveros a poner en cola con 'INICIAR PARTIDA'\n"),0);
                                                Jugadores[localizaJugador(Partidas[partida].getJugadorNoTurno().getSocket(), Jugadores)].setEstado(REGISTRADO_SIN_PARTIDA);
                                                Jugadores[localizaJugador(Partidas[partida].getJugadorTurno().getSocket(), Jugadores)].setEstado(REGISTRADO_SIN_PARTIDA);
                                                eliminar_partida(Partidas,partida);
                                            }
                                            if(status==0)
                                            {
                                               char banderitas[255];
                                               bzero(banderitas,sizeof(banderitas));

                                               strcpy(banderitas,Partidas[partida].impresoPart());
                                               send(Partidas[partida].getJugadorTurno().getSocket(),banderitas,sizeof(banderitas),0);

                                               strcpy(banderitas,Partidas[partida].impresoPart());
                                               send(Partidas[partida].getJugadorNoTurno().getSocket(),banderitas,sizeof(banderitas),0);

                                               strcpy(banderitas,"+Ok. Esperando a que el oponente acabe su turno\n");
                                               send(Partidas[partida].getJugadorNoTurno().getSocket(),banderitas,sizeof(banderitas),0); 

                                               strcpy(banderitas,"+Ok. Es tu turno\n");
                                               send(Partidas[partida].getJugadorTurno().getSocket(),banderitas,sizeof(banderitas),0);  
                                            }
                                            if(status == -1) //Ya habia una bandera ahi
                                            {
                                                char aviso[255];
                                                bzero(aviso, sizeof(aviso));
                                                strcpy(aviso, "-Err. Ya hay una bandera tuya en esa casilla, pruebe otra vez");
                                                send(Partidas[partida].getJugadorTurno().getSocket(),aviso,sizeof(aviso),0);  
                                            }
                                        }
                                    }
                                }
                                else
                                    send(Jugadores[jugador].getSocket(),"-Err. Accion invalida, no es tu turno\n",sizeof("-Err. Accion invalida, no es tu turno\n"),0); 
                            }
                            else
                               send(Jugadores[jugador].getSocket(),"-Err. Accion invalida, no estas en partida\n",sizeof("-Err. Accion invalida, no estas en partida\n"),0); 
                          pthread_mutex_unlock (&sem);
                        }

                        if(strncmp("HELP",buffer,4)== 0)
                        {

                        }

                        if(entra==false)
                        {
                            busca = localizaJugador(i,Jugadores);
                            //send(Jugadores[busca].getSocket(),"-Err. Comando invalido\n",sizeof("-Err.Comando invalido\n"),0); 

                        }
                    }
            		
            	}
            }
            }

        }

    }
    pthread_mutex_destroy(&sem);
    close(sd);
	return 0;
}

bool ExisteJugador(std::vector<Jugador> v, string j)
{
	for (int i = 0; i < v.size(); ++i)
	{
		if(j == v[i].getNombre())
			return true;
	}
	return false;
}

bool ExisteJugador(std::vector<Jugador> v, Jugador j)
{
	for (int i = 0; i < v.size(); ++i)
	{
		if(j == v[i])
			return true;
	}
	return false;
}

void imprimirJugadores(std::vector<Jugador> v)
{
    int noregistrados=0;
    if(v.size() != 0)
    {
        for (int i = 0; i < v.size(); ++i)
        {
            if(v[i].getNombre().size() == 0)
                noregistrados++;
            else
                std::cout<<v[i].getNombre()<<" "<<v[i].getPassword()<<" "<<v[i].getEstado()<<std::endl;
        }
        std::cout<<"Hay "<<noregistrados<<" jugadores no registrados en el servidor"<<std::endl;

    }
    else
        std::cout<<"No hay jugadores en la lista"<<std::endl;
}

void salirCliente(int socket, fd_set * readfds, std::vector<Jugador> v)
{

    char buffer[250];
    send(socket,"+Ok. Desconexion servidor\n",sizeof("+Ok. Desconexion servidor\n"),0);  
    close(socket);
    FD_CLR(socket,readfds);
    int aux;
    int busca = localizaJugador(socket,v);
    v[busca].setEstado(5);

    
}

int localizaJugador(int socket, std::vector<Jugador> v)
{
    for (int i = 0; i < v.size(); ++i)
    {
        if(v[i].getSocket() == socket)
            return i;
    }
    return -1;
}
int localizaJugador(string nombre, std::vector<Jugador> v)
{
    for (int i = 0; i < v.size(); ++i)
    {
        if(nombre.compare(0,nombre.size()-1,v[i].getNombre())== 0)
            return i;
    }
    return -1;
}

void eliminar_partida(std::vector<Partida> &p,int partida){ 
  //  pthread_mutex_lock (&sem);
    p.erase(p.begin()+partida);
    for(int i=partida;i<p.size();i++)
            p[i].cambiarPartida(i);
  //  pthread_mutex_unlock (&sem);
}

int buscarJugadorPartida(Jugador j, std::vector<Partida> partidas)
{
  for (int i = 0; i <partidas.size() ; ++i)
  {
    if(partidas[i].Jugador1().getSocket() == j.getSocket() or partidas[i].Jugador2().getSocket() == j.getSocket())
        return i;
  }
  return -1;
}

void escribirfichero(string nombrefich, Jugador j)
{
    fstream fichero;
    fichero.open(nombrefich.c_str(), ios::out | ios::app);
    if(fichero.is_open())
    {
            if(j.getNombre().length() != 0)
                    fichero<<j.getNombre()<<" "<<j.getPassword()<<"\n";
        fichero.close();
    }
    else
        std::cout<<"error\n";

}
