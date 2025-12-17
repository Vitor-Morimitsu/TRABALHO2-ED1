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

double calculoDeCanto(double bx, double by, double angulo, Anteparo an){
    double anguloFuturo = angulo + 0.0001; 
    Vertice v = calculaInterseccao(bx, by, anguloFuturo, an);
    if (v != NULL){
        double dist = getDistanciaVertice(v);
        destroiVertice(v);
        return dist;
    }
    return 999999999.0;
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

    double dist_novoVertice;
    if(novoVertice != NULL) {
        dist_novoVertice = getDistanciaVertice(novoVertice);
        destroiVertice(novoVertice);
    } else {
        double x1 = getX1Linha((Linha)celArv->anteparo);
        double y1 = getY1Linha((Linha)celArv->anteparo);
        double x2 = getX2Linha((Linha)celArv->anteparo);
        double y2 = getY2Linha((Linha)celArv->anteparo);
        double d1 = sqrt(pow(x1-bx,2) + pow(y1-by,2));
        double d2 = sqrt(pow(x2-bx,2) + pow(y2-by,2));
        dist_novoVertice = (d1 < d2) ? d1 : d2;
    }

    // Modificado para usar calculoDeCanto em caso de empate
    if(fabs(distAnteparo - dist_novoVertice) < 1e-4){ 
        double distFuturaAnteparo = calculoDeCanto(bx, by, angulo, an);
        double distFuturaNo = calculoDeCanto(bx, by, angulo, celArv->anteparo);

        if(distFuturaAnteparo < distFuturaNo){
            celArv->esquerda = insereRecursivo(celArv->esquerda,an,bx,by,angulo,distAnteparo);
        } else {
            celArv->direita = insereRecursivo(celArv->direita,an, bx,by,angulo, distAnteparo);
        }
    }
    else if(distAnteparo < dist_novoVertice){
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

celulaArvore encontrarMinimo(celulaArvore no){
    if(no == NULL){
        return NULL;
    }
    stCelulaArvore* arvore = (stCelulaArvore*) no;
    while(arvore->esquerda != NULL){
        arvore = arvore->esquerda;
    }
    return arvore;
}

celulaArvore encontrarMinimoArvore(Arvore arv){
    if(arv == NULL) return NULL;
    
    stArvore* arvore = (stArvore*)arv;
    
    if(arvore->raiz == NULL) return NULL;
    
    return encontrarMinimo(arvore->raiz);
}

stCelulaArvore* removerNoEncontrado(stCelulaArvore* alvo) {
    if(alvo->esquerda == NULL){
        stCelulaArvore* temp = alvo->direita;
        free(alvo);
        return temp;
    }else if(alvo->direita == NULL){
        stCelulaArvore* temp = alvo->esquerda;
        free(alvo);
        return temp;
    }
    
    stCelulaArvore* paiSucessor = alvo;
    stCelulaArvore* sucessor = alvo->direita;
    
    while(sucessor->esquerda != NULL){
        paiSucessor = sucessor;
        sucessor = sucessor->esquerda;
    }
    
    alvo->anteparo = sucessor->anteparo;
    
    if(paiSucessor == alvo){
        paiSucessor->direita = removerNoEncontrado(sucessor);
    } else {
        paiSucessor->esquerda = removerNoEncontrado(sucessor);
    }
    
    return alvo;
}

stCelulaArvore* removerRecursivoID(stCelulaArvore* cel, int id, int* removido){
    if(cel == NULL) return NULL;

    stCelulaArvore* celArv = (stCelulaArvore*)cel;
    int idAtual = getIDAnteparo(celArv->anteparo);

    if(id == idAtual){
        *removido = 1;
        return removerNoEncontrado(celArv);
    }
    
    int removidoEsq = 0;
    celArv->esquerda = removerRecursivoID(celArv->esquerda, id, &removidoEsq);
    if(removidoEsq) {
        *removido = 1;
        return celArv;
    }
    
    int removidoDir = 0;
    celArv->direita = removerRecursivoID(celArv->direita, id, &removidoDir);
    if(removidoDir) {
        *removido = 1;
        return celArv;
    }

    return celArv;
}

void removerArvore(Arvore arv, int id){
    if(arv == NULL){
        printf("Erro em removeArvore\n");
        return;
    }
    
    stArvore* arvore = (stArvore*)arv;
    int flag = 0;
    arvore->raiz = removerRecursivoID(arvore->raiz, id, &flag);
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

Anteparo getAnteparoCelula(celulaArvore cel){
    if(cel == NULL){
        printf("Erro em getAnteparoCelula: célula NULL\n");
        return NULL;
    }
    
    stCelulaArvore* celula = (stCelulaArvore*)cel;
    return celula->anteparo;
}