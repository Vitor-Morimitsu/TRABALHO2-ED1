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

void calcularVisibilidade(Poligono p, Lista anteparos, Lista formas, double xOrigem, double yOrigem, char ordenacao){
    if(p == NULL || anteparos == NULL || formas == NULL){
        printf("Erro em calcularVisibilidade\n");
        return;
    }
    //calculo da bounding box dos anteparos
    double minX = getMenorValorLista(anteparos,1);
    double minY = getMenorValorLista(anteparos,2);
    double maxX = getMaiorValorLista(anteparos,1);
    double maxY = getMaiorValorLista(anteparos,2);

    gerarBoundingBox(p, &minX,&minY,&maxX, &maxY);

    //id do scan vai ser 1000
    Linha scan = criarLinha(1000,xOrigem,yOrigem,maxX,maxY, NULL);

    Vertice v = calculaInterseccao(anteparos,scan);
    if(v != NULL){
        //ocorreu a intersecção com algum anteparo
        adicionarVerticePoligono(p,v);
    }
    //ver se alguma extremidade de anteparo está dentro do poligono ou intersecta alguma aresta do poligono
    CelulaLista atual = getPrimeiraCelulaLista(anteparos);
    while(atual != NULL){
        Anteparo ant = (Anteparo)getConteudoCelula(atual);
        double x1 = getX1Anteparo(ant);
        double x2 = getX2Anteparo(ant);
        double y1 = getY1Anteparo(ant);
        double y2 = getY2Anteparo(ant);
        


    }



    //ser qualquer vertice está dentro do polígono, se qualquer vertice do polígono está dentro do retangulo, se qualquer aresto do retangulo intersecta qualquer aresta do poligono

    //se centro do circulo está dentro do poligono, se existe alguma aresta do poligono cuja distancia ao centro <= raio, se algum vertice do poligono está dentro do círculo(distancia<= raio) 

}

bool pontoNoPoligono(Poligono p, double x, double y){
    int numVertices = getNumeroVertices(p);
    if(numVertices < 3) return false;
    
    int interseccoes = 0;
    
    for(int i = 0; i < numVertices; i++){
        double x1, y1, x2, y2;
        getVerticePoligono(p, i, &x1, &y1);
        getVerticePoligono(p, (i + 1) % numVertices, &x2, &y2);
        
        // Verifica se o raio horizontal passando por (x,y) intersecta a aresta
        if(((y1 > y) != (y2 > y)) && 
           (x < (x2 - x1) * (y - y1) / (y2 - y1) + x1)){
            interseccoes++;
        }
    }
    
    // Ímpar = dentro, Par = fora
    return (interseccoes % 2) == 1;
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
