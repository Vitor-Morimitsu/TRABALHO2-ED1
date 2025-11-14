#include "pacote.h"

typedef struct{
    Forma fig;
    char tipo;
}stPacote;

Pacote criaPacote(){
    stPacote* pac = (stPacote*)malloc(sizeof(stPacote));
    if(pac == NULL){
        printf("Erro ao criar pacote\n");
        return;
    }
    return pac;
}

Forma getFormaPacote(Pacote pac){
    stPacote* p = (stPacote*)pac;
    if(p == NULL){
        printf("Erro em getFormaPacote\n");
        return;
    }

    return p->fig;
}

char getTipoPacote(Pacote pac){
    stPacote* p = (stPacote*)pac;
    if(p == NULL){
        printf("Erro em getTipoPacote\n");
        return;
    }

    return p->tipo;
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