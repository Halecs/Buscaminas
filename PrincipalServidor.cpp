#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include<signal.h>
#include <unistd.h>
#include <ctime>
#include <arpa/inet.h>
#include <vector>
#include <queue>
#include <iostream>

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

int main(int argc, char const *argv[])
{
	int sd, new_sd; //Sockets 
	struct sockaddr_in sockname,from;
	fd_set readfds, auxfds;
    socklen_t from_len;
	int salida,on,ret, busca;

	std::vector<Jugador> Jugadores;
	std::vector<Partida> Partidas;
    //Necesitamos un vector de cola de emparejamiento
    std::vector<Jugador> Cola;

	string mensajeError = "Error no se puede realizar la accion";
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
                    	string mensaje = "+ OK\n";
                    	send(aux.getSocket(),mensaje.c_str(),mensaje.size(),0);
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
            				send(Jugadores[i].getSocket(), "Desconexion servidor\n", sizeof("Desconexion servidor\n"),0);
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
                        std::cout<<buffer<<endl;
                        /*Queda ver como se desconecta si esta en partida*/
                        if(strncmp("Salir", buffer,5) == 0)
                        {
                            cout<<"he entrado en salir"<<endl;
                            busca = localizaJugador(i,Jugadores);
                            if(Jugadores[busca].getEstado() != 3 && Jugadores[busca].getEstado() != 4)
                                salirCliente(i,&readfds,Jugadores);
                            else // Si estan en partida o buscando
                            {
                                //Registrado buscando partida
                                if(Jugadores[busca].getEstado() == 3)
                                {   
                                    for (int l = 0; l < Cola.size(); ++l)
                                    {
                                        if(Cola[l].getSocket() == Jugadores[busca].getSocket())
                                            Cola.erase(Cola.begin() + (l)-1); //???
                                        Jugadores[busca].setEstado(DESCONECTADO);
                                    }
                                }
                                //En partida
                                if(Jugadores[busca].getEstado() == 4)
                                {
                                    int partida = buscarJugadorPartida(Jugadores[busca], Partidas); //Encontramos la partida
                                    int elotro = Partidas[partida].encontrarJugadorOponente(Jugadores[busca].getSocket()); //Buscamos el socket del otro jugador de la partida
                                    if(elotro == 0) // El jugador 1 es el que se va a salir
                                    {
                                        send(Jugadores[busca].getSocket(),mensajeDesconexionPartida2.c_str(),sizeof(mensajeDesconexionPartida2),0);
                                        send(Partidas[partida].socketJugador1(),mensajeDesconexionPartida.c_str(),sizeof(mensajeDesconexionPartida),0);
                                        Jugadores[busca].setEstado(DESCONECTADO);
                                        Jugadores[localizaJugador(Partidas[partida].Jugador1().getSocket(), Jugadores)].setEstado(REGISTRADO_SIN_PARTIDA);
                            
                                    }
                                    else
                                    {
                                        send(Jugadores[busca].getSocket(),mensajeDesconexionPartida2.c_str(),sizeof(mensajeDesconexionPartida2),0);
                                        send(Partidas[partida].socketJugador2(),mensajeDesconexionPartida.c_str(),sizeof(mensajeDesconexionPartida),0);
                                        Jugadores[busca].setEstado(DESCONECTADO);
                                        Jugadores[localizaJugador(Partidas[partida].Jugador2().getSocket(), Jugadores)].setEstado(REGISTRADO_SIN_PARTIDA);
                                    }
                                    //Volver a poner en cola quizas mas adelante
                                }
                            }
                        }



                        if(strncmp("USUARIO ",buffer,7) == 0)
                        {

                            char usuarioAux[250];
                            string user=buffer;
                            user.copy(usuarioAux,250,8);
			    string usuario=usuarioAux;
                            busca = localizaJugador(i,Jugadores);
                            cout<<"El nombre de usuario="<<usuario<<endl;
                            if(Jugadores[busca].getEstado() == SIN_REGISTRAR)
                            {

                                if(Jugadores[busca].getNombre().length() == 0)
                                {
                                    int pos=localizaJugador(usuario,Jugadores);
                                    if( pos!=-1)
                                    {
                                      Jugadores[busca].setAux(pos);
                                      send(Jugadores[busca].getSocket(),"+Ok. Usuario correcto\n",sizeof("+Ok. Usuario correcto\n"),0);
                                      Jugadores[busca].setEstado(ESPERANDO_PASSWORD);
                                    }
                                    else
                                      send(Jugadores[busca].getSocket(),mensajeError.c_str(),mensajeError.length(),0);
                                }
                               else 
                                   send(Jugadores[busca].getSocket(),mensajeError.c_str(),mensajeError.length(),0);
                            } 
                            else{
                               std::cout<<"El usuario "<<busca<<" esta ya dentro o no esta registrado"<<endl;
                               send(Jugadores[busca].getSocket(),"adios\n",sizeof("adios\n"),0);}
                        }

                        if(strncmp("PASSWORD ",buffer,8)==0){
                              char passwordAux[250];
                              string pass=buffer;
                              pass.copy(passwordAux,250,9);
			      string password=passwordAux;
                              busca = localizaJugador(i,Jugadores);
                            if(Jugadores[busca].getEstado() == ESPERANDO_PASSWORD)
                            {
                                if(Jugadores[busca].getNombre().length() == 0)
                                {
                               
                                    if(Jugadores[Jugadores[busca].getAux()].getPassword()==password )
                                    {
                                       int r=Jugadores[busca].getAux();
                                       send(Jugadores[busca].getSocket(),"+Ok. Usuario validado",sizeof("+Ok. Usuario validado"),0);
                                       Jugadores[r].setSocket(Jugadores[busca].getSocket());
                                       Jugadores.erase(Jugadores.begin()+busca-1);
                                       Jugadores[r].setEstado(REGISTRADO_SIN_PARTIDA);   
                                    }
                                    else 
                                        send(Jugadores[busca].getSocket(),mensajeError.c_str(),mensajeError.length(),0); 
                                }
                                else 
                                   send(Jugadores[busca].getSocket(),mensajeError.c_str(),mensajeError.length(),0); 
                            }
                            else 
                               send(Jugadores[busca].getSocket(),mensajeError.c_str(),mensajeError.length(),0); 
                        }
                         
                        if(strncmp("REGISTRO ",buffer,9)==0){
                            string aux=buffer; 
                            size_t foundName=aux.find("-u ");
                            size_t foundPass=aux.find("-p ");
                            if((foundName==string::npos)||(foundPass==string::npos))
                                send(Jugadores[busca].getSocket(),mensajeError.c_str(),mensajeError.length(),0); 
                            else
                            {
                                if(foundName<foundPass)
                                {
                                    char nom[250],pass[250];
                                    string nombre,password;
                                    int namesize=(foundPass-foundName-3);
                                    aux.copy(nom,namesize,foundName+3);
                                    aux.copy(pass,250,foundPass+3);
                                    nombre=nom; password=pass;
                                    if(ExisteJugador(Jugadores,nombre))
                                        send(Jugadores[busca].getSocket(),"Nombre ya usado\n",sizeof("Nombre ya usado\n"),0);  
                                    else{
                                    
                                    }                                 
                                }
                            }    
                        }
                    }
            		
            	}
            }
            }

        }

    }
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
    for (int i = 0; i < v.size(); ++i)
        std::cout<<v[i].getNombre()<<std::endl;
}

void salirCliente(int socket, fd_set * readfds, std::vector<Jugador> v)
{

    char buffer[250];
    send(socket,"Adios,xd\n",sizeof("Adios,xd\n"),0);  
    close(socket);
    FD_CLR(socket,readfds);
    int aux;
    for (int i = 0; i < v.size(); ++i)
    {
        if(socket == v[i].getSocket())
            v[i].setEstado(5);
    }

    
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
        if(v[i].getNombre() == nombre)
            return i;
    }
    return -1;
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
