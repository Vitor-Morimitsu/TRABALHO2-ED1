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

typedef void* No;

/*
    Converte os dados da lista em um array.
*/
No* gerarArray(Lista vertices,int tamanho, double xOrigem, double yOrigem);

/*
    Função responsável por liberar a memória do array.
*/
void liberarArray(No array);

/*
    Retorna uma lista ordenada com o algoritmo mergeSort.
*/
void mergeSort(No* array,int tamanho);

/*
    Retorna uma lista ordenada com o algoritmo insertion sort.
*/
void insertionSort(No* array,int tamanho);

#endif