#ifndef PARTIDA_HPP
#define PARTIDA_HPP

#include "Buscaminas.hpp"
#include "Jugador.hpp"
#define A 0
#define B 1
class Partida{
  private:
    int turno_;
    Buscaminas busc_;
    Jugador jugadores_[2];
  public:
    Partida(Jugador jugador1,Jugador jugador2){
                jugadores_[0]=jugador1; jugadores_[1]=jugador2; turno_=0;}

    int descubrirCasilla(int i,int j);

    int ponerBandera(int i,int j,int jugador);

    bool esTurno(Jugador j){if(jugadores_[turno_]==j) return true;return false;}

    bool existeJugador(Jugador j){ if((jugadores_[0]==j)||(jugadores_[1]==j)) return true; return false;}
   
    
    






};



#endif
