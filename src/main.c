#include <stdio.h>
#include <stdlib.h>
#include "geo.h"
#include "lista.h"
//incluir todos os arquivos 

#define PATH_LEN 512
#define FILE_NAME_LEN 256

void removeExtension(char* dest, const char* src) {
    strcpy(dest, src);
    char* dot = strrchr(dest, '.');
    if (dot != NULL) {
        *dot = '\0';
    }
}

int main(int argc, char* argv[])
{
    FILE* arqGeo = NULL;
    char dirEntrada[PATH_LEN] = "";
    char dirSaida[PATH_LEN] = "";
    char nomeArquivoGeo[FILE_NAME_LEN] = "";
    char nomeArquivoQry[FILE_NAME_LEN] = "";
    char onlyQry[FILE_NAME_LEN] = "";
    int hasGeo = 0, hasSaida = 0;

    // Processa argumentos da linha de comando
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
            char *p = strrchr(argv[i], '/');
            strcpy(onlyQry, p ? p + 1 : argv[i]);
        } else {
            fprintf(stderr, "Parametro desconhecido ou invalido: %s\n", argv[i]);
            return EXIT_FAILURE;
        }
    }

    if (!hasGeo || !hasSaida) {
        fprintf(stderr, "Erro: parametros obrigatorios -f (geo) e -o (saida) nao fornecidos.\n");
        return EXIT_FAILURE;
    }

    char fullPathGeo[1024];
    snprintf(fullPathGeo, sizeof(fullPathGeo), "%s/%s", dirEntrada, nomeArquivoGeo);

    char fullPathQry[1024];
    if (strlen(nomeArquivoQry) > 0) {
        snprintf(fullPathQry, sizeof(fullPathQry), "%s/%s", dirEntrada, nomeArquivoQry);
    }

    char baseGeo[FILE_NAME_LEN];
    char baseQry[FILE_NAME_LEN];
    removeExtension(baseGeo, nomeArquivoGeo);
    removeExtension(baseQry, onlyQry);

    char arquivoSaidaSvgGeo[1024];
    snprintf(arquivoSaidaSvgGeo, sizeof(arquivoSaidaSvgGeo), "%s/%s.svg", dirSaida, nomeArquivoGeo);

    char arquivoSaidaSvgQry[1024];
    char arquivoSaidaTxt[1024];
    if (strlen(nomeArquivoQry) > 0) {
        snprintf(arquivoSaidaSvgQry, sizeof(arquivoSaidaSvgQry), "%s/%s-%s.svg", dirSaida, baseGeo, baseQry);
        snprintf(arquivoSaidaTxt, sizeof(arquivoSaidaTxt), "%s/%s-%s.txt", dirSaida, baseGeo, baseQry);
    }
    
    arqGeo = fopen(fullPathGeo, "r");
    if(arqGeo == NULL){
        printf("ERRO: Não foi possível abrir o arquivo .geo: %s\n", fullPathGeo);
    }

    FILE* arqSvgEntrada = fopen(arquivoSaidaSvgGeo, "w");
    if(arqSvgEntrada == NULL){
        printf("ERRO: Não foi possível criar arquivo SVG de entrada: %s\n", arquivoSaidaSvgGeo);
    }

    Lista formas = criarLista();

    lerGeo(arqGeo,arqSvgEntrada,formas);

    FILE* qry = fopen(fullPathQry, "r");
    if(qry == NULL){
        printf("ERRO: Não foi possível abrir o arquivo .qry: %s\n", fullPathQry);
    }

    


    


}