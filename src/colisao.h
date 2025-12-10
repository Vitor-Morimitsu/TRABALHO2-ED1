#ifndef COLISAO_H
#define COLISAO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lista.h"
#include "pacote.h"
#include "poligono.h"

/*
    Arquivo .h responsável por verificar as formas que foram atingidas pelas explosões.
/*

/*
    Verifica se um círculo está dentro do polígono de visibilidade.
    Um círculo está dentro se seu centro está dentro do polígono.
*/
bool circuloDentroPoligono(Circulo c, Poligono p);

/*
    Verifica se um retângulo está dentro do polígono de visibilidade.
    Um retângulo está dentro se seu centro está dentro do polígono.
*/
bool retanguloDentroPoligono(Retangulo r, Poligono p);

/*
    Verifica se uma linha está dentro do polígono de visibilidade.
    Uma linha está dentro se seu ponto médio está dentro do polígono.
*/
bool linhaDentroPoligono(Linha l, Poligono p);

/*
    Verifica se um texto está dentro do polígono de visibilidade.
    Um texto está dentro se sua posição está dentro do polígono.
*/
bool textoDentroPoligono(Texto t, Poligono p);

/*
    Verifica se uma forma (pacote) está dentro do polígono de visibilidade.
*/
bool formaDentroPoligono(Pacote pac, Poligono p);

/*
    Verifica se alguma forma foi atingida pela explosão (está dentro do polígono de visibilidade).
    Retorna true se pelo menos uma forma foi atingida.
    @param formas: lista de pacotes contendo as formas
    @param p: polígono de visibilidade
    @param xBomba: coordenada X da bomba
    @param yBomba: coordenada Y da bomba
*/
bool atingidaPelaExplosao(Lista formas, Poligono p, double xBomba, double yBomba);

/*
    Retorna uma lista com todas as formas que estão dentro do polígono de visibilidade.
    A lista retornada contém referências aos pacotes originais (não cópias).
    @param formas: lista de pacotes contendo as formas
    @param p: polígono de visibilidade
    @return Lista com as formas atingidas (deve ser liberada com liberaLista, mas não os pacotes)
*/
Lista obterFormasAtingidas(Lista formas, Poligono p);

/*
    Verifica se uma forma específica (pacote) está dentro do polígono de visibilidade.
    @param pac: pacote contendo a forma a ser verificada
    @param p: polígono de visibilidade
    @return true se a forma está dentro do polígono, false caso contrário
*/
bool formaDentroPoligono(Pacote pac, Poligono p);

#endif