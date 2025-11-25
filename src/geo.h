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
    Função responsável por ler o arquivo geo passado, gerar o svg inicial e registrar as formas em pacotes e colocá-los na lista.
*/
void lerGeo(FILE* geo, FILE* svgEntrada, Lista list);

#endif