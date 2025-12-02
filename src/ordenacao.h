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
typedef void* No;

/*
    Converte os dados da lista em um array.
*/
No* gerarArray(Lista lista, double xOrigem, double yOrigem);

/*
    Realiza o cálculo da distância entre dois pontos.
*/
double distanciaEntrePontos(double x1, double y1, double x2, double y2);

/*
    Retorna uma lista ordenada com o algoritmo mergeSort.Útil para subvetores maiores que 10 elementos.
*/
void mergeSort(Lista desordenada);

/*
    Retorna uma lista ordenada com o algoritmo insertion sort.
*/
Lista insertionSort(Lista desordenada,double xOrigem, double yOrigem, Arvore arv);
//estou ordenando os anteparos? todo anteparo é uma linha

/* 
    Função responsável por realizar o cálculo do determinante.
*/
double determinante(double x1, double y1, double x2, double y2, double x3, double y3);

#endif