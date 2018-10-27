#ifndef PARTIDA_HPP
#define PARTIDA_HPP

#include "Buscaminas.hpp"
#include "Jugador.hpp"
const int A=0;
const int B=1;
class Partida{
  private:
    int turno_;
    Buscaminas busc_;
    Jugador jugadores_[2];
  public:
    Partida(Jugador jugador1,Jugador jugador2){
                jugadores_[0]=jugador1; jugadores_[1]=jugador2; turno_=0;}


    int socketJugador1(){return jugadores_[0].getSocket();}
    int socketJugador2(){return jugadores_[1].getSocket();}
    Buscaminas getTablero(){return busc_;}
    Jugador Jugador1(){return jugadores_[0];}
    Jugador Jugador2(){return jugadores_[1];}
    int getTurno(){return turno_;}
    void cambiarPartida(int i){jugadores_[0].setPartida(i);jugadores_[1].setPartida(i);}
    Jugador getJugadorTurno(){return jugadores_[turno_];}
    Jugador getJugadorNoTurno(){return jugadores_[(turno_+1)%2];}
    int numeroDeJugador(int socket)
    {
        if(jugadores_[0].getSocket() == socket)
            return 1;
        if(jugadores_[1].getSocket() == socket)
            return 0;
    }
    int encontrarJugadorOponente(int socket){
        if(jugadores_[0].getSocket() == socket)
            return 1;
        if(jugadores_[1].getSocket() == socket)
            return 0;
    }
    int descubrirCasilla(std::string letra,int j);
    int letraNumero(std::string letra);

    int ponerBandera(int i,int j,int jugador);

    bool esTurno(Jugador j){if(jugadores_[turno_]==j) return true;return false;}

    bool existeJugador(Jugador j){ if((jugadores_[0]==j)||(jugadores_[1]==j)) return true; return false;}

    
    






};



#endif
