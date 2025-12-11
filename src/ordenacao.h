#ifndef ORDENACAO_H
#define ORDENACAO_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lista.h"
#include "arvore.h"
#include "anteparo.h"
#include "vertice.h"

/*
    Arquivo .h responsável pela criação e uso de algoritmos de ordenação.
*/

typedef struct stNo {
    Vertice ver;
    double angulo;
} stNo;

/*
    Converte os dados da lista em um array.
*/
stNo* gerarArray(Lista vertices,int tamanho, double xOrigem, double yOrigem);

/*
    Função responsável por liberar a memória do array.
*/
void liberarArray(stNo* array);

/*
    Retorna uma lista ordenada com o algoritmo mergeSort.
*/
void mergeSort(stNo* array,int tamanho);

/*
    Retorna uma lista ordenada com o algoritmo insertion sort.
*/
void insertionSort(stNo* array,int tamanho);

/*
    Retorna uma lista ordenada com o algoritmo quick sort.
*/
void quickSort(stNo* array, int low, int high);

#endif