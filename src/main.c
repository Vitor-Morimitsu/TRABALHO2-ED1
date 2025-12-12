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

// Função para remover a extensão de um nome de arquivo
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

    char arquivoSaidaSvg[MAX_FULL_PATH];
    char arquivoSaidaTxt[MAX_FULL_PATH];

    if (hasQry) {
        snprintf(arquivoSaidaSvg, sizeof(arquivoSaidaSvg), "%s/%s-%s.svg", dirSaida, baseGeo, baseQry);
        snprintf(arquivoSaidaTxt, sizeof(arquivoSaidaTxt), "%s/%s-%s.txt", dirSaida, baseGeo, baseQry);
    } else {
        snprintf(arquivoSaidaSvg, sizeof(arquivoSaidaSvg), "%s/%s.svg", dirSaida, baseGeo);
    }

    FILE *arqGeo = fopen(fullPathGeo, "r");
    if (arqGeo == NULL) {
        fprintf(stderr, "ERRO: Não foi possível abrir o arquivo .geo: %s\n", fullPathGeo);
        return EXIT_FAILURE;
    }

    FILE *arqSvg = fopen(arquivoSaidaSvg, "w");
    if (arqSvg == NULL) {
        fprintf(stderr, "ERRO: Não foi possível criar o arquivo SVG de saída: %s\n", arquivoSaidaSvg);
        fclose(arqGeo);
        return EXIT_FAILURE;
    }

    FILE *arqQry = NULL;
    FILE *arqTxt = NULL;
    if (hasQry) {
        arqQry = fopen(fullPathQry, "r");
        if (arqQry == NULL) {
            fprintf(stderr, "ERRO: Não foi possível abrir o arquivo .qry: %s\n", fullPathQry);
            fclose(arqGeo);
            fclose(arqSvg);
            return EXIT_FAILURE;
        }
        arqTxt = fopen(arquivoSaidaTxt, "w");
        if (arqTxt == NULL) {
            fprintf(stderr, "ERRO: Não foi possível criar o arquivo .txt de saída: %s\n", arquivoSaidaTxt);
            fclose(arqGeo);
            fclose(arqSvg);
            fclose(arqQry);
            return EXIT_FAILURE;
        }
    }

    Lista formas = criarLista();
    
    abrirSvg(arqSvg);

    lerGeo(arqGeo, formas);

    if (hasQry && arqQry != NULL) {
        lerQry(arqQry, arqTxt, arqSvg, formas);
    }

    desenharTodasAsFormas(arqSvg, formas);
    fecharSVG(arqSvg);

    fclose(arqGeo);
    fclose(arqSvg);
    if (hasQry) {
        fclose(arqQry);
        fclose(arqTxt);
    }

    liberaLista(formas);

    return EXIT_SUCCESS;
}
