#ifndef LISTA_H
#define LISTA_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pacote.h"

typedef void* Lista;
typedef void* CelulaLista;
typedef void* Forma;
typedef void* No;

Lista criarLista();

/*
Insere um pacote no final da lista.
*/
void insereLista(Lista l, void* conteudo);

/*
Retorna o tamanho da lista.
*/
int getTamanhoLista(Lista l);

/*
Retorna o conteudo de uma celula.
*/
void* getConteudoCelula(CelulaLista celula);

/*
Retorna a primeira celula da lista.
*/
CelulaLista getPrimeiraCelulaLista(Lista l);

/*
Retorna a proxima celula da lista.
*/
CelulaLista getProximaCelulaLista(CelulaLista celula);

/*
Retorna a ultima celula da lista.
*/
CelulaLista getUltimaCelulaLista(Lista l);

/*
Retorna a ultima celula da lista.
*/
CelulaLista getUltimaCelulaLista(Lista l);


/*
Encontra um pacote com o id especificado e o retorna.
*/
Pacote procuraPacoteLista(Lista l, int id);


/*
Remove um pacote da lista com o id fornecido
*/
void removeLista(Lista l, int id);

/*
Retorna o maior id da lista de formas
*/
int getMaiorIdLista(Lista l);

/*
    Retorna o maior valor de uma coordenada específica da lista.
    Opcao 1 para X, 2 para Y.
*/
double getMaiorValorLista(Lista lista, int opcao);

/*
    Retorna o menor valor de uma coordenada específica da lista.
    Opcao 1 para X, 2 para Y.
*/
double getMenorValorLista(Lista lista, int opcao);

/*
Libera a memória da lista e de todos os pacotes.
*/
void liberaLista(Lista l);

/*
Libera apenas a memória da estrutura da lista (células), sem liberar o conteúdo.
*/
void liberaEstruturaLista(Lista l);

/*
Remove um item da lista buscando pelo endereço do conteúdo.
*/
void removeDaListaPorConteudo(Lista l, void* conteudo);

/*
Remove um item da lista com o id fornecido e retorna o conteúdo (sem liberar).
*/
void* removeRetornaConteudo(Lista l, int id);

#endif