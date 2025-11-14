#ifndef TEXTO_H
#define TEXTO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    Este arquivo se refere à forma texto e suas respectivas características e propriedades úteis ao primeiro trabalho de estrutura de dados I.
*/

typedef void* Texto;
/*
    Arquivo .h para criação do texto e suas operações como set, get e cáculo de área.
*/

typedef void* Estilo;
/*
    Arquivo .h para criação e mudança de estilos de texto.
*/

// ------------------ FUNÇÕES DE TEXTO --------------------- //
/// @brief Cria e retorna um texto com os atributos fornecidos
/// @param i Identificador do texto
/// @param x Coordenada X do texto
/// @param y Coordenada Y do texto
/// @param corb Cor da borda
/// @param corp Cor de preenchimento 
/// @param a Posição da âncora do texto (inicio, meio, fim)

/// @param txto Texto
/// @param ts Estilo do texto
/// @return Retorna o texto criado
Texto criarTexto(int i, double x, double y, char* corb, char* corp, char a, char* txto);

/// @brief Retorna o identificador do texto
/// @param t Texto
/// @return Identificador do texto
int getIDTexto(Texto t);

/// @brief Retorna a coordenada X do texto
/// @param t Texto 
/// @return Coordenada X 
double getCoordXTexto(Texto t);

/// @brief Retorna a coordenada Y do texto
/// @param t Texto 
/// @return Coordenada Y do texto
double getCoordYTexto(Texto t);

/// @brief Retorna a cor da borda do texto
/// @param t Texto
/// @return Cor da borda do texto 
char* getCorBTexto(Texto t); 

/// @brief Retorna a cor de preenchimento
/// @param t Texto
/// @return Cor de preenchimento do texto
char* getCorPTexto(Texto t);

/// @brief Retorna a posição da âncora(inicio, meio, fim)
/// @param t Texto
/// @return Posição da âncora(início, meio, fim)
char getATexto(Texto t);

/// @brief Retorna o texto
/// @param t Texto 
/// @return Texto escrito
char* getTxtoTexto(Texto t);

/// @brief Retorna a quantidade de letras do texto
/// @param t Texto
/// @return Quantidade de letras do texto
int quantidadeLetras(Texto t);

/// @brief Retorna o estilo do texto
/// @param t Texto
/// @return Estilo do texto
Estilo getEstiloTexto(Texto t);

/// @brief Retorna o cálculo da área do texto
/// @param t Texto
/// @return Área do texto
double getAreaTexto(Texto t);

/// @brief Define o identificador do texto
/// @param t Texto
/// @param i Novo identificador
void setIDTexto(Texto t, int i);

/// @brief Define a coordenada X do texto
/// @param t Texto
/// @param x Nova coordenada X do texto
void setXTexto(Texto t, double x);

/// @brief Define a coordenada Y do texto
/// @param t Texto
/// @param y Nova coordenada Y do texto
void setYTexto(Texto t, double y);

/// @brief Define a cor da borda do texto
/// @param t Texto
/// @param corb Nova cor da borda do texto
void setCorBTexto(Texto t, char* corb);

/// @brief Define a cor de preenchimento do texto
/// @param t Texto
/// @param corp Nova cor de preenchimento
void setCorPTexto(Texto t, char* corp);

/// @brief Define a posição da âncora do texto
/// @param t Texto
/// @param a Nova posição da ancora
void setATexto(Texto t, char anc);

/// @brief Define o texto
/// @param t Texto
/// @param txto Novo texto
void setTxtoTexto(Texto t, char* txto);

/// @brief Define o estilo de um texto
/// @param t Texto
/// @param es Estilo
void setEstiloTexto(Texto t, Estilo es);

/// @brief Libera a memória de um texto
/// @param t Texto
void liberaTexto(Texto t);

//FUNÇÕES DE ESTILO

/// @param fFamily Familia da fonte
/// @param fWeight Weight da fonte
/// @param fSize Tamanho da fonte
/// @return Retorna o estilo
Estilo criarEstilo(char *fFamily, char *fWeight, char *fSize);

/// @brief Retorna a familia da fonte do texto 
/// @param ts Estilo ts
/// @return Familia
char* getfFamily(Estilo ts);

/// @brief Retorna o weight da fonte do texto 
/// @param ts Estilo ts
/// @return Weight
char* getfWeight(Estilo ts);

/// @brief Retorna o tamanho da fonte do texto
/// @param ts Estilo ts
/// @return Tamanho da fonte
char* getfSize(Estilo ts);

/// @brief Define a familia da fonte do texto 
/// @param ts Estilo ts
/// @param family Nova familia da fonte
void setfFamily(Estilo ts, char *family); 

/// @brief Define o weight da fonte do texto
/// @param ts Estilo ts
/// @param weight Novo weight da fonte
void setfWeight(Estilo ts, char *weight);

/// @brief Define o tamanho da fonte do texto
/// @param ts Estilo ts
/// @param size Novo tamanho de fonte
void setfSize(Estilo ts, char *size);

#endif