#ifndef SVG_H
#define SVG_H

#include <stdio.h>
#include <stdlib.h>
#include "pacote.h"
#include "geo.h"

/*
    Arquivo .h responsável pela criação de arquivo svg.
*/

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


#endif