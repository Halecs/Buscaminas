#include "Buscaminas.hpp"
#include <vector>
#include <cstdlib>

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
      if(buscaminas_[x][y].mina==true) i--;
      else buscaminas_[x][y].mina==true;
  }
}

/* Calcula el numero de minas alrededor */
void Buscaminas::generarNumeros(){

   for(int i=0;i<buscaminas_.size();i++){

      for(int j=0;j<buscaminas_.size();j++){

         if(!buscaminas_[i][j].mina){
          
              for(int k=i-1;k<i+1;k++){  
  
                 for(int l=j-1;l<j+1;l++){
                   
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

  // Imprimir banderas?
  // ¿Imprimir mina?
  for (int i = 0; i < buscaminas_.size(); ++i)
  {
    cout<<"\n";
    for (int j = 0; j < buscaminas_.size(); ++j)
    {
      if(buscaminas_[i][j].descubierto)
        cout<<buscaminas_[i][j].minasAlrededor;
      else
        cout<<"- ";
    }
  }
}

void ponerBandera(int i,int j, int jugador)
{
  if(!buscaminas_[i][j].descubierto && buscaminas_[i][j].bandera != jugador && buscaminas_[i][j].bandera != 3)
    buscaminas_[i][j].bandera += jugador;
}
