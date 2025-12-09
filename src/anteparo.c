#include "anteparo.h"

typedef struct anteparo{
    int id;
    double x1,y1,x2,y2;
    char* cor;
}stAnteparo;

Anteparo criarAnteparo(int id, double x1,double y1,double x2, double y2, char* cor){
    stAnteparo* ant = (stAnteparo*)malloc(sizeof(stAnteparo));
    if(ant == NULL){
        printf("Erro em criarAnteparo\n");
        return NULL;
    }

    ant->id = id;
    ant->x1 = x1;
    ant->y1 = y1;
    ant->x2 = x2;
    ant->y2 = y2;
    ant->cor = malloc(strlen(cor) + 1);
    if(ant->cor == NULL){
        printf("Erro ao alocar a cor em criarAnteparo\n");
        free(ant);
        return NULL;
    }
    strcpy(ant->cor, cor);
    return (Anteparo)ant;
}

Anteparo circuloViraAnteparo(Pacote pac, char orientacao){
    if(pac == NULL){
        printf("Erro em circuloViraAnteparo\n");
        return NULL;
    }
    Forma form = getFormaPacote(pac);
    char tipo = getTipoPacote(pac);
    if(tipo != 'c'){
        printf("Não é um círculo \n");
        return NULL;
    }
    double x = getCoordXCirculo((Circulo)form);
    double y = getCoordYCirculo((Circulo)form);
    double raio = getRaioCirculo((Circulo)form);
    int id = getIDCirculo((Circulo)form);
    char* cor = getCorBCirculo((Circulo)form);

    stAnteparo* ant = NULL;
    if(orientacao == 'h') {
        ant = criarAnteparo(id, x - raio, y, x + raio, y, cor);
    }else if(orientacao == 'v'){
        ant = criarAnteparo(id, x, y - raio, x, y + raio, cor);
    }

    return ant;
}

Anteparo* retanguloViraAnteparo(Pacote pac, int maiorID){
    if(pac == NULL){
        printf("Erro em retanguloViraAnteparo\n");
        return NULL;
    }
    Forma form = getFormaPacote(pac);
    char tipo = getTipoPacote(pac);
    if(tipo != 'r'){
        printf("Não é um retangulo\n");
        return NULL;
    }
    Retangulo ret = (Retangulo)form;
    int id = getIDRetangulo(ret);
    double x = getCoordXRetangulo(ret);
    double y = getCoordYRetangulo(ret);
    double w = getWRetangulo(ret);
    double h = getHRetangulo(ret);
    char* cor = getCorBRetangulo(ret);

    Anteparo *array = malloc(4*sizeof(Anteparo));
    array[0] = criarAnteparo(maiorID + 1, x, y,x+w,y,cor);
    array[1] = criarAnteparo(maiorID + 2, x,y,x,y+h,cor);
    array[2] = criarAnteparo(maiorID + 3, x+w, y,x+w, y+h,cor);
    array[3] = criarAnteparo(maiorID, x,y+h,x+w,y+h,cor);

    return array;
}

Anteparo linhaViraAnteparo(Pacote pac){
    if(pac == NULL){
        printf("Erro em linhaViraAnteparo\n");
        return NULL;
    }
    Forma form = getFormaPacote(pac);
    char tipo = getTipoPacote(pac);
    if(tipo != 'l'){
        printf("Não é uma linha\n");
        return NULL;
    }
    Linha l = (Linha)form;

    stAnteparo* ant = criarAnteparo(getIDLinha(l),getX1Linha(l),getY1Linha(l),getX2Linha(l),getY2Linha(l),getCorLinha(l));
    return ant;
}

Anteparo textoViraAnteparo(Pacote pac){
    if(pac == NULL){
        printf("Erro em textoViraAnteparo\n");
        return NULL;
    }
    char tipo = getTipoPacote(pac);
    if(tipo != 't'){
        printf("Não é um texto\n");
        return NULL;
    }
    Forma form = getFormaPacote(pac);
    Texto t = (Texto)form;
    char ancora = getATexto(t);
    double x = getCoordXTexto(t);
    double y = getCoordYTexto(t);
    double x1,y1,x2,y2;
    if(ancora == 'i'){
        x1 = x;
        y1 = y;
        x2 = x + 10*strlen(getTxtoTexto(t));
        y2 = y;
    }else if(ancora == 'f'){
        x1 = x - 10*strlen(getTxtoTexto(t));
        y1 = y;
        x2 = x;
        y2 = y;
    }else if(ancora == 'm'){
        x1 = x - 10*strlen(getTxtoTexto(t))/2;
        y1 = y;
        x2 = x + 10*strlen(getTxtoTexto(t))/2;
        y2 = y;
    }

    stAnteparo* ant = criarAnteparo(getIDTexto(t),x1,y1,x2,y2,getCorBTexto(t));

    return ant;
}

int getIDAnteparo(Anteparo a){
    if(a == NULL){
        printf("Erro em getIDAnteparo\n");
        return -1;
    }
    stAnteparo* anteparo = (stAnteparo*)a;
    return anteparo->id;
}

double getX1Anteparo(Anteparo a){
    if(a == NULL){
        printf("Erro em getX1Anteparo\n");
        return -1;
    }
    stAnteparo* anteparo = (stAnteparo*)a;
    return anteparo->x1;
}

double getY1Anteparo(Anteparo a){
    if(a == NULL){
        printf("Erro em getY1Anteparo\n");
        return -1;
    }
    stAnteparo* anteparo = (stAnteparo*)a;
    return anteparo->y1;
}

double getX2Anteparo(Anteparo a){
    if(a == NULL){
        printf("Erro em getX2Anteparo\n");
        return -1;
    }
    stAnteparo* anteparo = (stAnteparo*)a;
    return anteparo->x2;
}

double getY2Anteparo(Anteparo a){
    if(a == NULL){
        printf("Erro em getY2Anteparo\n");
        return -1;
    }
    stAnteparo* anteparo = (stAnteparo*)a;
    return anteparo->y2;
}

char* getCorAnteparo(Anteparo a){
    if(a == NULL){
        printf("Erro em getCorAnteparo\n");
        return NULL;
    }
    stAnteparo* anteparo = (stAnteparo*)a;
    return anteparo->cor;
}

void setCorAnteparo(Anteparo a, char* cor){
    if(a == NULL || cor == NULL){
        printf("Erro em setCorAnteparo\n");
        return;
    }
    stAnteparo* anteparo = (stAnteparo*)a;

    if(anteparo->cor!= NULL){
        free(anteparo->cor);
    }

    anteparo->cor = malloc(strlen(cor) + 1);
    strcpy(anteparo->cor, cor);
}

void liberarAnteparo(Anteparo a){
    if(a == NULL){
        printf("Erro em liberarAnteparo\n");
        return;
    }

    stAnteparo* anteparo = (stAnteparo*)a;
    free(anteparo->cor);
    free(anteparo);
}