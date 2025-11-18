#ifndef LISTA_H
#define LISTA_H

/*
    Arquivo .h destinado à criação e utilização de uma lista ligada.
*/

#include <stdio.h>
#include <stdlib.h>
#include "pacote.h"

typedef void* Lista;

/*
    Cria uma lista e a retorna.
*/
Lista criarLista();

/*
    Insere um pacote na lista.
*/
void insereLista(Lista l, Pacote pac);

/*
    Remove um pacote da lista e o retorna.
*/
Pacote removeLista(Lista l,int id); //chamar procura pacote

/*
    Retorna o tamanho da lista.
*/
int getTamanhoLista(Lista l);

Pacote procuraPacoteLista(Lista l,int id);

#endif