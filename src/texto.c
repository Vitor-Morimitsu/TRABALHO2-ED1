#include "texto.h"
typedef struct stEstilo{
    char* fFamily;
    char* fWeight;
    char* fSize;
} estilo;
typedef struct stTexto{
    int i;
    double x, y;
    char* corb;
    char* corp;
    char a;
    char* txto;
    Estilo estiloTexto;
} stTexto;

Texto criarTexto(int i, double x, double y, char* corb, char* corp, char a, char* txto){
    stTexto *t = malloc(sizeof(stTexto));
    t->i = i;
    t->x = x;
    t->y = y;
    t->corb = (char*)malloc(strlen(corb) + 1);
    if(t->corb == NULL){
        printf("Erro ao alocar memória da cor da borda do texto");
        exit(1);
    }
    strcpy(t->corb, corb);

    t->corp = (char*)malloc(strlen(corp) + 1);
    if(t->corp == NULL){
        printf("Erro ao alocar memória da cor de preenchimento do texto");
        exit(1);
    }
    strcpy(t->corp, corp);

    t->a = a;

    t->estiloTexto = NULL;

    t->txto = (char*)malloc(strlen(txto) + 1);
    if(t->txto == NULL){
        printf("Erro ao alocar memória para o texto");
        exit(1);
    }
    strcpy(t->txto, txto);

    return ((stTexto*)t);
}

int getIDTexto(Texto t){
    return((stTexto*)t)->i;
}

double getCoordXTexto(Texto t){
    return((stTexto*)t)->x;
}

double getCoordYTexto(Texto t){
    return((stTexto*)t)->y;
}

char* getCorBTexto(Texto t){
    return((stTexto*)t)->corb;
}

char* getCorPTexto(Texto t){
    return((stTexto*)t)->corp;
}

char getATexto(Texto t){
    return((stTexto*)t)->a;
}

char* getTxtoTexto(Texto t){
    return((stTexto*)t)->txto;
}

int quantidadeLetras(Texto t){
    char* texto = getTxtoTexto(t);
    if(texto == NULL) return -1;
    int quantidade = strlen(texto);
    return quantidade;
}

Estilo getEstiloTexto(Texto t){
    return((stTexto*)t)->estiloTexto;
}

double getAreaTexto(Texto t){
    return 20*strlen(((stTexto*)t)->txto);
}

void setIDTexto(Texto t, int i){
    ((stTexto*)t)->i = i;
}

void setXTexto(Texto t, double x){
    ((stTexto*)t)->x = x;
}

void setYTexto(Texto t, double y){
    ((stTexto*)t)->y = y;
}

void setCorBTexto(Texto t, char* corb){
    stTexto* texto =(stTexto*)t;
    free(texto->corb);
    texto->corb = (char*)malloc(strlen(corb)+1);
    if(texto->corb == NULL){
        printf("Erro ao alocar memória para a corb do texto\n");
        exit(1);
    }
    strcpy(texto->corb, corb);
}

void setCorPTexto(Texto t, char* corp){
    stTexto* texto =(stTexto*)t;
    free(texto->corp);
    texto->corp = (char*)malloc(strlen(corp) +1);
    if(texto->corp == NULL){
        printf("Erro ao alocar a corp do texto\n");
        exit(1);
    }
    strcpy(texto->corp, corp);
}
 
void setATexto(Texto t, char anc){
    ((stTexto*)t)->a = anc;
}

void setTxtoTexto(Texto t, char* txto){
    stTexto* tex = (stTexto*)t;
    free(tex->txto);
    tex->txto = (char*)malloc(strlen(txto)+1);
    if(tex->txto == NULL){
        printf("Erro ao alocar memória para texto em Texto\n");
        exit(1);
    }
    strcpy(tex->txto, txto);
}

void setEstiloTexto(Texto t, Estilo es){
    ((stTexto*)t)->estiloTexto = es;
}

void liberaTexto(Texto t){
    if(t == NULL) return;
    stTexto* texto = (stTexto*)t;
    free(texto->corb);
    free(texto->corp);
    if(texto->estiloTexto != NULL){
        estilo* ts = (estilo*)texto->estiloTexto;
        free(ts->fFamily);
        free(ts->fSize);
        free(ts->fWeight);
        free(texto->estiloTexto);
    }
    free(texto->txto);
    free(texto);
}



// FUNÇÕES DE ESTILO
Estilo criarEstilo(char* fFamily, char* fWeight, char*fSize){
    estilo* ts = malloc(sizeof(estilo));
    ts->fFamily = (char*)malloc(strlen(fFamily)+1);
    if(ts->fFamily == NULL){
        printf("Erro ao alocar memória para família");
        exit(1);
    }
    strcpy(ts->fFamily, fFamily);

    ts->fWeight = (char*)malloc(strlen(fWeight)+1);
    if(ts->fWeight == NULL){
        printf("Erro ao alocar memória para weight");
        exit(1);
    }
    strcpy(ts->fWeight, fWeight);

    ts->fSize = (char*)malloc(strlen(fSize)+1);
    if(ts->fSize == NULL){
        printf("Erro ao alocar memória para o tamanho");
        exit(1);
    }
    strcpy(ts->fSize, fSize);

    return((estilo*)ts);
}

char* getfFamily(Estilo ts){
    return((estilo*)ts)->fFamily;
}

char* getfWeight(Estilo ts){
    return((estilo*)ts)->fWeight;
}

char* getfSize(Estilo ts){
    return((estilo*)ts)->fSize;
}

void setfFamily(Estilo ts, char* fFamily){
    estilo* es =(estilo*)ts;
    free(es->fFamily);
    es->fFamily = malloc(strlen(fFamily)+1);
    strcpy(es->fFamily, fFamily);
}

void setfWeight(Estilo ts, char* fWeight){
    estilo* es =(estilo*)ts;
    free(es->fWeight);
    es->fWeight = malloc(strlen(fWeight)+1);
    strcpy(es->fWeight, fWeight);
}

void setfSize(Estilo ts, char* fSize){
    estilo* es = (estilo*)ts;
    free(es->fSize);
    es->fSize = malloc(strlen(fSize)+1);
    strcpy(es->fSize, fSize);
}
