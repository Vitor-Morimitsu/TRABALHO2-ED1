#ifndef ARVORE_H
#define ARVORE_H

#include <stdio.h>
#include <stdlib.h>
#include "pacote.h"
#include "vertice.h"

/*
    Arquivo .h responsável pela criação e uttilização de uma árvore binária.
*/

typedef void* Arvore;
typedef void* celulaArvore;

/*
    Função responsável por criar a arvore e inicializar ela.
*/
Arvore criarArvore();

/*
    Função responsável por inserir um pacote na árvore.
*/
void insereArvore(Arvore arv, Anteparo an, double bx, double by, double angulo,double distCelAnterior);

/*
    Função responsável por remover um conteudo da arvore pelo ID, necessitando dos dados geométricos para navegação.
*/
void removerArvore(Arvore arv, int id);

/*
    Função responsável por liberar toda a memória da árvore
*/
void liberarArvore(Arvore arv);

/*
    Encontra o menor valor presente na árvore completa. Retorna a célula com o menor valor ou NULL se a árvore estiver vazia.
*/
celulaArvore encontrarMinimoArvore(Arvore arv);

/*
    Encontra o menor valor presente no lado esquerdo da árvore.
*/
celulaArvore encontrarMinimo(celulaArvore cel);

/*
    Retorna o anteparo armazenado em uma célula da árvore.
*/
Anteparo getAnteparoCelula(celulaArvore cel);


#endif