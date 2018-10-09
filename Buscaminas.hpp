#ifndef BUSCAMINAS_HPP
#define BUSCAMINAS_HPP
#include <vector>
#include <ctime>
#include <cstdlib>

class Buscaminas{
   private:
    std::vector < std::vector<casilla> > buscaminas_;
    int nMinas_;
    void generarMinas();
    void generarNumeros();
   public:
    Buscaminas(int nMinas){
      buscaminas_.resize(10);
      for(int i=0;i<10;i++)
          buscaminas_[i].resize(10);  
      srand(time(NULL)); 
      nMinas_=20;         
     }
     void generarBuscaminas();




};

struct casilla{
    int minasAlrededor;

    /*
    bandera=0 ----> No hay bandera
    bandera=1 ----> Bandera jugador 1
    bandera=2 ----> Bandera jugador 2
    bandera=3 ----> Bandera ambos jugadores
    */
    int bandera;
    bool mina;
    };
#endif
