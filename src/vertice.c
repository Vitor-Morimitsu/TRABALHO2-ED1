#include "vertice.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct vertice{
    double x, y;
    double angulo;
    double distancia;
    char tipo;
    Anteparo a;
}stVertice;

Vertice criarVertice(){
    stVertice* vertice = (stVertice*)malloc(sizeof(stVertice));
    if(vertice == NULL){
        printf("Erro em criarVertice\n");
        return NULL;
    }
    vertice->x = 0.0;
    vertice->y = 0.0;
    vertice->angulo = 0.0;
    vertice->distancia = 0.0;
    vertice->tipo = 'n';
    vertice->a = NULL;

    return (Vertice)vertice;
}

double getXVertice(Vertice v){
    stVertice* vertice = (stVertice*)v;
    return vertice->x;
}

double getYVertice(Vertice v){
    stVertice* vertice = (stVertice*)v;
    return vertice->y;
}

double getAnguloVertice(Vertice v){
    stVertice* vertice = (stVertice*)v;
    return vertice->angulo;
}

double getDistanciaVertice(Vertice v){
    stVertice* vertice = (stVertice*)v;
    return vertice->distancia;
}

char getTipoVertice(Vertice v){
    stVertice* ver = (stVertice*)v;
    return ver->tipo;
}

Anteparo getAnteparoVertice(Vertice v){
    stVertice* vertice = (stVertice*)v;
    return vertice->a;
}

void setXVertice(Vertice v, double x){
    stVertice* vertice = (stVertice*)v;
    vertice->x = x;
}

void setYVertice(Vertice v, double y){
    stVertice* ver = (stVertice*)v;
    ver->y = y;
}

void setAnguloVertice(Vertice v, double xBomba, double yBomba){
    stVertice* ver = (stVertice*)v;
    double dx = getXVertice(v) - xBomba;
    double dy = getYVertice(v) - yBomba;

    ver->angulo = atan2(dy,dx);
}

void setDistanciaVertice(Vertice v, double distancia){
    stVertice* ver = (stVertice*)v;
    ver->distancia = distancia;
}

void setTipoVertice(Vertice v, char tipo){
    stVertice* ver = (stVertice*)v;
    ver->tipo = tipo;
}

void setAnteparoVertice(Vertice v, Anteparo a){
    stVertice* ver = (stVertice*)v;
    ver->a = a;
}

void destroiVertice(Vertice v){
    stVertice* vertice = (stVertice*)v;
    if(vertice != NULL){
        free(vertice);
    }
}