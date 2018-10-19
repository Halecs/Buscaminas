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
#include <iostream>

#include "Jugador.hpp"
#include "Partida.hpp"

using namespace std;

bool ExisteJugador(std::vector<Jugador> v, Jugador j);
void imprimirJugadores(std::vector<Jugador> v);
void salirCliente(int socket, fd_set * readfds, std::vector<Jugador> v);
int localizaJugador(int socket, std::vector<Jugador> v);



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
            		/* USUARIO, JUGAR, BUSCAR PARTIDA, LOGIN*/
            		/* 1º)	LOGIN*/
                    char buffer[50];
                    bzero(buffer,sizeof(buffer));
                    recv(i,buffer,sizeof(buffer),0);
                    if(buffer > 0)
                    {
                        /*Queda ver como se desconecta si esta en partida*/
                        if(strcmp("Salir\n", buffer) == 0)
                        {
                            busca = localizaJugador(i,Jugadores);
                            if(Jugadores[busca].getEstado() != 3 && Jugadores[busca].getEstado() != 4)
                                salirCliente(i,&readfds,Jugadores);
                            else // Si estan en partida o buscando
                            {
                                //Registrado buscando partida
                                if(Jugadores[busca].getEstado() == 3)
                                {   
                                    for (int i = 0; i < Cola.size(); ++i)
                                    {
                                        if(Cola[i].getSocket() == Jugadores[busca].getSocket())
                                            Cola.erase(Cola.begin() + (i-1)); //???
                                        Jugadores[busca].setEstado(DESCONECTADO);
                                    }
                                }
                                //En partida
                                if(Jugadores[busca].getEstado() == 4)
                                {
                                    
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
