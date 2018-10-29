#include "Partida.hpp"
#include "Buscaminas.hpp"
#include "Jugador.hpp"

int Partida::descubrirCasilla(std::string letra,int j){
   int i = letraNumero(letra);
   //turno_=(turno_+1)%2;
   int resultado = busc_.descubrirCasilla(j,i);
  if(resultado == 1)
    return 1;
  else
    if(resultado == 0)
    {
      turno_=(turno_+1)%2;
      return 0;
    }
    if(resultado == -1)
      return -1;
}




int Partida::ponerBandera(int i,int j,int jugador){
                 turno_=(turno_+1)%2;
                 if(!busc_.ponerBandera(i,j, jugador)) return -1;
                 else if(busc_.getBanderasJugador(jugador)==10) return 1;
                 else{ busc_.imprimir(); return 0;}
           }
int Partida::letraNumero(std::string letra)
{

  int i;
  if(letra.compare("A")==0) i=0;
  if(letra.compare("B")==0) i=1;
  if(letra.compare("C")==0) i=2;
  if(letra.compare("D")==0) i=3;
  if(letra.compare("E")==0) i=4;
  if(letra.compare("F")==0) i=5;
  if(letra.compare("G")==0) i=6;
  if(letra.compare("H")==0) i=7;
  if(letra.compare("I")==0) i=8;
  if(letra.compare("J")==0) i=9;

  return i;

}
