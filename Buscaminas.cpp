#include "Buscaminas.hpp"
#include <vector>
#include <cstdlib>
#include <string>
#include <iostream>

/* Inicializa todo*/
void Buscaminas::generarPartida(){
   for(int i=0;i<buscaminas_.size();i++){
      for(int j=0;j<buscaminas_.size();j++){
            buscaminas_[i][j].bandera=0;
            buscaminas_[i][j].minasAlrededor=0; 
            buscaminas_[i][j].mina=false;
            buscaminas_[i][j].descubierto=false;  
      }
    }       
   generarMinas();
   generarNumeros();   
}

/* Inicializa minas*/
void Buscaminas::generarMinas(){
  for(int i=0;i<nMinas_;i++){
     int numAl=rand();
     int x=numAl%10;
     int y=(numAl%100)/10;
      if(buscaminas_[x][y].mina==true) 
      	i--;
      else 
      	buscaminas_[x][y].mina=true;
  }
}

/* Calcula el numero de minas alrededor */
void Buscaminas::generarNumeros(){

   for(int i=0;i<buscaminas_.size();i++){

      for(int j=0;j<buscaminas_.size();j++){

         if(!buscaminas_[i][j].mina){
          
              for(int k=i-1;k<=i+1;k++){  
  
                 for(int l=j-1;l<=j+1;l++){
                   
                   if((k>=0)&&(l>=0)&&(k<buscaminas_.size())&&(l<buscaminas_.size())){

                     if(buscaminas_[k][l].mina==true){
                         buscaminas_[i][j].minasAlrededor++;
                     }

                  }

               }

             }
             
          }

       }

   }

}

char* Buscaminas::imprimir()
{

  std::string impreso;
  impreso="  A B C D E F G H I J"; 
  for (int i = 0; i < buscaminas_.size(); ++i)
  {
    impreso+="\n";
    for (int j = 0; j < buscaminas_.size(); ++j)
    {
      if (j == 0)
      {
        int aux = i; 
        impreso += std::to_string(aux++);
        impreso += " ";

      }      
      if(buscaminas_[i][j].descubierto)
        {
          impreso+=std::to_string(buscaminas_[i][j].minasAlrededor);impreso+=" ";
        }
        else{
        	if(buscaminas_[i][j].bandera == 0)
          	impreso+="- ";
          else{
          	if(buscaminas_[i][j].bandera == 1)
          		impreso+="A ";
          	else if(buscaminas_[i][j].bandera == 2)
          		impreso+="B ";
          		else if(buscaminas_[i][j].bandera == 3)
          			impreso+="AB ";
        }
       
    }
  }
 }
  char* xd = &impreso[0]; 
  return xd;
}




bool Buscaminas::ponerBandera(int i,int j, int jugador)
{
	/*Si no esta descubierta y el jugador no ha puesto una bandera ya ahí*/
  if(!buscaminas_[i][j].descubierto && buscaminas_[i][j].bandera != jugador && buscaminas_[i][j].bandera != 3)
  {
    nBanderas_[jugador]++;
    buscaminas_[i][j].bandera += jugador;
  	return true;
  }
  else
  	return false;
}

/*Devuelve 1 si hay mina, 0 si se ha descubierto correctamente, -1 si ha habido un error */
int Buscaminas::descubrirCasilla(int i,int j)
{
       std::cout<<i<<","<<j<<std::endl;
	if(!buscaminas_[i][j].descubierto)
	{
		buscaminas_[i][j].descubierto = true;
    if((buscaminas_[i][j].minasAlrededor==0)&&(!buscaminas_[i][j].mina))
    {
      for(int k=i-1;k<=i+1;k++)
      {  
        for(int l=j-1;l<=j+1;l++)
        {
          if((k>=0)&&(l>=0)&&(k<buscaminas_.size())&&(l<buscaminas_.size())&&(k!=i)&&(l!=j))
            descubrirCasilla(k,l);
        }
      }
    }
		if(buscaminas_[i][j].mina == true)
			return 1;
		else 
			return 0;
	}
	else
		return -1;
}

bool Buscaminas::CasillaDescubierta(std::string letra, int j)
{
  int i=letraNumero(letra);

  return buscaminas_[i][j].descubierto;
}

int Buscaminas::letraNumero(std::string letra)
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
