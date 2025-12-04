#ifndef POLIGONO_H
#define POLIGONO_H

#include <float.h>
#include "lista.h"
#include "pacote.h"
#include "vertice.h"
#include "ordenacao.h"
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
    Função responsável por calcular o polígono de visibilidade a partir dos parâmetros fornecidos.
    /// @param ordenacao: escolher o tipo de ordenação desejado( q = quicksort, m = mergesort)
    /// @param parametro: parâmetro para utilizar o insertion sort
*/
void calcularVisibilidade(Poligono p, Lista anteparos, Lista atingidos, double xOrigem, double yOrigem, char ordenacao, int parametro);

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
#endif