#include "lista.h"
#include <float.h>

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

void insereLista(Lista l, void* conteudo){
    if(l == NULL || conteudo == NULL){
        printf("Erro ao inserir pacote na lista\n");
        return;
    }

    stLista* lista = (stLista*)l;

    pont novo = malloc(sizeof(stCelula));
    if(novo == NULL){
        printf("Erro ao inserir novo elemento na lista\n");
        return;
    }

    novo->conteudo = conteudo;
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

void* getConteudoCelula(CelulaLista celula){
    if(celula == NULL){
        printf("Erro em getConteudoCelula: celula NULL\n");
        return NULL;
    }
    
    stCelula* cel = (stCelula*)celula;
    return cel->conteudo;
}

CelulaLista getPrimeiraCelulaLista(Lista l){
    if(l == NULL){
        printf("Erro em getPrimeiraCelulaLista\n");
        return NULL;
    }

    stLista* lista = (stLista*)l;
    stCelula* cel = lista->inicio;
    return (CelulaLista)cel;
}

CelulaLista getProximaCelulaLista(CelulaLista celula){
    if(celula == NULL){
        printf("Erro em getProximaCelulaLista\n");
        return NULL;
    }

    stCelula* cel = (stCelula*)celula;
    return (CelulaLista)cel->proximo;
}

CelulaLista getUltimaCelulaLista(Lista l){
    if(l == NULL){
        printf("Erro em getUltimaCelulaLista\n");
        return NULL;
    }

    stLista* lista = (stLista*)l;
    return (CelulaLista)lista->fim;
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
        if(confirma == 1){
            return (Pacote)temp->conteudo;
        }

        temp = temp->proximo;
    }
    return NULL;
}

void removeLista(Lista l, int id){
    if(l == NULL){
        printf("Erro em removeLista\n");
        return;
    }
    stLista* lista = (stLista*)l;
    stCelula* temp = lista->inicio;
    if(temp == NULL){
        printf("Lista vazia.Impossível remoção\n");
        return;
    }
    while(temp != NULL){
        void* cont = (void*)temp->conteudo;
        int identificador = getIDPacote(cont);
        
        if(identificador == id){
            if(temp->anterior != NULL){
                temp->anterior->proximo = temp->proximo;
            }else{
                lista->inicio = temp->proximo;
            }

            if(temp->proximo != NULL){
                temp->proximo->anterior = temp->anterior;
            }
            else{
                lista->fim = temp->anterior;
            }
            
            lista->tamanho--;
            liberarPacote(cont);
            free(temp);
            return;
        }
        
        temp = temp->proximo;
    }
    printf("DEBUG: removeLista: ID %d not found for removal.\n", id);
    //não encontrou o pacote com o id especificado
}

void* removeRetornaConteudo(Lista l, int id) {
    if (l == NULL) {
        printf("Erro em removeRetornaConteudo: lista NULL\n");
        return NULL;
    }

    stLista* lista = (stLista*)l;
    stCelula* temp = lista->inicio;
    if (temp == NULL) {
        printf("Lista vazia. Impossível remover.\n");
        return NULL;
    }

    while (temp != NULL) {
        void* cont = (void*)temp->conteudo;
        int identificador = getIDPacote(cont);

        if (identificador == id) {
            if (temp->anterior != NULL) {
                temp->anterior->proximo = temp->proximo;
            } else {
                lista->inicio = temp->proximo;
            }

            if (temp->proximo != NULL) {
                temp->proximo->anterior = temp->anterior;
            } else {
                lista->fim = temp->anterior;
            }
            
            lista->tamanho--;
            void* retConteudo = temp->conteudo;
            free(temp); // Free the cell, but not its content
            return retConteudo;
        }
        temp = temp->proximo;
    }
    return NULL; // Content not found
}

int getMaiorIdLista(Lista l){
    if(l == NULL) return -1;

    stLista* original = (stLista*)l;

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

double getMaiorValorLista(Lista lista, int opcao){
    if(lista == NULL){
        printf("Erro em getMaiorLista\n");
        return -1;
    }

    stLista* l = (stLista*)lista;
    stCelula* temp = l->inicio;
    if(temp == NULL){
        return 0.0; // Lista vazia
    }
    
    double maior = -DBL_MAX; // Inicializa com menor valor possível

    if(opcao == 1){ //eixo x
        while(temp != NULL){
            Pacote pac = (Pacote)temp->conteudo;
            Forma form = getFormaPacote(pac);
            char tipo = getTipoPacote(pac);
            double valor = -2;
            if(tipo == 'c') valor = getCoordXCirculo((Circulo)form) + getRaioCirculo((Circulo)form);
            if(tipo == 'r') valor = getCoordXRetangulo((Retangulo)form) + getWRetangulo((Retangulo)form);
            if(tipo == 'l'){
                double x1 = getX1Linha((Linha)form);
                double x2 = getX2Linha((Linha)form);

                valor = (x1>x2) ? x1 : x2;
            }
            if(tipo == 't'){
                Texto t = (Texto)form;
                char ancora = getATexto(t);
                double x1,x2;
                if(ancora == 'i'){
                x1 = getCoordXTexto(t);
                x2 = x1 + 10.0*strlen(getTxtoTexto(t));

                }else if(ancora == 'f'){
                    x1 = getCoordXTexto(t) - 10*strlen(getTxtoTexto(t));
                    x2 = getCoordXTexto(t);

                }else if(ancora == 'm'){
                    x1 = getCoordXTexto(t) - 10*strlen(getTxtoTexto(t))/2;
                    x2 = getCoordXTexto(t) + 10*strlen(getTxtoTexto(t))/2;

                }else{
                    printf("Âncora inválida em getMaiorLista\n");
                }

                valor = (x1>x2) ? x1 : x2;
            }
    
            maior = (maior > valor) ? maior : valor;
            temp = temp->proximo;
        }
    }else if(opcao == 2){ //eixo y
        while(temp != NULL){
            Pacote pac = (Pacote)temp->conteudo;
            Forma form = getFormaPacote(pac);
            char tipo = getTipoPacote(pac);
            double valor = -2;
            if(tipo == 'c') valor = getCoordYCirculo((Circulo)form) + getRaioCirculo((Circulo)form);
            if(tipo == 'r') valor = getCoordYRetangulo((Retangulo)form) + getHRetangulo((Retangulo)form);
            if(tipo == 'l'){
                double y1 = getY1Linha((Linha)form);
                double y2 = getY2Linha((Linha)form);

                valor = (y1>y2) ? y1 : y2;
            }
            if(tipo == 't'){
                Texto t = (Texto)form;
                char ancora = getATexto(t);
                double y1,y2;
                if(ancora == 'i'){
                y1 = getCoordYTexto(t);
                y2 = y1 + 10.0*strlen(getTxtoTexto(t));

                }else if(ancora == 'f'){
                    y1 = getCoordYTexto(t) - 10*strlen(getTxtoTexto(t));
                    y2 = getCoordYTexto(t);

                }else if(ancora == 'm'){
                    y1 = getCoordYTexto(t) - 10*strlen(getTxtoTexto(t))/2;
                    y2 = getCoordYTexto(t) + 10*strlen(getTxtoTexto(t))/2;

                }else{
                    printf("Âncora inválida em getMaiorLista\n");
                }

                valor = (y1>y2) ? y1 : y2;
            }
    
            maior = (maior > valor) ? maior : valor;
            temp = temp->proximo;
        }

    }
    return maior;
}

double getMenorValorLista(Lista lista, int opcao){
    if(lista == NULL){
        printf("Erro em getMenorLista\n");
        return -1;
    }

    stLista* l = (stLista*)lista;
    stCelula* temp = l->inicio;
    
    if(temp == NULL){
        return 0.0; // Lista vazia
    }
    
    double menor = DBL_MAX; // Inicializa com maior valor possível
    if(opcao == 1){//eixo x
        while(temp != NULL){
            Pacote pac = temp->conteudo;
            Forma f = getFormaPacote(pac);
            char tipo = getTipoPacote(pac);
            double valor = -2;
            if(tipo == 'c') valor = getCoordXCirculo((Circulo)f) - getRaioCirculo((Circulo)f);
            else if(tipo == 'r') valor = getCoordXRetangulo((Retangulo)f);
            else if(tipo == 'l'){
                double x1 = getX1Linha((Linha)f);
                double x2 = getX2Linha((Linha)f);
                valor = (x1<x2) ? x1 : x2;
            }else if(tipo == 't'){
                Texto t = (Texto)f;
                char ancora = getATexto(t);
                double x1,x2;
                if(ancora == 'i'){
                x1 = getCoordXTexto(t);
                x2 = x1 + 10.0*strlen(getTxtoTexto(t));

                }else if(ancora == 'f'){
                    x1 = getCoordXTexto(t) - 10*strlen(getTxtoTexto(t));
                    x2 = getCoordXTexto(t);

                }else if(ancora == 'm'){
                    x1 = getCoordXTexto(t) - 10*strlen(getTxtoTexto(t))/2;
                    x2 = getCoordXTexto(t) + 10*strlen(getTxtoTexto(t))/2;

                }else{
                    printf("Âncora inválida em getMenorLista\n");
                }

                valor = (x1<x2) ? x1 : x2;
            }

            menor = (menor < valor) ? menor : valor;
            temp = temp->proximo;
        }

    }else if(opcao == 2){ //eixo y
        while(temp != NULL){
            Pacote pac = temp->conteudo;
            Forma f = getFormaPacote(pac);
            char tipo = getTipoPacote(pac);
            double valor = -2;

            if(tipo == 'c') valor = getCoordYCirculo((Circulo)f) - getRaioCirculo((Circulo)f);
            else if(tipo == 'r') valor = getCoordYRetangulo((Retangulo)f);
            else if(tipo=='l'){
                double y1 = getY1Linha((Linha)f);
                double y2 = getY2Linha((Linha)f);
                valor = (y1<y2) ? y1 : y2;
            }else if(tipo == 't'){
                Texto t = (Texto)f;
                char ancora = getATexto(t);
                double y1,y2;
                if(ancora == 'i'){
                y1 = getCoordYTexto(t);
                y2 = y1 + 10.0*strlen(getTxtoTexto(t));

                }else if(ancora == 'f'){
                    y1 = getCoordYTexto(t) - 10*strlen(getTxtoTexto(t));
                    y2 = getCoordYTexto(t);

                }else if(ancora == 'm'){
                    y1 = getCoordYTexto(t) - 10*strlen(getTxtoTexto(t))/2;
                    y2 = getCoordYTexto(t) + 10*strlen(getTxtoTexto(t))/2;

                }else{
                    printf("Âncora inválida em getMenorLista\n");
                }

                valor = (y1<y2) ? y1 : y2;
            }
            menor = (menor < valor) ? menor : valor;
            temp = temp->proximo;
        }

    }
    return menor;
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

void liberaEstruturaLista(Lista l){
    if(l == NULL) return;
    
    stLista* lista = (stLista*)l;
    stCelula* temp = lista->inicio;
    stCelula* proximo;
    
    //percorre a lista liberando cada célula
    while(temp != NULL) {
        proximo = temp->proximo;
        free(temp);
        temp = proximo;
    }
    
    //libera a estrutura da lista
    free(lista);
}

void removeDaListaPorConteudo(Lista l, void* conteudo) {
    if (l == NULL || conteudo == NULL) {
        return;
    }

    stLista* lista = (stLista*)l;
    stCelula* temp = lista->inicio;

    while (temp != NULL) {
        if (temp->conteudo == conteudo) {
            if (temp->anterior != NULL) {
                temp->anterior->proximo = temp->proximo;
            } else {
                lista->inicio = temp->proximo;
            }

            if (temp->proximo != NULL) {
                temp->proximo->anterior = temp->anterior;
            } else {
                lista->fim = temp->anterior;
            }
            
            lista->tamanho--;
            free(temp);
            return; 
        }
        temp = temp->proximo;
    }
}
