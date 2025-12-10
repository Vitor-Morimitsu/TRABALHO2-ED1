#include "geometria.h"

double distanciaEntrePontos(double x1, double y1, double x2, double y2){
    double horizontal = fabs(x2-x1);
    double potenciaX = pow(horizontal, 2);

    double vertical = fabs(y2-y1);
    double potenciaY = pow(vertical,2);

    double distancia = sqrt(potenciaX + potenciaY);

    return distancia;
}

double determinante(double x1, double y1, double x2, double y2, double x3, double y3){
    double det = x1*y2 + y1*x3 + x2*y3 - (y2*x3 + x1*y3 + y1*x2);
    return det;
}

bool dentroBoundingBox(double x, double y, double minX, double minY, double maxX, double maxY){
    return (x >= minX && x <= maxX && y >= minY && y <= maxY);
}

Vertice calculaInterseccao(Lista anteparos, Linha scaner) {
    if(anteparos == NULL) {
        printf("Erro em calculaInterseccao\n");
        return NULL;
    }
    
    double x1 = getX1Linha(scaner);
    double y1 = getY1Linha(scaner);
    double x2 = getX2Linha(scaner);
    double y2 = getY2Linha(scaner);

    CelulaLista celula = getPrimeiraCelulaLista(anteparos);
    
    while(celula != NULL) {
        Anteparo antCelula = getConteudoCelula(celula);
        double x3 = getX1Anteparo(antCelula);
        double y3 = getY1Anteparo(antCelula);
        double x4 = getX2Anteparo(antCelula);
        double y4 = getY2Anteparo(antCelula);

        double denom = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4);
        
        if(fabs(denom) > 1e-10) {
            double t = ((x1-x3)*(y3-y4) - (y1-y3)*(x3-x4)) / denom;
            double u = -((x1-x2)*(y1-y3) - (y1-y2)*(x1-x3)) / denom;
            
            if(t >= 0 && t <= 1 && u >= 0 && u <= 1) {
                double xIntersec = x1 + t * (x2 - x1);
                double yIntersec = y1 + t * (y2 - y1);
                
                Vertice v = criarVertice();
                setXVertice(v, xIntersec);
                setYVertice(v, yIntersec);
                return v;
            }
        }
        
        celula = getProximaCelulaLista(celula);
    }
    
    return NULL; //nenhuma interseção encontrada
}

Vertice encontraInterseccaoMaisProxima(Arvore segmentosAtivos, Linha scaner) {
    if (segmentosAtivos == NULL || scaner == NULL) {
        return NULL;
    }

    // Busca o segmento mais próximo na árvore (mais à esquerda)
    Linha linhaProxima = (Linha)buscaCelulaArvore(segmentosAtivos);
    
    if (linhaProxima == NULL) {
        return NULL;
    }

    double x1 = getX1Linha(scaner);
    double y1 = getY1Linha(scaner);
    double x2 = getX2Linha(scaner);
    double y2 = getY2Linha(scaner);
    
    double x3 = getX1Linha(linhaProxima);
    double y3 = getY1Linha(linhaProxima);
    double x4 = getX2Linha(linhaProxima);
    double y4 = getY2Linha(linhaProxima);

    double denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

    if (fabs(denom) > 1e-9) {
        double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denom;
        double u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / denom;

        if (t > 0 && t <= 1 && u >= 0 && u <= 1) {
            double xIntersec = x1 + t * (x2 - x1);
            double yIntersec = y1 + t * (y2 - y1);
            
            double dist = distanciaEntrePontos(x1, y1, xIntersec, yIntersec);
            
            if (dist > 1e-6) {
                Vertice v = criarVertice();
                setXVertice(v, xIntersec);
                setYVertice(v, yIntersec);
                setAnguloVertice(v, x1, y1);
                setDistanciaVertice(v, dist);
                return v;
            }
        }
    }

    return NULL;
}
