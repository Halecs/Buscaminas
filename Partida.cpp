#include "Partida.hpp"
#include "Buscaminas.hpp"
#include "Jugador.hpp"

int Partida::descubrirCasilla(int i,int j){
   turno_=(turno_+1)%2;
   int resultado=busc_.descubrirCasilla(i,j);
   if(resultado==1){
        busc_.imprimir();
        jugadores_[0].setEstado(REGISTRADO_SIN_PARTIDA);
        jugadores_[1].setEstado(REGISTRADO_SIN_PARTIDA);
        return 1;}
  
   else if(resultado==-1) return -1;
   else{ busc_.imprimir(); return 0;}
}




int Partida::ponerBandera(int i,int j,int jugador){
                 turno_=(turno_+1)%2;
                 if(!busc_.ponerBandera(i,j, jugador)) return -1;
                 else if(busc_.getBanderasJugador(jugador)==10) return 1;
                 else{ busc_.imprimir(); return 0;}
           }
