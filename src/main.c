#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "geo.h"
#include "lista.h"
#include "qry.h"
#include "svg.h"

#define PATH_LEN 512
#define FILE_NAME_LEN 256
#define MAX_FULL_PATH 4096

void removeExtension(char* dest, const char* src) {
    strcpy(dest, src);
    char* dot = strrchr(dest, '.');
    if (dot != NULL) {
        *dot = '\0';
    }
}

int main(int argc, char* argv[]) {
    char dirEntrada[PATH_LEN] = "";
    char dirSaida[PATH_LEN] = "";
    char nomeArquivoGeo[FILE_NAME_LEN] = "";
    char nomeArquivoQry[FILE_NAME_LEN] = "";
    char tipoOrdenacao = 'q';
    int limiteInsertionSort = 10;
    int hasGeo = 0, hasQry = 0, hasSaida = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-e") == 0 && i + 1 < argc) {
            strcpy(dirEntrada, argv[++i]);
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            strcpy(dirSaida, argv[++i]);
            hasSaida = 1;
        } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            strcpy(nomeArquivoGeo, argv[++i]);
            hasGeo = 1;
        } else if (strcmp(argv[i], "-q") == 0 && i + 1 < argc) {
            strcpy(nomeArquivoQry, argv[++i]);
            hasQry = 1;
        } else if (strcmp(argv[i], "-to") == 0 && i + 1 < argc) {  
            i++;  
            if (strcmp(argv[i], "q") == 0) {
                tipoOrdenacao = 'q';
            } else if (strcmp(argv[i], "m") == 0) {
                tipoOrdenacao = 'm';
            } else {
                fprintf(stderr, "Aviso: tipo de ordenacao invalido '%s', usando 'q'\n", argv[i]);
                tipoOrdenacao = 'q';
            }
        } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) { 
            limiteInsertionSort = atoi(argv[++i]);  
            if (limiteInsertionSort <= 0) {
                fprintf(stderr, "Aviso: limite insertion sort invalido, usando 10\n");
                limiteInsertionSort = 10;
            }
        } else {
            fprintf(stderr, "Parametro desconhecido ou invalido: %s\n", argv[i]);
            return EXIT_FAILURE;
        }
    }

    if (!hasGeo || !hasSaida) {
        fprintf(stderr, "Erro: parametros obrigatorios -f (geo) e -o (saida) nao fornecidos.\n");
        return EXIT_FAILURE;
    }

    char fullPathGeo[MAX_FULL_PATH];
    snprintf(fullPathGeo, sizeof(fullPathGeo), "%s/%s", dirEntrada, nomeArquivoGeo);

    char fullPathQry[MAX_FULL_PATH];
    if (hasQry) {
        snprintf(fullPathQry, sizeof(fullPathQry), "%s/%s", dirEntrada, nomeArquivoQry);
    }

    char baseGeo[FILE_NAME_LEN];
    removeExtension(baseGeo, nomeArquivoGeo);

    char baseQry[FILE_NAME_LEN] = "";
    if (hasQry) {
        char onlyQryName[FILE_NAME_LEN];
        char *p = strrchr(nomeArquivoQry, '/');
        strcpy(onlyQryName, p ? p + 1 : nomeArquivoQry);
        removeExtension(baseQry, onlyQryName);
    }

    char arquivoSvgGeo[MAX_FULL_PATH];
    char arquivoSvgFinal[MAX_FULL_PATH];
    char arquivoSaidaTxt[MAX_FULL_PATH];

    snprintf(arquivoSvgGeo, sizeof(arquivoSvgGeo), "%s/%s.svg", dirSaida, baseGeo);

    if (hasQry) {
        snprintf(arquivoSvgFinal, sizeof(arquivoSvgFinal), "%s/%s-%s.svg", dirSaida, baseGeo, baseQry);
        snprintf(arquivoSaidaTxt, sizeof(arquivoSaidaTxt), "%s/%s-%s.txt", dirSaida, baseGeo, baseQry);
    }

    FILE *arqGeo = fopen(fullPathGeo, "r");
    if (arqGeo == NULL) {
        fprintf(stderr, "ERRO: Não foi possível abrir o arquivo .geo: %s\n", fullPathGeo);
        return EXIT_FAILURE;
    }

    Lista formas = criarLista();
    if (formas == NULL) {
        fprintf(stderr, "ERRO: Não foi possível criar lista de formas\n");
        fclose(arqGeo);
        return EXIT_FAILURE;
    }
    
    lerGeo(arqGeo, formas);
    fclose(arqGeo);

    FILE *arqSvgGeo = fopen(arquivoSvgGeo, "w");
    if (arqSvgGeo == NULL) {
        fprintf(stderr, "ERRO: Não foi possível criar o arquivo SVG: %s\n", arquivoSvgGeo);
        liberaLista(formas);
        return EXIT_FAILURE;
    }

    abrirSvg(arqSvgGeo);
    desenharTodasAsFormas(arqSvgGeo, formas);
    fecharSVG(arqSvgGeo);
    fclose(arqSvgGeo);

    if (hasQry) {
        FILE *arqQry = fopen(fullPathQry, "r");
        if (arqQry == NULL) {
            fprintf(stderr, "ERRO: Não foi possível abrir o arquivo .qry: %s\n", fullPathQry);
            liberaLista(formas);
            return EXIT_FAILURE;
        }

        FILE *arqTxt = fopen(arquivoSaidaTxt, "w");
        if (arqTxt == NULL) {
            fprintf(stderr, "ERRO: Não foi possível criar o arquivo .txt: %s\n", arquivoSaidaTxt);
            fclose(arqQry);
            liberaLista(formas);
            return EXIT_FAILURE;
        }

        FILE *arqSvgFinalFile = fopen(arquivoSvgFinal, "w");
        if (arqSvgFinalFile == NULL) {
            fprintf(stderr, "ERRO: Não foi possível criar o arquivo SVG final: %s\n", arquivoSvgFinal);
            fclose(arqQry);
            fclose(arqTxt);
            liberaLista(formas);
            return EXIT_FAILURE;
        }

        char comandoOrdenacao[2];
        comandoOrdenacao[0] = tipoOrdenacao;
        comandoOrdenacao[1] = '\0';

        abrirSvg(arqSvgFinalFile);
        lerQry(arqQry, arqTxt, arqSvgFinalFile, formas, comandoOrdenacao, limiteInsertionSort);
        desenharTodasAsFormas(arqSvgFinalFile, formas);
        fecharSVG(arqSvgFinalFile);

        fclose(arqQry);
        fclose(arqTxt);
        fclose(arqSvgFinalFile);
    }

    liberaLista(formas);

    return EXIT_SUCCESS;
}