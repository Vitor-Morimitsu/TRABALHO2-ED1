#include "linha.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"

typedef struct linha{
    int i;
    double x1, y1, x2, y2;
    char* cor;
}stLinha;

Linha criarLinha(int i, double x1, double y1, double x2, double y2, char* cor){
    stLinha* l = malloc(sizeof(stLinha)); 
    l->i = i;
    l->x1 = x1, l->y1 = y1, l->x2 = x2, l->y2 = y2;
    l->cor = (char*)malloc(strlen(cor) + 1);
    if(l->cor == NULL){
        printf("Erro ao alocar memória na cor da linha");
        exit(1);
    }
    strcpy(l->cor, cor);

    return((stLinha*)l);
}

int getIDLinha(Linha l){
    return((stLinha*)l)->i;
}

double getX1Linha(Linha l){
    return((stLinha*)l)->x1;
}

double getY1Linha(Linha l){
    return((stLinha*)l)->y1;
}

double getX2Linha(Linha l){
    return((stLinha*)l)->x2;
}

double getY2Linha(Linha l){
    return((stLinha*)l)->y2;
}

char* getCorLinha(Linha l){
    return((stLinha*)l)->cor;
}

double getAreaLinha(Linha l){
    //calculo do  comprimento
    stLinha* linha = (stLinha*)l;

    double difx = linha->x2 - linha->x1;
    double dify = linha->y2 - linha->y1;
    double comp = sqrt(pow(difx, 2) + pow(dify, 2));

    return 2*comp;
}

void setIDLinha(Linha l, int i){
    ((stLinha*)l)->i = i;
}

void setX1Linha(Linha l, double x1){
    ((stLinha*)l)->x1 = x1;
}

void setY1Linha(Linha l, double y1){
    ((stLinha*)l)->y1 = y1;
}

void setX2Linha(Linha l, double x2){
    ((stLinha*)l)->x2 = x2;
}

void setY2Linha(Linha l, double y2){
    ((stLinha*)l)->y2 = y2;
}

void setCorLinha(Linha l, char* cor){
    ((stLinha*)l)->cor = cor;
}

void liberaLinha(Linha l){
    if(l == NULL) return;

    stLinha* linha = (stLinha*)l;
    free(linha->cor);
    free(linha);
}