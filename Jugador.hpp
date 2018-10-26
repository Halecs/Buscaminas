#ifndef JUGADOR_HPP
#define JUGADOR_HPP
#include <cassert>
#include <string>
const int SIN_REGISTRAR=0;
const int ESPERANDO_PASSWORD=1;
const int REGISTRADO_SIN_PARTIDA=2;
const int REGISTRADO_BUSCANDO_PARTIDA=3;
const int REGISTRADO_JUGANDO=4;
const int DESCONECTADO=5;
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
     int partida_;
     int aux;
    public:
     int getSocket(){return socket_;}
     std::string getNombre(){return nombre_;}
     int getEstado(){return estado_;}
     int getPartida(){return partida_;}
     int getAux(){return aux;}
     std::string getPassword(){return password_;}
     void setPartida(int partida){partida_=partida;}
     void setSocket(int socket){socket_=socket;}
     void setNombre(std::string nombre){nombre_=nombre;}
     void setEstado(int estado){estado_=estado;}
     void setPassword(std::string password){password_=password;}
     void setAux(int au){aux=au;}
     bool operator==(Jugador j){
                     if(this->nombre_==j.nombre_) return true;
                     else return false;
       }
    /*inline Jugador & operator=(Jugador const  &j)
    {
      setSocket(j.getSocket());
      setNombre(j.getNombre());
      setPassword(j.getPassword());
      setEstado(j.getEstado());
      setPartida(j.getPartida());
      setAux(j.getAux());
      return *this;
    }*/


};

#endif
