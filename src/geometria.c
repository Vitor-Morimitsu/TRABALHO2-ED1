#include "geometria.h"

#define pi 3.141592653589793
#define epsilon 0.00001

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

    CelulaLista cel = getPrimeiraCelulaLista(verticesPoligono);
    if(cel == NULL){
        printf("Erro ao achar a celula atual em pontoEmPoligono em geometria.c\n");
        return false;
    }

    int numVertices = getTamanhoLista(verticesPoligono);

    Vertice* vertices = malloc(numVertices*sizeof(Vertice));
    if(vertices == NULL){
        printf("Erro ao alocar memória em pontoEmPoligono\n");
        return false;
    }

    int i = 0;
    while(cel!= NULL){
        vertices[i++] = (Vertice)getConteudoCelula(cel);
        cel = getProximaCelulaLista(cel);
    }

    int interseccoes = 0;
    for(int i = 0;i<numVertices; i++){
        //vertice i p vertice j
        int j = (i+1)%numVertices;
        double x1 = getXVertice(vertices[i]);
        double y1 = getYVertice(vertices[i]);
        double x2 = getXVertice(vertices[j]);
        double y2 = getYVertice(vertices[j]);

        if(((y1 > py) != (y2 > py))) { // Aresta cruza linha horizontal
            // Calcula a coordenada X do cruzamento
            double xCruzamento = (x2 - x1) * (py - y1) / (y2 - y1) + x1;
            
            if(px < xCruzamento) { // Cruzamento à direita do ponto
                interseccoes++;
            }
        }
    }
    free(vertices);

    //num impar de inteersecções significa que o ponto está dentro
    return (interseccoes % 2) == 1;
    
}

bool segmentoIntersectaSegmento(double x1, double y1, double x2, double y2,double x3, double y3, double x4, double y4) {
    // Vetores direção
    double dx1 = x2 - x1;
    double dy1 = y2 - y1;
    double dx2 = x4 - x3;
    double dy2 = y4 - y3;
    
    // Determinante 
    double det = dx1 * dy2 - dy1 * dx2;
    
    // Se det = 0, segmentos são paralelos (ou colineares)
    if(fabs(det) < 1e-10) {
        // Verificar se são colineares E se sobrepõem
        // (caso especial - simplificado aqui)
        return false;
    }
    
    // Vetor da origem do segmento 1 ao segmento 2
    double dx3 = x3 - x1;
    double dy3 = y3 - y1;
    
    // Parâmetros da interseção
    // Segmento 1: P = (x1,y1) + t * (dx1,dy1), onde 0 ≤ t ≤ 1
    // Segmento 2: Q = (x3,y3) + u * (dx2,dy2), onde 0 ≤ u ≤ 1
    
    double t = (dx3 * dy2 - dy3 * dx2) / det;
    double u = (dx3 * dy1 - dy3 * dx1) / det;
    
    return (t >= -epsilon && t <= 1.0 + epsilon && u >= -epsilon && u <= 1.0 + epsilon);
}

double distanciaPontoSegmento(double px, double py,double x1, double y1, double x2, double y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    
    double compAoQuadrado = dx*dx + dy*dy;
    
    if(compAoQuadrado < 1e-10) {
        return distanciaEntrePontos(px, py, x1, y1);
    }
    
    double dpx = px - x1;
    double dpy = py - y1;
    
    double t = (dpx * dx + dpy * dy) / compAoQuadrado;
    
    
    if(t < 0.0) {
        t = 0.0; //projeção antes do início
    } else if(t > 1.0) {
        t = 1.0;//projeção depois do fim
    }
    
    double closestX = x1 + t * dx;
    double closestY = y1 + t * dy;
    
    return distanciaEntrePontos(px, py, closestX, closestY);
}