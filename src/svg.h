#ifndef SVG_H
#define SVG_H

#include <stdio.h>
#include <stdlib.h>
#include "pacote.h"
#include "geo.h"
#include "poligono.h"

/*
    Arquivo .h responsável pela criação de arquivo svg.
*/

/*
    Função responsável por abrir o arquivo Svg passado como parâmetro.
*/
void abrirSvg(FILE* arqSvg,double x,double y, double w, double h);

/*
    Função responsável por gerar um círculo em SVG.
*/
void desenharCirculoSVG(FILE* arqSvg,Circulo circ);

/*
    Função responsável por gerar um retângulo em SVG.
*/
void desenharRetanguloSVG(FILE* arqSvg, Retangulo r);

/*
    Função responsável por gerar uma linha em SVG.
*/
void desenharLinhaSVG(FILE* arqSvg, Linha l);

/*
    Função responsável por gerar um texto em SVG.
*/
void desenharTextoSVG(FILE* arqSvg, Texto t, Estilo ts);

/*
    Função responsável por gerar um polígono no svg
*/
void desenharPoligonoSVG(FILE* arqSvg, Poligono p, char* corB, char* corP);

/// @brief Realiza o comando de encerramento do arquivo svg
/// @param arqSvg Arquivo
void fecharSVG(FILE* arqSvg);

/*
    Função que percorre a lista de formas e desenha cada uma no arquivo SVG.
*/
void desenharTodasAsFormas(FILE* arqSvg, Lista formas);
#endif