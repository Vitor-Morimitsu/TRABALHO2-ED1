#ifndef ARVORE_H
#define ARVORE_H

#include <stdio.h>
#include <stdlib.h>
#include "pacote.h"

/*
    Arquivo .h responsável pela criação e uttilização de uma árvore binária.
*/

typedef void* Arvore;
typedef stCelulaArvore* CelulaArvore;

/*
    Função responsável por criar a arvore e inicializar ela.
*/
Arvore criaArvore();

/*
    Função responsável por inserir um pacote na árvore.
*/
void insereArvore(Arvore arv, Pacote segmentoAtivo);

/* 
    Função responsável por buscar uma célula da árvore que possui a maior distancia
*/
CelulaArvore* buscaCelulaArvore(Arvore arv);

void removeArvore(Arvore arv);

#endif