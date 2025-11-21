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

void insereArvore(Arvore arv, void* segmentoAtivo){
    if(arv == NULL || segmentoAtivo == NULL){
        printf("Erro em insereArvore\n");
        return;
    }

    stArvore* arvore = (stArvore*)arv;
    if(arvore->raiz == NULL){//é o primeiro elemento
        arvore->raiz = segmentoAtivo;    
    }else{

    }

}