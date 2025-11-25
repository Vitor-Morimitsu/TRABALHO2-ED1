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
        return NULL;
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

int getTamanhoLista(Lista l){
    if(l == NULL){
        printf("Erro em getTamanhoLista\n");
        return -1;
    }

    return ((stLista*)l)->tamanho;
}

Pacote procuraPacoteLista(Lista l, int id){
    if(l == NULL){
        printf("Erro em procuraPacoteLista\n");
        return NULL;
    }

    stLista* lista = (stLista*)l;
    stCelula* temp = lista->inicio;
    int confirma = -1;
    while(temp != NULL){
        confirma = comparaPacote((Pacote)temp->conteudo, id);
        if(confirma == 0){
            return (Pacote)temp->conteudo;
        }

        temp = temp->proximo;
    }
    return NULL;
}

Pacote removeLista(Lista l, int id){
    if(l == NULL){
        printf("Erro em removeLista\n");
        return NULL;
    }

    int identificador = -1;
    stLista* lista = (stLista*)l;
    stCelula* temp = lista->inicio;
    if(temp == NULL){
        printf("Lista vazia.Impossível remoção\n");
        return NULL;
    }

    while(temp != NULL){
        Pacote p = (Pacote)temp->conteudo;
        int identificador = getIDPacote(p);
        
        if(identificador == id){
            if(temp->anterior != NULL){
                temp->anterior->proximo = temp->proximo;
            }else{
                //primeiro elemento
                lista->inicio = temp->proximo;
            }

            if(temp->proximo != NULL){
                temp->proximo->anterior = temp->anterior;
            }else{
                //último elemento
                lista->fim = temp->anterior;
            }
            
            lista->tamanho--;  
            free(temp);        
            return p;          
        }
        
        temp = temp->proximo;
    }
    
    //não encontrou o pacote com o id especificado
    return NULL;
}

int getMaiorIdLista(Lista l){
    if(l == NULL) return;

    stLista* original = (stLista*)l;

    int tamanho = getTamanhoLista(l);
    int maiorAtual = -1;

    stCelula* temp = original->inicio;

    while(temp != NULL){
        Pacote p = (Pacote)temp->conteudo;
        Forma f = getFormaPacote(p);
        char tipo = getTipoPacote(p);
        int idForma = 0;
        if(tipo == 'c'){
            idForma = getIDCirculo((Circulo)f);
        }else if(tipo == 'r'){
            idForma = getIDRetangulo((Retangulo)f);
        }else if(tipo == 'l'){
            idForma = getIDLinha((Linha)f);
        }else if(tipo == 't'){
            idForma = getIDTexto((Texto)f);
        }

        if(idForma > maiorAtual) maiorAtual = idForma;

        temp = temp->proximo;
    }
    return maiorAtual;
}

void liberaLista(Lista l){
    if(l == NULL) return;
    
    stLista* lista = (stLista*)l;
    stCelula* temp = lista->inicio;
    stCelula* proximo;
    
    //percorre a lista liberando cada célula
    while(temp != NULL) {
        proximo = temp->proximo;
        
        liberarPacote((Pacote)temp->conteudo);
        
        free(temp);
        temp = proximo;
    }
    
    //libera a estrutura da lista
    free(lista);
}
