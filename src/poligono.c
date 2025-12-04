#include "poligono.h"

typedef struct poligono{
    Lista vertices;
    double minX, minY,maxX,maxY;
}stPoligono;

Poligono criarPoligono(){
    stPoligono* p = (stPoligono*)malloc(sizeof(stPoligono));
    if(p == NULL){
        printf("Erro em criarPoligono\n");
        return NULL;
    }
    p->minX = DBL_MAX;
    p->minY = DBL_MAX;
    p->maxX = -DBL_MAX;
    p->maxY = -DBL_MAX;
    p->vertices = criarLista();
    return (Poligono)p;
}

void calcularVisibilidade(Poligono p, Lista anteparos, Lista atingidos, double xOrigem, double yOrigem, char ordenacao, int parametro){
    if(p == NULL || anteparos == NULL || atingidos == NULL){
        printf("Erro em calcularVisibilidade\n");
        return;
    }
    //--------------------------TERMINAR
}

void adicionarVerticePoligono(Poligono p, Vertice v){
    if(p == NULL || v == NULL){
        printf("Erro em adicionarVerticePoligono\n");
        return;
    }
    stPoligono* pol = (stPoligono*)p;
    insereLista(pol->vertices, (void*)v);
}

int getNumeroVertices(Poligono p){
    if(p == NULL){
        printf("Erro em getNumeroVertices\n");
    }
    stPoligono* pol = (stPoligono*)p;
    int tamanho = getTamanhoLista(pol->vertices);
    return tamanho;
}

void liberarPoligono(Poligono p){
    if(p == NULL){
        printf("Erro em liberarPoligono\n");
        return;
    }

    stPoligono* pol = (stPoligono*)p;
    liberaLista(pol->vertices);
    free(pol);
}
