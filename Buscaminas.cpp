#include "Buscaminas.hpp"
#include <vector>
#include <cstdlib>
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

void Buscaminas::imprimir()
{

  // ¿Imprimir mina?
  for (int i = 0; i < buscaminas_.size(); ++i)
  {
    std::cout<<"\n";
    for (int j = 0; j < buscaminas_.size(); ++j)
    {
      if(buscaminas_[i][j].descubierto)
        std::cout<<buscaminas_[i][j].minasAlrededor<<" ";
      else
      	if(buscaminas_[i][j].bandera == 0)
        	std::cout<<"- ";
        else
        	if(buscaminas_[i][j].bandera == 1)
        		std::cout<<"A ";
        	else if(buscaminas_[i][j].bandera == 2)
        		std::cout<<"B ";
        		else if(buscaminas_[i][j].bandera == 3)
        			std::cout<<"AB ";
    }
  }
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
	if(!buscaminas_[i][j].descubierto)
	{
		buscaminas_[i][j].descubierto == true;
                if(buscaminas_[i][j].minasAlrededor==0){
                    for(int k=i-1;k<=i+1;k++){  
  
                      for(int l=j-1;l<=j+1;l++){
                   
                         if((k>=0)&&(l>=0)&&(k<buscaminas_.size())&&(l<buscaminas_.size())){
                               
                              descubrirCasilla(k,l);
                          } 
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
