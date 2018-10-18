#ifndef JUGADOR_HPP
#define JUGADOR_HPP
#include <cassert>
#include <string>
#define SIN_REGISTRAR 0
#define ESPERANDO_PASSWORD 1
#define REGISTRADO_SIN_PARTIDA 2
#define REGISTRADO_BUSCANDO_PARTIDA 3
#define REGISTRADO_JUGANDO 4
#define DESCONECTADO 5
/*Estados del jugador
   0->Conectado sin registrar
   1->Esperando contraseña
   2->Registrado sin partida
   3->Registrado buscando partida
   4->Registrado jugando
   5->Desconectado
*/
class Jugador{
    private:
     int socket_;
     std::string nombre_;
     std::string password_;
     int estado_;
    public:
     int getSocket(){return socket_;}
     std::string getNombre(){return nombre_;}
     void setSocket(int socket){socket_=socket;}
     void setNombre(std::string nombre){nombre_=nombre;}
     void setEstado(int estado){estado_=estado;}
     
     bool operator==(Jugador j){
                     if(this->nombre_==j.nombre_) return true;
                     else return false;
       }



};

#endif
