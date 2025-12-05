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

void gerarBoundingBox(Poligono p,double *minX, double *minY, double *maxX, double *maxY){
    if(p == NULL){
        printf("Erro em gerarBoundingBox\n");
        return;
    }
    stPoligono* pol =(stPoligono*)p;
    *minX = pol->minX;
    *minY = pol->minY;
    *maxX = pol->maxX;
    *maxY = pol->maxY;
}

void calcularVisibilidade(Poligono p, Lista anteparos, Lista atingidos, double xOrigem, double yOrigem, char ordenacao, int parametro){
    if(p == NULL || anteparos == NULL || atingidos == NULL){
        printf("Erro em calcularVisibilidade\n");
        return;
    }
    double minX = getMenorValorLista(anteparos,1);
    double minY = getMenorValorLista(anteparos,2);
    double maxX = getMaiorValorLista(anteparos,1);
    double maxY = getMaiorValorLista(anteparos,2);

    gerarBoundingBox(p, &minX,&minY,&maxX, &maxY);

    Linha scan = criarLinha(1000,xOrigem,yOrigem,maxX,maxY, NULL);

    Vertice v = calculaInterseccao(anteparos,scan);
    if(v != NULL){
        //ocorreu a intersecção com algum anteparo
        adicionarVerticePoligono(p,v);

    }

}

void getVerticePoligono(Poligono p,int n , double* x, double* y){
    if(p == NULL){
        printf("Erro em getVerticepoligono\n");
        return;
    }
    stPoligono* pol = (stPoligono*)p;

    CelulaLista atual = getPrimeiraCelulaLista(pol->vertices);
    int i = 0;
    while(i< n){
        atual = getProximaCelulaLista(atual);
        i++;
    }
    Vertice v = (Vertice)getConteudoCelula(atual);
    *x = getXVertice(v);
    *y = getYVertice(v);
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
