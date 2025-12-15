#ifndef GEOMETRIA_H
#define GEOMETRIA_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pacote.h"
#include "vertice.h"
#include "lista.h"
#include "arvore.h"

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
    Função responsável por ver se um ponto está dentro do bounding Box do polígono.
*/
bool dentroBoundingBox(double x, double y, double minX, double minY, double maxX, double maxY);

/*
    Função responsável por calcular o ponto de intercecção em um anteparo a partir da bomba
*/
Vertice calculaInterseccao(double bx, double by, double angulo,Anteparo ant);

/*
    Função responsável por identificar se um ponto está no polígono.
*/
bool pontoEmPoligono(double px, double py, Lista verticesPoligono);

/*
    Verifica se segmento intersecta segmento. Lança raios para a direita e conta quantas arestas do poligono ele cruza.
    Ímpar: ponto está fora do poligono
    Par: ponto está dentro do polígono
*/
bool segmentoIntersectaSegmento(double x1, double y1, double x2, double y2,double x3, double y3, double x4, double y4);

/*
    Função responsável por calcular a distancia de um ponto até o segmento
*/
double distanciaPontoSegmento(double px, double py,double x1, double y1, double x2, double y2);
#endif