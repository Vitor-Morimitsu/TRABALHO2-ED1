#include "geometria.h"

#define pi 3.141592653589793
#define epsilon 1e-9

double distanciaEntrePontos(double x1, double y1, double x2, double y2){
    double dx = x2 - x1;
    double dy = y2 - y1;
    return sqrt(dx*dx + dy*dy);
}

double determinante(double x1, double y1, double x2, double y2, double x3, double y3){
    double det = x1*y2 + y1*x3 + x2*y3 - (y2*x3 + x1*y3 + y1*x2);
    return det;
}

bool dentroBoundingBox(double x, double y, double minX, double minY, double maxX, double maxY){
    return (x >= minX && x <= maxX && y >= minY && y <= maxY);
}

Vertice calculaInterseccao(double bx, double by, double angulo,Anteparo ant){
    if(ant == NULL){
        printf("Anteparo não encontrado em calculaInterseccao\n");
        return NULL;
    }
    double x1 = getX1Anteparo(ant);
    double y1 = getY1Anteparo(ant);
    double x2 = getX2Anteparo(ant);
    double y2 = getY2Anteparo(ant);

    double dx_raio = cos(angulo);
    double dy_raio = sin(angulo);

    double dx_seg = x2-x1;
    double dy_seg = y2-y1;

    double dx_dif = x1 - bx;
    double dy_dif = y1 - by;

    double det = dx_raio * dy_seg - dy_raio*dx_seg;
    if(fabs(det) < 1e-10){
        return NULL;
    }
    double t = (dx_dif * dy_seg - dy_dif * dx_seg) / det;
    double u = (dx_dif * dy_raio - dy_dif * dx_raio) / det;
    if (t >= -epsilon && u >= -epsilon && u <= 1.0 + epsilon){
        Vertice v = criarVertice();
        setXVertice(v, bx + (dx_raio * t));
        setYVertice(v, by + (dy_raio * t));
        setDistanciaVertice(v, (t < 0 ? 0 : t)); 
        setAnteparoVertice(v,ant);
        return v;
    }
    return NULL;
}

bool pontoEmPoligono(double px, double py, Lista verticesPoligono){
    if(verticesPoligono == NULL){
        printf("Erro em pontoEmPoligono\n");
        return false;
    }

    CelulaLista atual = getPrimeiraCelulaLista(verticesPoligono);
    if(atual == NULL){
        printf("Erro ao achar a celula atual em pontoEmPoligono em geometria.c\n");
        return;
    }
}

bool segmentoIntersectaSegmento(double x1, double y1, double x2, double y2,double x3, double y3, double x4, double y4);

double distanciaPontoSegmento(double px, double py,double x1, double y1, double x2, double y2);