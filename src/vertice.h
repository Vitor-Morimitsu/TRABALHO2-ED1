#ifndef VERTICE_H
#define VERTICE_H

#include <stdio.h>
#include <stdlib.h>
#include "ordenacao.h"
#include "lista.h"
#include "anteparo.h"

typedef void* Vertice;

/*
    Função responsável por criar e retornar um vértice.
*/
Vertice criarVertice();

/*
    Retorna a coordenada X do vertice
*/
double getXVertice(Vertice v);

/*
    Retorna a coordenada Y do vertice
*/
double getYVertice(Vertice v);

/*
    Retorna o angulo do vertice em relação à bomba.
*/
double getAnguloVertice(Vertice v);

/*
    Retorna a distancia do vertice em relação bomba.
*/
double getDistanciaVertice(Vertice v);

/*
    Retorna o tipo do vértice. 
    'n' não definido.
    'i' início
    'f' fim
*/
char getTipoVertice(Vertice v);

/*
    Retorna o anteparo do vertice
*/
Anteparo getAnteparoVertice(Vertice v);

/*
    Define a coordenada X do vértice
*/
void setXVertice(Vertice v, double x);

/*
    Define a coordenada Y do vertice
*/
void setYVertice(Vertice v, double y);

/*
    Define o angulo do vertice
*/
void setAnguloVertice(Vertice v, double xBomba, double yBomba);

/*
    Define a distancia do vertice até a bomba
*/
void setDistanciaVertice(Vertice v, double distancia);

/*
    Define o tipo do vértice
    'n' não definido.
    'i' início
    'f' fim
*/
void setTipoVertice(Vertice v, char tipo);

/*
    Define o anteparo do vertice
*/
void setAnteparoVertice(Vertice v, Anteparo a);

#endif