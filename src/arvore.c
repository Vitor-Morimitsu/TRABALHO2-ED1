#include "arvore.h"
#include "linha.h"
#include "geometria.h"
#include <math.h>

typedef struct celula{
    Anteparo anteparo;
    struct celula* esquerda;
    struct celula* direita;
}stCelulaArvore;

typedef struct{
    stCelulaArvore* raiz;
}stArvore;

Arvore criarArvore(){
    stArvore* arvore = malloc(sizeof(stArvore));
    if(arvore == NULL){
        printf("Erro ao criarArvore.\n");
        return NULL;
    }
    arvore->raiz = NULL;

    return (Arvore)arvore;
}

celulaArvore insereRecursivo(celulaArvore cel,Anteparo an, double bx,double by,double angulo, double distAnteparo){
    if(cel == NULL){
        //célula vazia, nova célula deve ser criada.
        stCelulaArvore* novaArvore = malloc(sizeof(stCelulaArvore));
        if(novaArvore == NULL){
            printf("Erro ao alocar novaArvore em insereRecursivo\n");
            return NULL;
        }
        novaArvore->anteparo = an;
        novaArvore->direita = NULL;
        novaArvore->esquerda = NULL;
        return (celulaArvore)novaArvore;
    }
    //árvore não vazia
    stCelulaArvore* celArv =(stCelulaArvore*)cel;

    Vertice novoVertice = calculaInterseccao(bx,by,angulo,celArv->anteparo);

    //usar valor grande p indicar que não há intersecção
    double dist_novoVertice = (novoVertice != NULL) ? getDistanciaVertice(novoVertice) : 999999999.0; 
    if(novoVertice != NULL){
        //erro ao criar o novo vertice
        destroiVertice(novoVertice);
    }

    if(distAnteparo < dist_novoVertice){
        celArv->esquerda = insereRecursivo(celArv->esquerda,an,bx,by,angulo,distAnteparo);
    }else{
        celArv->direita = insereRecursivo(celArv->direita,an, bx,by,angulo, distAnteparo);
    }
    return celArv;
}

void insereArvore(Arvore arv, Anteparo an, double bx, double by, double angulo,double distCelAnterior){
    if(arv == NULL ){
        printf("Erro em insereArvore\n");
        return;
    }
    stArvore* a = (stArvore*)arv;
    a->raiz = insereRecursivo(a->raiz,an,bx,by,angulo,distCelAnterior);    
}

celulaArvore encontrarMinimo(stCelulaArvore* cel){
    if(cel == NULL) return NULL;
    while(cel->esquerda != NULL){
        cel = cel->esquerda;
    }
    return cel;
}

celulaArvore removerRecursivo(celulaArvore cel, int id){
    if(cel == NULL) return NULL;

    stCelulaArvore* celArv = (stCelulaArvore*)cel;

    int idAtual = getIDAnteparo(celArv->anteparo);

    if(id<idAtual){
        celArv->esquerda = removerRecursivo(celArv->esquerda, id);
    }else if (id>idAtual){
        celArv->direita = removerRecursivo(celArv->direita, id);
    }else{
        //achou a celula
        if(celArv->esquerda == NULL){
            stCelulaArvore* temp = celArv->direita;
            free(celArv);
            return temp;
        }else if(celArv->direita == NULL){
            stCelulaArvore* temp = celArv->esquerda;
            free(celArv);
            return temp;
        }
        stCelulaArvore* sucessor = encontrarMinimo(celArv->direita);
        celArv->anteparo = sucessor->anteparo;
        celArv->direita = removerRecursivo(celArv->direita, getIDAnteparo(sucessor->anteparo));
    }
    return cel;
}

void removerArvore(Arvore arv, int id){
    if(arv == NULL){
        printf("Erro em removeArvore\n");
        return;
    }
    
    stArvore* arvore = (stArvore*)arv;
    arvore->raiz = removerRecursivo(arvore->raiz, id);

}

void liberarNosRecursivo(stCelulaArvore* cel){
    if(cel == NULL) return;
    liberarNosRecursivo(cel->esquerda);
    liberarNosRecursivo(cel->direita);
    free(cel);
}

void liberarArvore(Arvore arv){
    if(arv == NULL){
        return;
    }
    
    stArvore* arvore = (stArvore*)arv;
    liberarNosRecursivo(arvore->raiz);
    free(arvore);
}