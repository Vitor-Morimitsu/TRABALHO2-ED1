#ifndef LINHA_H
#define LINHA_H

#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"

/*
    Este arquivo se refere à forma de uma linha e suas características e propriedades úteis ao primeiro trabalho de estrutura de dados I.
*/

typedef void* Linha;

/*
    Arquivo .h para criação da linha com suas respectivas operações get, set e cálculo de área.
*/

/// @brief Cria e retorna uma linha 
/// @param i Identificador da linha
/// @param x1 Coordenada X do primeiro ponto
/// @param y1 Coordenada Y do primeiro ponto
/// @param x2 Coordenada X do segundo ponto 
/// @param y2 Coordenada Y do segundo ponto
/// @param cor Cor da linha
Linha criarLinha(int i, double x1, double y1, double x2, double y2, char* cor);

/// @brief Retorna o id da linha
/// @param l Linha
/// @return Identificador da linha
int getIDLinha(Linha l);

/// @brief Retorna coordenada X do primeiro ponto
/// @param l Linha
/// @return Cooordenada X do primeiro ponto
double getX1Linha(Linha l);

/// @brief Retorna coordenada Y do primeiro ponto
/// @param l Linha
/// @return Coordenada Y do primeiro ponto
double getY1Linha(Linha l);

/// @brief Retorna a coordenada X do segundo ponto
/// @param l Linha
/// @return Coordenada X do segundo ponto
double getX2Linha(Linha l);

/// @brief Retorna a coordenada Y do segundo ponto
/// @param l Linha
/// @return Coordenada Y do segundo ponto
double getY2Linha(Linha l);

/// @brief Retorna a cor da linha
/// @param l Linha
/// @return Cor da linha
char* getCorLinha(Linha l);

/// @brief Realiza o cálculo da área da linha
/// @param l Linha
/// @return Área da linha
double getAreaLinha(Linha l);

/// @brief Define o identificador da linha
/// @param l Linha
/// @param i Novo identificador
void setIDLinha(Linha l, int i);

/// @brief Define a coordenada X do primeiro ponto
/// @param l Linha
/// @param x1 Coordenada X do primeiro ponto da linha
void setX1Linha(Linha l, double x1);

///@brief Define a coordenada Y do primeiro ponto
/// @param l Linha
/// @param y1 Coordenada Y do primeiro ponto da linha
void setY1Linha(Linha l, double y1);

/// @brief Define a coordenada X do segundo ponto 
/// @param l Linha
/// @param x2 Coordenada X do segundo ponto da linha
void setX2Linha(Linha l, double x2);

/// @brief Define a coordenada Y do segundo ponto
/// @param l Linha
/// @param y2 Coordenada Y do segundo ponto da linha
void setY2Linha(Linha l, double y2);

/// @brief Define a cor da linha
/// @param l Linha
/// @param Cor Cor da linha
void setCorLinha(Linha l, char* cor);

/// @brief Libera uma linha
/// @param l Linha
void liberaLinha(Linha l);

#endif