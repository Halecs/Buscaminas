#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <iostream>

int main(){
  int sd;
    struct sockaddr_in sockname;
    socklen_t len_sockname;
    fd_set readfds, auxfds;
    int salida;
    int fin = 0;
 
  sd = socket (AF_INET, SOCK_STREAM, 0);
	if (sd == -1)
	{
		std::cout<<"No se puede abrir el socket cliente\n";
    		exit (1);	
	}

 	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(2000);
	sockname.sin_addr.s_addr =  inet_addr("127.0.0.1");

	
	len_sockname = sizeof(sockname);
	
	if (connect(sd, (struct sockaddr *)&sockname, len_sockname) == -1)
	{
		std::cout<<"Error de conexión\n";
		exit(1);
	}
    FD_ZERO(&auxfds);
    FD_ZERO(&readfds);
    
    FD_SET(0,&readfds);
    FD_SET(sd,&readfds);

    char buffer[250];
    
    do
    {
    	auxfds = readfds;
        salida = select(sd+1,&auxfds,NULL,NULL,NULL);

        //Si tengo un mensaje del servidor
        if(FD_ISSET(sd, &auxfds))
        {
        	bzero(buffer,sizeof(buffer));
            recv(sd,buffer,sizeof(buffer),0);
            if(strcmp(buffer,"+Ok. Desconexion servidor\n") == 0)	//Si el servidor se va a desconectar
            {
                fin =1;
                std::cout<<"Desconectado del servidor correctamente"<<std::endl;
            }
            else
            	std::cout<<buffer;
        }
        else
        {
        	//Introducido informacion por teclado
            if(FD_ISSET(0,&auxfds))
            {
                bzero(buffer,sizeof(buffer));
                fgets(buffer,sizeof(buffer),stdin);
                send(sd,buffer,sizeof(buffer),0);
                /*if(strcmp(buffer,"Salir\n") == 0)	//Si el cliente pide la desconexión
                {
                	fin = 1;
                	std::cout<<"Desconectada del servidor correctamente"<<std::endl;
                }	*/
            }

        }//SALIR DEL SERVIDOR

    }while(fin == 0);
  return 0;
   }

