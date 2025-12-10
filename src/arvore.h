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
typedef void* CelulaArvore;

/*
    Função responsável por criar a arvore e inicializar ela.
*/
Arvore criaArvore();

/*
    Função responsável por inserir um pacote na árvore.
*/
void insereArvore(Arvore arv, void* segmentoAtivo);

/* 
    Função responsável por buscar uma célula da árvore que possui o menor angulo
*/
CelulaArvore buscaCelulaArvore(Arvore arv);

/*
    Função responsável por remover um conteudo da arvore pelo ID
*/
void removeArvore(Arvore arv, int id);

/*
    Função responsável por definir a origem e ângulo atual para cálculo de distâncias
*/
void setOrigemArvore(Arvore arv, double xOrigem, double yOrigem, double anguloAtual);

/*
    Função responsável por liberar toda a memória da árvore
*/
void liberarArvore(Arvore arv);

#endif