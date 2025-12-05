#ifndef LISTA_H
#define LISTA_H

/*
    Arquivo .h destinado à criação e utilização de uma lista ligada.
*/

#include <stdio.h>
#include <stdlib.h>
#include "pacote.h"

typedef void* Lista;
typedef void* CelulaLista;

/*
    Cria uma lista e a retorna.
*/
Lista criarLista();

/*
    Insere um pacote na lista.
*/
void insereLista(Lista l, void* pac);

/*
    Remove um pacote da lista e o retorna.
*/
CelulaLista removeLista(Lista l, int id);

/*
    Retorna o tamanho da lista.
*/
int getTamanhoLista(Lista l);

/*
    Retorna o pacote contido em uma célula.
*/
Pacote getConteudoCelula(CelulaLista celula);

/*
    Retorna a primeira célula da lista.
*/
CelulaLista getPrimeiraCelulaLista(Lista l);

/*
    Retorna a próxima célula da lista.
*/
CelulaLista getProximaCelulaLista(CelulaLista celula);

/*
    Procura e retorna um pacote contido na lista sem apagá-lo da lista.
*/
Pacote procuraPacoteLista(Lista l, int id);

/*
    Retorna o maior id da lista.
*/
int getMaiorIdLista(Lista l);

/*
    Retorna o maior valor de x ou de y presente na lista dependendo do comando passado.
    1 retorna o maior x. 
    2 retorna o maior y.
*/
double getMaiorValorLista(Lista lista, int opcao);

/*
    Retorna o menor valor de x ou y presente na lista dependendo da opção selecionada.
    1 retorna o menor x.
    2 retorna o menor y.
*/
double getMenorValorLista(Lista lista, int opcao);

/*
    Libera a memória de uma lista.
*/
void liberaLista(Lista l);

#endif