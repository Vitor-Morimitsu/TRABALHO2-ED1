#ifndef GEO_H
#define GEO_H

#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"
#include "pacote.h"
#include "lista.h"
#include "svg.h"

/*
    Arquivo .h responsável pela leitura do arquivo geo.
*/

/*
    Função responsável por ler o arquivo geo passado e gerar o svg inicial.
*/
void lerGeo(FILE* geo, FILE* svgEntrada, Lista list);



#endif