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

int comparaPacote(Pacote pac, int id){
    if(pac == NULL){
        printf("Erro ao comparar pacotes.");
        return;
    }

    stPacote* p = (stPacote*)pac;
    int idForma = -1;

    if(p->tipo == 'c'){
        return getIDCirculo((Circulo*)p->fig);
    }else if(p->tipo == 'r'){
        return getIDRetangulo((Retangulo*)p->fig);
    }else if(p->tipo == 'l'){
        return getIDLinha((Linha*)p->fig);
    }else if(p->tipo == 't'){
        return  getIDTexto((Texto*)p->fig);
    }

}