#include "lista.h"

typedef struct celula{
    void* conteudo;
    struct celula* anterior;
    struct celula* proximo;
}stCelula;

typedef stCelula* pont;

typedef struct{
    pont inicio;
    pont fim;
    int tamanho;
}stLista;

Lista criarLista(){
    stLista* lista = malloc(sizeof(stLista));
    if(lista == NULL){
        printf("Erro ao criar lista\n");
        return;
    }

    lista->inicio = NULL;
    lista->fim = NULL;
    lista->tamanho = 0;
    return lista;
}

void insereLista(Lista l, Pacote pac){
    if(l == NULL || pac == NULL){
        printf("Erro ao inserir pacote na lista\n");
        return;
    }

    stLista* lista = (stLista*)l;

    pont novo = malloc(sizeof(stCelula));
    if(novo == NULL){
        printf("Erro ao inserir novo elemento na lista\n");
        return;
    }

    novo->conteudo = pac;
    novo->proximo = NULL;
    if(lista->inicio == NULL){
        //lista vazia
        lista->inicio = novo;
        novo->anterior = NULL;
    }else{
        lista->fim->proximo = novo;
        novo->anterior = lista->fim;       
    }
    lista->fim = novo;
    lista->tamanho++;
}