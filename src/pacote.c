#include "pacote.h"
#include "poligono.h"

typedef struct{
    Forma fig;
    char tipo;
}stPacote;

Pacote criaPacote(){
    stPacote* pac = (stPacote*)malloc(sizeof(stPacote));
    if(pac == NULL){
        printf("Erro ao criar pacote\n");
        return NULL;
    }
    pac->fig = NULL;
    pac->tipo = 'n';
    return (Pacote)pac;
}

Forma getFormaPacote(Pacote pac){
    stPacote* p = (stPacote*)pac;
    
    if(p == NULL){
        printf("Erro em getFormaPacote\n");
        return NULL;
    }

    return p->fig;
}

char getTipoPacote(Pacote pac){
    stPacote* p = (stPacote*)pac;
    if(p == NULL){
        printf("Erro em getTipoPacote\n");
        return '\0';
    }

    return p->tipo;
}

int getIDPacote(Pacote pac){
    if(pac == NULL){
        printf("Erro em getIDPacote\n");
        return -1;
    }
    
    int id = -1;
    char tipo = getTipoPacote(pac);
    Forma form = getFormaPacote(pac);

    if(tipo == 'c'){
        id = getIDCirculo((Circulo)form);
    }else if(tipo == 'r'){
        id = getIDRetangulo((Retangulo)form);
    }else if(tipo == 'l'){
        id = getIDLinha((Linha)form);
    }else if(tipo == 't'){
        id = getIDTexto((Texto)form);
    }

    return id;
}

void setFormaPacote(Pacote pac, Forma form){
    if(pac == NULL || form == NULL){
        printf("Erro em setFormaPacote\n");
        return;
    }

    stPacote* p = (stPacote*)pac;
    p->fig = form;
}

void setTipoPacote(Pacote pac, char type){
    if(pac == NULL){
        printf("Erro em setTipoPacote\n");
        return;
    }
    stPacote* p = (stPacote*)pac;
    p->tipo = type;
}

int comparaPacote(Pacote pac, int id){
    if(pac == NULL){
        printf("Erro ao comparar pacotes.\n");
        return 0; // Retorna 0 para erro/sem correspondência
    }

    stPacote* p = (stPacote*)pac;
    int shape_id = -1;

    // Obtém o ID da forma dentro do pacote
    if(p->tipo == 'c'){
        shape_id = getIDCirculo((Circulo)p->fig);
    }else if(p->tipo == 'r'){
        shape_id = getIDRetangulo((Retangulo)p->fig);
    }else if(p->tipo == 'l'){
        shape_id = getIDLinha((Linha)p->fig);
    }else if(p->tipo == 't'){
        shape_id = getIDTexto((Texto)p->fig);
    } else {
        printf("Erro: Tipo de forma desconhecido em comparaPacote.\n");
        return 0;
    }

    // Compara o shape_id extraído com o id fornecido
    return shape_id == id; // Retorna 1 se verdadeiro, 0 se falso.
}

void liberarPacote(Pacote p) {
    if(p == NULL) {
        return;
    }
    if(p == NULL) {
        return;
    }
    if(p == NULL) {
        return;
    }
    
    stPacote* pacote = (stPacote*)p;
    
    // Verifica se o tipo está inicializado antes de usar
    if(pacote->fig != NULL && pacote->tipo != '\0' && pacote->tipo != 'n') {
        if(pacote->tipo == 'c'){
            liberaCirculo((Circulo)pacote->fig);
        }else if(pacote->tipo == 'r'){
            liberaRetangulo((Retangulo)pacote->fig);
        }else if(pacote->tipo == 'l'){
            liberaLinha((Linha)pacote->fig);
        }else if(pacote->tipo == 't'){
            liberaTexto((Texto)pacote->fig);
        }else if(pacote->tipo == 'p'){ 
            liberarPoligono((Poligono)pacote->fig);
        }else{
            // Tipo desconhecido, mas não é erro se for 'n' (não inicializado)
            fprintf(stderr, "Aviso: Tipo de pacote desconhecido '%c' ao liberar\n", pacote->tipo);
        }
    }
    
    free(pacote);
}