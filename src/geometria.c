#include "geometria.h"

#define pi 3.141592653589793
#define epsilon 1e-9

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

// Vertice encontraInterseccaoMaisProxima(Lista segmentosAtivos, double xOrigem, double yOrigem, double angulo) {
//     if (segmentosAtivos == NULL) {
//         return NULL;
//     }

//     Vertice vMaisProximo = NULL;
//     double menorDistancia = INFINITY;

//     // Cria um raio longo na direção do ângulo para garantir que ele cruze os segmentos
//     double far_dist = 20000.0; // Distância "infinita"
//     double xRaioFim = xOrigem + far_dist * cos(angulo);
//     double yRaioFim = yOrigem + far_dist * sin(angulo);

//     // Itera por todos os segmentos ativos na lista
//     for (CelulaLista celula = getPrimeiraCelulaLista(segmentosAtivos); celula != NULL; celula = getProximaCelulaLista(celula)) {
//         Linha segmento = (Linha)getConteudoCelula(celula);

//         double x3 = getX1Linha(segmento);
//         double y3 = getY1Linha(segmento);
//         double x4 = getX2Linha(segmento);
//         double y4 = getY2Linha(segmento);

//         // Fórmula de interseção de dois segmentos de reta
//         double denom = (xOrigem - xRaioFim) * (y3 - y4) - (yOrigem - yRaioFim) * (x3 - x4);

//         if (fabs(denom) > 1e-9) { // Evita divisão por zero (retas paralelas)
//             double t = ((xOrigem - x3) * (y3 - y4) - (yOrigem - y3) * (x3 - x4)) / denom;
//             double u = -((xOrigem - xRaioFim) * (yOrigem - y3) - (yOrigem - yRaioFim) * (xOrigem - x3)) / denom;

//             // Verifica se a interseção ocorre dentro de ambos os segmentos
//             // t > 0 para garantir que a interseção esteja à frente do raio
//             // u entre 0 e 1 para garantir que esteja no segmento de anteparo
//             if (t > 1e-9 && u >= 0 && u <= 1) {
//                 double xIntersec = xOrigem + t * (xRaioFim - xOrigem);
//                 double yIntersec = yOrigem + t * (yRaioFim - yOrigem);
//                 double dist = distanciaEntrePontos(xOrigem, yOrigem, xIntersec, yIntersec);

//                 if (dist < menorDistancia) {
//                     menorDistancia = dist;
//                     if (vMaisProximo == NULL) {
//                         vMaisProximo = criarVertice();
//                     }
//                     setXVertice(vMaisProximo, xIntersec);
//                     setYVertice(vMaisProximo, yIntersec);
//                     setAnguloVertice(vMaisProximo, xOrigem, yOrigem);
//                     setDistanciaVertice(vMaisProximo, dist);
//                     setAnteparoVertice(vMaisProximo,(Anteparo)segmento);
//                 }
//             }
//         }
//     }

//     return vMaisProximo;
// }
