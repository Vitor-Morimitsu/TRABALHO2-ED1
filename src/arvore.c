#include "arvore.h"

typedef struct celula{
    Pacote conteudo;
    struct celula* esquerda;
    struct celula* direita;
}stCelulaArvore;

typedef struct{
    stCelulaArvore* raiz;
    int tamanho;
}stArvore;

Arvore criaArvore(){
    stArvore* arvore = malloc(sizeof(stCelulaArvore));
    if(arvore == NULL){
        printf("Erro ao criarArvore.\n");
        return NULL;
    }
    
    arvore->raiz = NULL;
    arvore->tamanho = 0;

    return (Arvore)arvore;
}

void insereArvore(Arvore arv, Pacote segmentoAtivo){
    if(arv == NULL || segmentoAtivo == NULL){
        printf("Erro em insereArvore\n");
        return;
    }

    stArvore* arvore = (stArvore*)arv;
    stCelulaArvore* cel = arvore->raiz;
    if(arvore->raiz == NULL){//é o primeiro elemento
        arvore->raiz = segmentoAtivo;    
    }else{
        if(cel->esquerda == NULL){
            stCelulaArvore* novaCelula = malloc(sizeof(stCelulaArvore));
            if(novaCelula == NULL) {
                printf("Erro ao alocar espaço para a nova célula da árvore\n");
                return;
            }
            cel->esquerda = novaCelula;
            novaCelula->conteudo = segmentoAtivo;
        }else{
            if(cel->direita == NULL){
                stCelulaArvore* novaCelula = malloc(sizeof(stCelulaArvore));
                if(novaCelula == NULL) {
                    printf("Erro ao alocar espaço para a nova célula da árvore\n");
                    return;
                }
                cel->direita = novaCelula;
                novaCelula->conteudo = segmentoAtivo;
            }else{
                stCelulaArvore* novaCelula = malloc(sizeof(stCelulaArvore));
                if(novaCelula == NULL) {
                    printf("Erro ao alocar espaço para a nova célula da árvore\n");
                    return;
                }
                cel->esquerda->esquerda = novaCelula;
                novaCelula->conteudo = segmentoAtivo;

            }
        }

    }
    arvore->tamanho++;

}