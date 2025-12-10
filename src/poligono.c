#include "poligono.h"
#include "ordenacao.h"
#include "linha.h"
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct NO{
    Vertice ver;
    double angulo;
}stNo;

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

void calcularVisibilidade(Poligono p, Lista anteparos, Lista /*formas*/, Arvore arvoreSegmentos, double xOrigem, double yOrigem, char* comando){
    if(p == NULL || anteparos == NULL || arvoreSegmentos == NULL){
        printf("Erro em calcularVisibilidade: parametros invalidos\n");
        return;
    }

    //gerar todos os vértices dos anteparos, calcular seus ângulos e definir o tipo (início/fim)
    Lista todosVertices = criarLista();
    
    for(CelulaLista celula = getPrimeiraCelulaLista(anteparos); celula != NULL; celula = getProximaCelulaLista(celula)) {
        Pacote pac = (Pacote)getConteudoCelula(celula);
        char tipo = getTipoPacote(pac);
        
        if(tipo != 'l') continue; 
        
        Linha linha = (Linha)getFormaPacote(pac);
        
        Vertice v1 = criarVertice();
        setXVertice(v1, getX1Linha(linha));
        setYVertice(v1, getY1Linha(linha));
        setAnguloVertice(v1, xOrigem, yOrigem);
        setAnteparoVertice(v1, (Anteparo)linha);

        Vertice v2 = criarVertice();
        setXVertice(v2, getX2Linha(linha));
        setYVertice(v2, getY2Linha(linha));
        setAnguloVertice(v2, xOrigem, yOrigem);
        setAnteparoVertice(v2, (Anteparo)linha);

        //determina qual vértice é 'inicio' e qual é 'fim'
        double ang1 = getAnguloVertice(v1);
        double ang2 = getAnguloVertice(v2);
        double diff = ang1 - ang2;

        //normaliza a diferença para o intervalo [-PI, PI]
        if (diff > M_PI) diff -= 2 * M_PI;
        if (diff < -M_PI) diff += 2 * M_PI;

        if (diff < 0) {
            setTipoVertice(v1, 'i'); //ang1 < ang2, v1 é início
            setTipoVertice(v2, 'f');
        } else {
            setTipoVertice(v2, 'i'); //ang2 < ang1, v2 é início
            setTipoVertice(v1, 'f');
        }

        insereLista(todosVertices, v1);
        insereLista(todosVertices, v2);
    }

    //ordenar os vértices por ângulo
    int tamanho = getTamanhoLista(todosVertices);
    if (tamanho == 0) {
        liberaLista(todosVertices);
        return; //nenhum vértice para processar
    }
    
    No* arrayOrdenado = gerarArray(todosVertices, tamanho, xOrigem, yOrigem);
    
    //determina o tipo de ordenação baseado no comando
    char ordenacao = 'i'; // padrão insertion sort
    if(comando != NULL && strlen(comando) > 0) {
        if(strcmp(comando, "m") == 0) {
            ordenacao = 'm';
        } else if(strcmp(comando, "q") == 0) {
            ordenacao = 'q';
        }
    }
    
    if (ordenacao == 'm') {
        mergeSort(arrayOrdenado, tamanho);
    } else if (ordenacao == 'q') {
        insertionSort(arrayOrdenado, tamanho); // Pode implementar quicksort se necessário
    } else {
        insertionSort(arrayOrdenado, tamanho);
    }

    //algoritmo de Varredura (Sweep-line) usando Árvore
    //usa a árvore passada como parâmetro para manter segmentos ativos ordenados
    Arvore segmentosAtivos = arvoreSegmentos;
    double far_dist = 10000.0; //distância longa para projeção do raio

    stNo* nos = (stNo*)arrayOrdenado;

    for (int i = 0; i < tamanho; i++) {
        stNo* noAtual = &nos[i];
        Vertice eventoVertice = noAtual->ver;
        double eventoAngulo = noAtual->angulo;
        Anteparo eventoAnteparo = getAnteparoVertice(eventoVertice);

        //atualiza a origem e ângulo atual na árvore para cálculo de distâncias
        setOrigemArvore(segmentosAtivos, xOrigem, yOrigem, eventoAngulo);

        //raio um pouco antes do ângulo do evento 
        double anguloAntes = eventoAngulo - 0.0001;
        setOrigemArvore(segmentosAtivos, xOrigem, yOrigem, anguloAntes);
        
        //busca o segmento mais próximo (mais à esquerda na árvore)
        Linha linhaProxima = (Linha)buscaCelulaArvore(segmentosAtivos);
        if (linhaProxima != NULL) {
            double x_far_antes = xOrigem + far_dist * cos(anguloAntes);
            double y_far_antes = yOrigem + far_dist * sin(anguloAntes);
            Linha ray_antes = criarLinha(0, xOrigem, yOrigem, x_far_antes, y_far_antes, "blue");
            
            Vertice v_antes = encontraInterseccaoMaisProxima(segmentosAtivos, ray_antes);
            if (v_antes != NULL) {
                adicionarVerticePoligono(p, v_antes);
            }
            liberaLinha(ray_antes);
        }

        //atualiza a árvore de segmentos ativos 
        char eventoTipo = getTipoVertice(eventoVertice);
        if (eventoTipo == 'i') {
            //adiciona o anteparo (linha) aos segmentos ativos
            setOrigemArvore(segmentosAtivos, xOrigem, yOrigem, eventoAngulo);
            insereArvore(segmentosAtivos, eventoAnteparo);
        } else if (eventoTipo == 'f') {
            //remove o anteparo dos segmentos ativos
            Linha linhaRemover = (Linha)eventoAnteparo;
            int idRemover = getIDLinha(linhaRemover);
            removeArvore(segmentosAtivos, idRemover);
        }

        //raio exatamente no ângulo do evento 
        setOrigemArvore(segmentosAtivos, xOrigem, yOrigem, eventoAngulo);
        
        Linha linhaProximaDepois = (Linha)buscaCelulaArvore(segmentosAtivos);
        if (linhaProximaDepois != NULL) {
            double x_far_depois = xOrigem + far_dist * cos(eventoAngulo);
            double y_far_depois = yOrigem + far_dist * sin(eventoAngulo);
            Linha ray_depois = criarLinha(0, xOrigem, yOrigem, x_far_depois, y_far_depois, "blue");

            Vertice v_depois = encontraInterseccaoMaisProxima(segmentosAtivos, ray_depois);
            if (v_depois != NULL) {
                adicionarVerticePoligono(p, v_depois);
            }
            liberaLinha(ray_depois);
        }
    }

    //libera a memória do array usado para ordenação
    if(arrayOrdenado != NULL) {
        free(arrayOrdenado);
    }

    CelulaLista celula = getPrimeiraCelulaLista(todosVertices);
    while(celula != NULL) {
        Vertice v = (Vertice)getConteudoCelula(celula);
        destroiVertice(v);
        celula = getProximaCelulaLista(celula);
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
