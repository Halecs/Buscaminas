#ifndef BUSCAMINAS_HPP
#define BUSCAMINAS_HPP
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>

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
    bool descubierto;
};


class Buscaminas{
   private:
    std::vector < std::vector<casilla> > buscaminas_;
    int nMinas_;
    std::vector<int> nBanderas_;
    void generarMinas();
    void generarNumeros();
    
   public:
    Buscaminas(){
      buscaminas_.resize(10);
      for(int i=0;i<10;i++)
          buscaminas_[i].resize(10);  
      srand(time(NULL)); 
      nBanderas_.resize(3,0);
      nMinas_=20;     
      generarPartida();    
     }
    bool banderaCorrecta(int jugador);
    bool CasillaDescubierta(std::string letra, int j);
    int letraNumero(std::string letra);
    int getBanderasJugador(int jugador){return nBanderas_[jugador];}
    void generarPartida();
    std::string imprimir();
    bool ponerBandera(int i,int j, int jugador);
    int  descubrirCasilla(int i, int j);
    std::string imprimirTodo();
    std::string casillasDesc();
     
};


#endif
