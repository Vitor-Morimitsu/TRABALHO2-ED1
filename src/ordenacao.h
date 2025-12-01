#ifndef ORDENACAO_H
#define ORDENACAO_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lista.h"
#include "arvore.h"

/*
    Arquivo .h responsável pela criação e uso de algoritmos de ordenação.
*/

/*
    Realiza o cálculo da distância entre dois pontos.
*/
double distanciaEntrePontos(double x1, double y1, double x2, double y2);

/*
    Retorna uma lista ordenada com o algoritmo mergeSort.Útil para subvetores maiores que 10 elementos.
*/
Lista mergeSort(Lista desordenada);

/*
    Retorna uma lista ordenada com o algoritmo insertion sort.
*/
Lista insertionSort(Lista desordenada,double xOrigem, double yOrigem, Arvore arv);
//estou ordenando os anteparos? todo anteparo é uma linha


#endif