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
    stArvore* arvore = malloc(sizeof(stArvore));
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
    stCelulaArvore* novaCelula = malloc(sizeof(stCelulaArvore));
    if(novaCelula == NULL) {
        printf("Erro ao alocar espaço para a nova célula da árvore\n");
        return;
    }
    novaCelula->conteudo = segmentoAtivo;
    novaCelula->esquerda = NULL;
    novaCelula->direita = NULL;
    
    //árvore vazia
    if(arvore->raiz == NULL){
        arvore->raiz = novaCelula;
        arvore->tamanho++;
        return;
    }
    
    //inserção na esquerda 
    stCelulaArvore* atual = arvore->raiz;
    while(atual->esquerda != NULL) {
        if(atual->direita == NULL){
            atual->direita = novaCelula;
            arvore->tamanho++;
            return;
        }
        atual = atual->esquerda;

    }
    atual->esquerda = novaCelula;
    
    arvore->tamanho++;
}