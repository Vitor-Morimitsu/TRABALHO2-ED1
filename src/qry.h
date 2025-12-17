#ifndef QRY_H
#define QRY_H

#include <stdio.h>
#include <stdlib.h>
#include "lista.h"
#include "arvore.h"
#include "pacote.h"
#include <string.h>
#include "svg.h"
#include "geo.h"
#include "colisao.h"

/*
    Arquivo .h responsável pelo processamento das instruções contidas no .qry
*/

/*
    Função responsável por criar a arena retangular que englobará todas as formas e anteparos.
*/
void gerarArena(FILE* svgEntrada, Lista pacotes,Lista anteparos);

/*
    Função responsável por processar o comando A descrito no trabalho.
*/
void comandoA(FILE* arqTxt,Lista pacotes,Lista anteparos,int inicio, int fim, char letra);

/*
    Função responsável por processar o comando D descrito no trabalho.
*/
/*
    Função responsável por processar o comando D descrito no trabalho.
*/
void comandoD(FILE* arqTxt, char* sufixo, FILE* svgPrincipal, Lista anteparos, Lista formas, double xBomba, double yBomba, char tipoOrdenacao, int limiteInsertionSort, double minX, double minY, double width, double height);

/*
    Função responsável por processar o comando P descrito no trabalho.
*/
void comandoP(FILE* txt, char* sufixo, FILE* svgPrincipal, Lista formas, Lista anteparos, double x, double y, char* cor, char tipoOrdenacao, int limiteInsertionSort, double minX, double minY, double width, double height);

/*
    Função responsável por processar o comando CLN descrito no trabalho.
*/
void comandoCln(FILE* txt, char* sufixo, FILE* svgPrincipal, Lista formas, Lista anteparos, double x, double y, double dx, double dy, char tipoOrdenacao, int limiteInsertionSort, double minX, double minY, double width, double height);


/*
    Função principal responsável por ler as intruções contidas no arquivo qry.
*/
void lerQry(FILE* qry,FILE* txt, FILE* svg, Lista formas, char tipoOrdenacao,int limiteInsertionSort);

#endif