#ifndef GEOMETRIA_H
#define GEOMETRIA_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pacote.h"
#include "vertice.h"
#include "lista.h"

/*
    Arquivo .h responsável por realizar todas as geometrias necessárias no segundo trabalho de estrutura de dados.
*/

/*
    Realiza o cálculo da distância entre dois pontos.
*/
double distanciaEntrePontos(double x1, double y1, double x2, double y2);

/* 
    Função responsável por realizar o cálculo do determinante.
*/
double determinante(double x1, double y1, double x2, double y2, double x3, double y3);

/*
    Função responsável por calcular o ponto de intersecção de um segmento com uma reta de origem na bomba e destino no segmento
    criará um novo vertice do polígono
*/
Vertice calculaInterseccao(Lista anteparos,Linha scaner);

#endif