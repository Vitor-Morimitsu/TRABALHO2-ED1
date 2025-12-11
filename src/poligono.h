#ifndef POLIGONO_H
#define POLIGONO_H

#include <float.h>
#include "lista.h"
#include <stdbool.h>
#include "pacote.h"
#include "vertice.h"
#include "ordenacao.h"
#include "geometria.h"
#include "arvore.h"

/*
    Arquivo .h responsável pela criação e uso do polígono de visibilidade.
*/

typedef void* Poligono;

/*
    Função responsável por criar o polígono.
*/
Poligono criarPoligono();

/*
    Função responsável por obter o boundig Box do polígono
*/
void gerarBoundingBox(Poligono p,double *minX, double *minY, double *maxX, double *maxY);

/*
    Função responsável por calcular o polígono de visibilidade a partir dos parâmetros fornecidos.
    /// @param p: polígono onde os vértices serão adicionados
    /// @param anteparos: lista de pacotes contendo linhas (anteparos)
    /// @param arvoreSegmentos: árvore BST para manter segmentos ativos ordenados por distância
    /// @param xOrigem: coordenada X da origem (bomba)
    /// @param yOrigem: coordenada Y da origem (bomba)
    /// @param comando: escolher o tipo de ordenação desejado( "q" = quicksort, "m" = mergesort, "i" = insertion sort)
*/
void calcularVisibilidade(Poligono p, Lista anteparos, double xOrigem, double yOrigem, char* comando);

/*
    Verifica se um ponto está dentro do polígono.
*/
bool pontoNoPoligono(Poligono p, double x, double y);

/*
    Função responsável por retornar as coordenadas do vértice do poligono
*/
void getVerticePoligono(Poligono p, int n,double* x, double* y);

/*
    Função responsável por adicionar um vertice na lista de vertices do polígono.
*/
void adicionarVerticePoligono(Poligono p, Vertice v);

/*
    Função responsável por retornar a quantidade de vertices do polígono
*/
int getNumeroVertices(Poligono p);

/*
    Função responsável por liberar a memória do polígono
*/
void liberarPoligono(Poligono p);

Anteparo getAnteparoPoligono(Poligono p, int indice);
#endif