#include "Partida.hpp"
#include "Buscaminas.hpp"
#include "Jugador.hpp"
#include <iostream>
#include <cstring>

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


bool Partida::comprobarBanderas(int jugador){
   return busc_.banderaCorrecta(jugador);}

int Partida::ponerBandera(std::string letra,int j,int jugador)
{
  int i = letraNumero(letra);
  jugador++;
  if(!busc_.ponerBandera(j,i, jugador)) 
    return -1;
  else{ 
    if(busc_.getBanderasJugador(jugador) == 10) 
      return 1;
    else
    {  
      turno_=(turno_+1)%2; 
      return 0;
    }
  }
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



char* Partida::impresoPart(){ 
   std::string imp="+Ok.Tablero.";
   std::string aux=busc_.imprimir();
   size_t space_pos = aux.find("\n");
   aux=aux.substr(space_pos+1);
   while( space_pos!=std::string::npos){

      std::string s=aux.substr(0,space_pos);
      if(s.size()>6){
      size_t finding=s.find(" ");
      s=s.substr(finding+1);

       while(finding!=std::string::npos){
           std::string x=s.substr(0,finding); 

           
           s=s.substr(finding+1);
           imp+=x;imp+=",";
           finding=s.find(" ");
        }
        imp+=s;
        if(imp.rfind(",")==imp.size()-1)imp=imp.substr(0,imp.size()-1);  
      imp+=";";
     }
      aux=aux.substr(space_pos+1);

      space_pos = aux.find("\n");
  }

char *povaia = new char[imp.length() + 1];
strcpy(povaia, imp.c_str());
return povaia;
   }

