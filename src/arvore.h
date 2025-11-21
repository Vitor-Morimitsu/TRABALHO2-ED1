#ifndef ARVORE_H
#define ARVORE_H

#include <stdio.h>
#include <stdlib.h>
#include "pacote.h"

/*
    Arquivo .h responsável pela criação e uttilização de uma árvore binária.
*/

typedef void* Arvore;

/*
    Função responsável por criar a arvore e inicializar ela.
*/
Arvore criaArvore();

/*
    Função responsável por inserir um pacote na árvore.
*/
void insereArvore(Arvore arv, void* segmentoAtivo);

void buscaCelulaArvore(Arvore arv, int id);

void removeArvore(Arvore arv);

#endif