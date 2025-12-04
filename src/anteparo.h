#ifndef ANTEPARO_H
#define ANTEPARO_H  

#include "pacote.h"
#include <string.h>
#include "lista.h"

/*
    Arquivo .h responsável pela criação e utilização de anteparos.
*/

typedef void* Anteparo;

/*
    Função responsável por criar o anteparo com os parâmetros passados.
*/
Anteparo criarAnteparo(int id, double x1,double y1,double x2, double y2, char* cor);

/*
    Função responsável por transformar um círculo em anteparo.
*/
Anteparo circuloViraAnteparo(Pacote pac, char orientacao);

/*
    Função responsável por transformar um retângulo em anteparo.
*/
Anteparo* retanguloViraAnteparo(Pacote pac, int maiorID);

/*
    Função responsável por transformar uma linha em anteparo.
*/
Anteparo linhaViraAnteparo(Pacote pac);

/*
    Função responsável por transformar um Texto em anteparo.
*/
Anteparo textoViraAnteparo(Pacote pac);

/*
    Função responsável por retornar o id do anteparo
*/
int getIDAnteparo(Anteparo a);

/*
    Retorna a coordenada x1 do anteparo
*/
double getX1Anteparo(Anteparo a);

/*
    Retorna a coordenada y1 do anteparo
*/
double getY1Anteparo(Anteparo a);

/*
    Retorna a coordenada x2 do anteparo
*/
double getX2Anteparo(Anteparo a);

/*
    Retorna a coordenada y2 do anteparo
*/
double getY2Anteparo(Anteparo a);

/*
    Retorna a cor do anteparo
*/
char* getCorAnteparo(Anteparo a);

/*
    Define a nova cor do anteparo 
*/
void setCorAnteparo(Anteparo a, char* novaCor);

/*
    Libera a memória do anteparo.
*/
void liberarAnteparo(Anteparo a);
#endif