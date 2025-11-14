#ifndef CIRCULO_H
#define CIRCULO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    Este arquivo se refere à forma círculo e suas características e propriedades úteis ao primeiro trabalho de estrutura de dados I.
*/

typedef void *Circulo;
/*
    Este é um arquivo .h que se refere a um circulo e suas operações como get, set e cálculo de área.
*/

/// @brief Cria e retorna um novo círculo com os atributos fornecidos.
/// @param i Identificador do círculo
/// @param x Coordenada x do centro do círculo
/// @param y Coordenada y do centro do círculo
/// @param r Raio do círculo
/// @param corb Cor da borda do círculo
/// @param corp Cor interna de preenchimento
/// @return Retorna o círculo criado
Circulo criaCirculo(int i,double x, double y, double r, char* corb, char* corp);

/// @brief Retorna o identificador do círculo
/// @param c Círculo
/// @return Identificador do círculo
int getIDCirculo(Circulo c);

/// @brief Retorna a coordenada X do centro do círculo
/// @param c Círculo
/// @return Coordenada X do centro do círculo
double getCoordXCirculo(Circulo c);

/// @brief Retorna a coordenada Y do centro do círculo
/// @param c Círculo
/// @return Coordenada Y do centro do círculo
double getCoordYCirculo(Circulo c);

/// @brief Retorna o raio do círculo
/// @param c Círculo
/// @return Raio do círculo
double getRaioCirculo(Circulo c);

/// @brief Retorna a cor da borda do círculo
/// @param c Círculo
/// @return Cor da borda do círculo
char* getCorBCirculo(Circulo c);

/// @brief Retorna a cor de preenchimento do círculo
/// @param c Círculo
/// @return Cor interna do círculo
char* getCorPCirculo(Circulo c);

/// @brief Realiza o cálculo da área e o retorna
/// @param c Círculo
/// @return Área do círculo
double getAreaCirculo(Circulo c);

/// @brief Define o identificador do círculo
/// @param c Círculo
/// @param i Novo identificador 
void setIDCirculo(Circulo c, int i);

/// @brief Define a coordenada X do centro do círculo
/// @param c Círculo
/// @param x Coordenada X do círculo
void setXCirculo(Circulo c, double x);

/// @brief Define a coordenada Y do centro do círculo
/// @param c Círculo
/// @param y Coordenada Y do círculo
void setYCirculo(Circulo c, double y);

/// @brief Define o raio do círculo
/// @param c Círculo
/// @param r Raio
void setRaioCirculo(Circulo c, double r);

/// @brief Define a cor da borda do círculo
/// @param c Círculo
/// @param corb Cor da borda do círculo
void setCorBCirculo(Circulo c, char* corb);

/// @brief Define a cor de preenchimento do círculo
/// @param c Círculo
/// @param corp Cor interna do círculo
void setCorPCirculo(Circulo c, char* corp);

/// @brief Libera um círculo
/// @param c Círculo
void liberaCirculo(Circulo c);
#endif