#include "arvore.h"
#include "linha.h"
#include "geometria.h"
#include <math.h>

typedef struct celula{
    void* conteudo;
    double distancia; // Distância do segmento à origem
    struct celula* esquerda;
    struct celula* direita;
}stCelulaArvore;

typedef struct{
    stCelulaArvore* raiz;
    int tamanho;
    double xOrigem;
    double yOrigem;
    double anguloAtual;
}stArvore;

// Função auxiliar para calcular a distância de um segmento à origem em um dado ângulo
static double calcularDistanciaSegmento(Linha linha, double xOrigem, double yOrigem, double angulo) {
    double x1 = getX1Linha(linha);
    double y1 = getY1Linha(linha);
    double x2 = getX2Linha(linha);
    double y2 = getY2Linha(linha);
    
    // Cria um raio na direção do ângulo
    double far_dist = 10000.0;
    double x_far = xOrigem + far_dist * cos(angulo);
    double y_far = yOrigem + far_dist * sin(angulo);
    
    // Calcula interseção do raio com o segmento
    double denom = (xOrigem - x_far) * (y1 - y2) - (yOrigem - y_far) * (x1 - x2);
    
    if (fabs(denom) < 1e-9) {
        return INFINITY; // Paralelo
    }
    
    double t = ((xOrigem - x1) * (y1 - y2) - (yOrigem - y1) * (x1 - x2)) / denom;
    double u = -((xOrigem - x_far) * (yOrigem - y1) - (yOrigem - y_far) * (xOrigem - x1)) / denom;
    
    if (t > 0 && t <= 1 && u >= 0 && u <= 1) {
        double xIntersec = xOrigem + t * (x_far - xOrigem);
        double yIntersec = yOrigem + t * (y_far - yOrigem);
        return distanciaEntrePontos(xOrigem, yOrigem, xIntersec, yIntersec);
    }
    
    return INFINITY;
}

// Função auxiliar recursiva para inserir na árvore
static stCelulaArvore* inserirRecursivo(stCelulaArvore* raiz, stCelulaArvore* novaCelula) {
    if (raiz == NULL) {
        return novaCelula;
    }
    
    // Menor distância vai para a esquerda
    if (novaCelula->distancia < raiz->distancia) {
        raiz->esquerda = inserirRecursivo(raiz->esquerda, novaCelula);
    } else {
        raiz->direita = inserirRecursivo(raiz->direita, novaCelula);
    }
    
    return raiz;
}

// Função auxiliar para encontrar o nó mais à esquerda (menor distância)
static stCelulaArvore* encontrarMinimo(stCelulaArvore* raiz) {
    while (raiz != NULL && raiz->esquerda != NULL) {
        raiz = raiz->esquerda;
    }
    return raiz;
}

// Função auxiliar recursiva para remover da árvore
static stCelulaArvore* removerRecursivo(stCelulaArvore* raiz, int id) {
    if (raiz == NULL) {
        return NULL;
    }
    
    Linha linha = (Linha)raiz->conteudo;
    int idAtual = getIDLinha(linha);
    
    if (id == idAtual) {
        // Caso 1: Nó folha
        if (raiz->esquerda == NULL && raiz->direita == NULL) {
            free(raiz);
            return NULL;
        }
        // Caso 2: Apenas filho direito
        if (raiz->esquerda == NULL) {
            stCelulaArvore* temp = raiz->direita;
            free(raiz);
            return temp;
        }
        // Caso 3: Apenas filho esquerdo
        if (raiz->direita == NULL) {
            stCelulaArvore* temp = raiz->esquerda;
            free(raiz);
            return temp;
        }
        // Caso 4: Dois filhos - substitui pelo sucessor (menor da direita)
        stCelulaArvore* sucessor = encontrarMinimo(raiz->direita);
        raiz->conteudo = sucessor->conteudo;
        raiz->distancia = sucessor->distancia;
        raiz->direita = removerRecursivo(raiz->direita, getIDLinha((Linha)sucessor->conteudo));
        return raiz;
    }
    
    // Busca recursiva
    raiz->esquerda = removerRecursivo(raiz->esquerda, id);
    raiz->direita = removerRecursivo(raiz->direita, id);
    
    return raiz;
}

Arvore criaArvore(){
    stArvore* arvore = malloc(sizeof(stArvore));
    if(arvore == NULL){
        printf("Erro ao criarArvore.\n");
        return NULL;
    }
    
    arvore->raiz = NULL;
    arvore->tamanho = 0;
    arvore->xOrigem = 0.0;
    arvore->yOrigem = 0.0;
    arvore->anguloAtual = 0.0;

    return (Arvore)arvore;
}

void insereArvore(Arvore arv, void* segmentoAtivo){
    if(arv == NULL || segmentoAtivo == NULL){
        printf("Erro em insereArvore\n");
        return;
    }

    stArvore* arvore = (stArvore*)arv;
    Linha linha = (Linha)segmentoAtivo;
    
    stCelulaArvore* novaCelula = malloc(sizeof(stCelulaArvore));
    if(novaCelula == NULL) {
        printf("Erro ao alocar espaço para a nova célula da árvore\n");
        return;
    }
    
    novaCelula->conteudo = segmentoAtivo;
    novaCelula->distancia = calcularDistanciaSegmento(linha, arvore->xOrigem, arvore->yOrigem, arvore->anguloAtual);
    novaCelula->esquerda = NULL;
    novaCelula->direita = NULL;
    
    arvore->raiz = inserirRecursivo(arvore->raiz, novaCelula);
    arvore->tamanho++;
}

CelulaArvore buscaCelulaArvore(Arvore arv){
    if(arv == NULL){
        printf("Erro em buscaCelulaArvore\n");
        return NULL;
    }
    
    stArvore* arvore = (stArvore*)arv;
    
    // Retorna o nó mais à esquerda (menor distância)
    stCelulaArvore* minimo = encontrarMinimo(arvore->raiz);
    
    if(minimo != NULL) {
        return (CelulaArvore)minimo->conteudo;
    }
    
    return NULL;
}

void removeArvore(Arvore arv, int id){
    if(arv == NULL){
        printf("Erro em removeArvore\n");
        return;
    }
    
    stArvore* arvore = (stArvore*)arv;
    arvore->raiz = removerRecursivo(arvore->raiz, id);
    if(arvore->tamanho > 0) {
        arvore->tamanho--;
    }
}

void setOrigemArvore(Arvore arv, double xOrigem, double yOrigem, double anguloAtual){
    if(arv == NULL){
        printf("Erro em setOrigemArvore\n");
        return;
    }
    
    stArvore* arvore = (stArvore*)arv;
    arvore->xOrigem = xOrigem;
    arvore->yOrigem = yOrigem;
    arvore->anguloAtual = anguloAtual;
}

// Função auxiliar recursiva para liberar todos os nós
static void liberarNosRecursivo(stCelulaArvore* raiz) {
    if (raiz == NULL) {
        return;
    }
    
    liberarNosRecursivo(raiz->esquerda);
    liberarNosRecursivo(raiz->direita);
    free(raiz);
}

void liberarArvore(Arvore arv){
    if(arv == NULL){
        return;
    }
    
    stArvore* arvore = (stArvore*)arv;
    liberarNosRecursivo(arvore->raiz);
    free(arvore);
}