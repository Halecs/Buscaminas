CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(practica1_ed)
set (CMAKE_CXX_FLAGS "-lrt -g -std=c++11")
add_executable(PrincipalServidor PrincipalServidor.cpp Partida.cpp Buscaminas.cpp)
add_executable(PrincipalCliente PrincipalCliente.cpp)
 
