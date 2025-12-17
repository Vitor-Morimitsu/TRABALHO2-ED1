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
typedef struct stNo No;

/*
    Converte os dados da lista em um array.
*/
No* gerarArray(Lista vertices,int tamanho);

/// @brief Retorna o vértice de um nó
/// @param n no
/// @return No
Vertice getVerticeNo(No* n);

/* Recupera o vértice na posição 'indice' do array genérico.
   Resolve o problema de aritmética de ponteiros em outros arquivos.
*/
Vertice getVerticeDoArray(No* array, int indice);

/* Recupera o ângulo na posição 'indice' do array genérico.
*/
double getAnguloDoArray(No* array, int indice);

/*
    Função responsável por liberar a memória do array.
*/
void liberarArray(No* array);

/*
    Retorna uma lista ordenada com o algoritmo mergeSort.
*/
void mergeSort(No* array, int tamanho, int limiteInsertionSort);

/*
    Retorna uma lista ordenada com o algoritmo insertion sort.
*/
void insertionSort(No* array,int tamanho);

/*
    Retorna uma lista ordenada com o algoritmo quick sort.
*/
void quickSort(No* array, int low, int high);

#endif