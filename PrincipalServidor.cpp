#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstdlib>
#include <string>
#include<signal.h>
#include <unistd.h>
#include <ctime>
#include <arpa/inet.h>
#include <vector>

#include "Jugador.hpp"
#include "Partida.hpp"

int main(int argc, char const *argv[])
{
	int sd, new_sd; //Sockets 
	struct sockaddr_in sockname,from;
	fd_set readfds, auxfds;
	int salida,ret;

	std::vector<Jugador> Jugadores;
	std::vector<Partida> Partidas;

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

                    	/*nombre = mensajeRec;
                    	if(NoExisteJugador(Jugadores, aux))
                    		aux.setNombre(nombre);
                    	else 
                    		send(aux.getSocket(),mensajeError.c_str(),mensaje.size(),0);*/
                    }

            	}
            	/* Si ha introducido el servidor algo por teclado */
            	else if(i==0)
            	{
            		/*Cogemos la cadena y comparamos con distintas opciones (SALIR, VER LISTA DE JUGADORES...) */
            		/* salir */
            		string entrada;
            		fgets(entrada, sizeof(entrada),stdin);
            		if("SALIR" == entrada)
            		{
            			for (int i = 0; i < Jugadores.size(); ++i)
            			{
            				send(Jugadores[i], "Desconexion servidor\n", strlen("Desconexion servidor\n"),0);
                            close(Jugadores[i]);
                            FD_CLR(Jugadores[i],&readfds);
            			}
            			close(sd);
            			exit(-1);
            		}
            	}
            	else
            	{
            		/*El usuario ha hecho algo*/
            		/* USUARIO, JUGAR, BUSCAR PARTIDA, LOGIN*/
            		/* 1º)	LOGIN*/
            		
            	}
            }

        }

    }
    close(sd);
	return 0;
}


bool NoExisteJugador(std::std::vector<Jugador> v, Jugador j)
{
	for (int i = 0; i < Jugador.size(); ++i)
	{
		if(j == v[i])
			return false;
	}
	return true;
}