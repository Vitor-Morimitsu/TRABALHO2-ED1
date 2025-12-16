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

// Função auxiliar com flag de sucesso
celulaArvore removerRecursivoComFlag(celulaArvore cel, int id, double bx, double by, double angulo, double distAnteparo, int* removido){
    if(cel == NULL) {
        *removido = 0;
        return NULL;
    }

    stCelulaArvore* celArv = (stCelulaArvore*)cel;

    // Verificar se é o nó alvo
    int idAtual = getIDAnteparo(celArv->anteparo);
    if(id == idAtual){
        *removido = 1;
        // Lógica de remoção
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
        // Recalcular para remover sucessor
        Vertice vSuc = calculaInterseccao(bx, by, angulo, sucessor->anteparo);
        double distSuc = (vSuc != NULL) ? getDistanciaVertice(vSuc) : 999999999.0;
        if(vSuc != NULL) destroiVertice(vSuc);
        
        celArv->direita = removerRecursivoComFlag(celArv->direita, getIDAnteparo(sucessor->anteparo), bx, by, angulo, distSuc, removido);
        return cel;
    }

    // Decisão de caminho
    Vertice v = calculaInterseccao(bx, by, angulo, celArv->anteparo);
    double distAtual;
    if (v != NULL) {
        distAtual = getDistanciaVertice(v);
        destroiVertice(v);
    } else {
        distAtual = 999999999.0;
    }

    // Tentar caminho geométrico primeiro
    if(distAnteparo < distAtual){
        celArv->esquerda = removerRecursivoComFlag(celArv->esquerda, id, bx, by, angulo, distAnteparo, removido);
        if(*removido) return cel;
        
        // Se não achou na esquerda, tenta direita (backtracking para corrigir erro numérico)
        celArv->direita = removerRecursivoComFlag(celArv->direita, id, bx, by, angulo, distAnteparo, removido);
    } else {
        celArv->direita = removerRecursivoComFlag(celArv->direita, id, bx, by, angulo, distAnteparo, removido);
        if(*removido) return cel;
        
        // Se não achou na direita, tenta esquerda
        celArv->esquerda = removerRecursivoComFlag(celArv->esquerda, id, bx, by, angulo, distAnteparo, removido);
    }

    return cel;
}

celulaArvore removerRecursivo(celulaArvore cel, int id, double bx, double by, double angulo, double distAnteparo){
    int removido = 0;
    return removerRecursivoComFlag(cel, id, bx, by, angulo, distAnteparo, &removido);
}

void removerArvore(Arvore arv, int id, double bx, double by, double angulo, double distAnteparo){
    if(arv == NULL){
        printf("Erro em removeArvore\n");
        return;
    }
    
    stArvore* arvore = (stArvore*)arv;
    arvore->raiz = removerRecursivo(arvore->raiz, id, bx, by, angulo, distAnteparo);

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