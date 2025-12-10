#include "poligono.h"
#include "ordenacao.h"

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
    if(p == NULL || anteparos == NULL){
        printf("Erro em calcularVisibilidade: parametros invalidos\n");
        return;
    }

    // Etapa 1: Extrair todos os vértices dos anteparos e calcular seus ângulos
    Lista todosVertices = criarLista();
    for(CelulaLista celula = getPrimeiraCelulaLista(anteparos); celula != NULL; celula = getProximaCelulaLista(celula)) {
        Anteparo ant = getConteudoCelula(celula);
        Vertice v1 = criarVertice();
        setXVertice(v1, getX1Anteparo(ant));
        setYVertice(v1, getY1Anteparo(ant));
        setAnguloVertice(v1, xOrigem, yOrigem);
        setAnteparoVertice(v1, ant);
        insereLista(todosVertices, v1);

        Vertice v2 = criarVertice();
        setXVertice(v2, getX2Anteparo(ant));
        setYVertice(v2, getY2Anteparo(ant));
        setAnguloVertice(v2, xOrigem, yOrigem);
        setAnteparoVertice(v2, ant);
        insereLista(todosVertices, v2);
    }

    // Etapa 2: Ordenar os vértices por ângulo
    int tamanho = getTamanhoLista(todosVertices);
    No* arrayOrdenado = gerarArray(todosVertices, tamanho, xOrigem, yOrigem);
    
    if (ordenacao == 'm') {
        mergeSort(arrayOrdenado, tamanho);
    } else { // Assume 'q' ou qualquer outro para insertionSort
        insertionSort(arrayOrdenado, tamanho);
    }

    // Etapa 3: Algoritmo de Varredura (Sweep-line) (a ser implementado)
    // O 'arrayOrdenado' agora contém os vértices ordenados por ângulo.
    // O próximo passo usará este array.

    // Etapa 4: Construir o polígono final (a ser implementado)

    // Libera a memória do array usado para ordenação
    if(arrayOrdenado != NULL) {
        free(arrayOrdenado);
    }

    // Libera a memória da lista temporária de vértices, já que não será mais usada
    for(CelulaLista celula = getPrimeiraCelulaLista(todosVertices); celula != NULL; celula = getProximaCelulaLista(celula)) {
        Vertice v = getConteudoCelula(celula);
        destroiVertice(v);
    }
    liberaLista(todosVertices);
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
        return;
    }

    stPoligono* pol = (stPoligono*)p;

    // Libera cada vértice armazenado na lista
    for(CelulaLista celula = getPrimeiraCelulaLista(pol->vertices); celula != NULL; celula = getProximaCelulaLista(celula)) {
        Vertice v = getConteudoCelula(celula);
        destroiVertice(v);
    }
    
    liberaLista(pol->vertices);
    free(pol);
}
