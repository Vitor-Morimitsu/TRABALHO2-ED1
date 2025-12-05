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
