#include "poligono.h"
#include "ordenacao.h"
#include "linha.h"
#include "geometria.h"
#include "arvore.h"      
#include "anteparo.h"    
#include <math.h>
#include <string.h>
#include <float.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct poligono{
    Lista vertices;
    double minX, minY, maxX, maxY;
}stPoligono;

static bool segmentoIntersectaRaio(Linha linha, double xOrigem, double yOrigem, double angulo) {
    double x1 = getX1Linha(linha);
    double y1 = getY1Linha(linha);
    double x2 = getX2Linha(linha);
    double y2 = getY2Linha(linha);
    
    double rayDirX = cos(angulo);
    double rayDirY = sin(angulo);
    
    double segDirX = x2 - x1;
    double segDirY = y2 - y1;
    double segOrigX = x1 - xOrigem;
    double segOrigY = y1 - yOrigem;
    
    double cross = rayDirX * segDirY - rayDirY * segDirX;
    
    if (fabs(cross) < 1e-10) {
        return false;
    }
    
    double t = (segOrigX * segDirY - segOrigY * segDirX) / cross;
    double u = (segOrigX * rayDirY - segOrigY * rayDirX) / cross;
    
    // ✅ CORREÇÃO: Verificar AMBOS t e u
    // t > 0: raio vai para frente
    // -0.1 <= u <= 1.1: interseção dentro (ou perto) do segmento
    return (t > 1e-6 && u >= -0.1 && u <= 1.1);
}

static Vertice duplicarVertice(Vertice original) {
    if (original == NULL) return NULL;
    
    Vertice copia = criarVertice();
    if (copia == NULL) return NULL;
    
    setXVertice(copia, getXVertice(original));
    setYVertice(copia, getYVertice(original));
    
    return copia;
}

static void liberarListaDeVertices(Lista lista) {
    if (lista == NULL) return;
    
    for(CelulaLista celula = getPrimeiraCelulaLista(lista); 
        celula != NULL; 
        celula = getProximaCelulaLista(celula)) {
        Vertice v = (Vertice)getConteudoCelula(celula);
        destroiVertice(v);
    }
    
    liberaEstruturaLista(lista);
}

static void atualizarBoundingBox(stPoligono* pol, double x, double y) {
    if (x < pol->minX) pol->minX = x;
    if (x > pol->maxX) pol->maxX = x;
    if (y < pol->minY) pol->minY = y;
    if (y > pol->maxY) pol->maxY = y;
}

static double calcularEpsilon(double angulo) {
    return fmax(1e-9, fabs(angulo) * 1e-10);
}

Poligono criarPoligono(){
    stPoligono* p = (stPoligono*)malloc(sizeof(stPoligono));
    if(p == NULL){
        printf("Erro em criarPoligono: falha na alocacao de memoria\n");
        return NULL;
    }
    
    p->minX = DBL_MAX;
    p->minY = DBL_MAX;
    p->maxX = -DBL_MAX;
    p->maxY = -DBL_MAX;
    p->vertices = criarLista();
    
    if(p->vertices == NULL){
        free(p);
        printf("Erro em criarPoligono: falha ao criar lista de vertices\n");
        return NULL;
    }
    
    return (Poligono)p;
}

static void simplificarPoligono(Poligono p, double tolerancia) {
    if(p == NULL) return;
    
    stPoligono* pol = (stPoligono*)p;
    int n = getTamanhoLista(pol->vertices);
    
    if(n < 3) return;
    
    printf("DEBUG: Simplificando polígono com %d vértices\n", n);
    
    // Criar array com todos os vértices
    Vertice* vertices = malloc(n * sizeof(Vertice));
    if(vertices == NULL) return;
    
    int idx = 0;
    for(CelulaLista celula = getPrimeiraCelulaLista(pol->vertices);
        celula != NULL;
        celula = getProximaCelulaLista(celula)) {
        vertices[idx++] = (Vertice)getConteudoCelula(celula);
    }
    
    // Marcar vértices a manter
    bool* manter = malloc(n * sizeof(bool));
    for(int i = 0; i < n; i++) manter[i] = false;
    
    // Sempre manter primeiro vértice
    manter[0] = true;
    
    // Para cada vértice, verificar se está longe o suficiente do anterior mantido
    int ultimoMantido = 0;
    for(int i = 1; i < n; i++) {
        double x1 = getXVertice(vertices[ultimoMantido]);
        double y1 = getYVertice(vertices[ultimoMantido]);
        double x2 = getXVertice(vertices[i]);
        double y2 = getYVertice(vertices[i]);
        
        double dist = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
        
        // Calcular ângulo também
        double dx1 = x1 - getXVertice(vertices[(ultimoMantido - 1 + n) % n]);
        double dy1 = y1 - getYVertice(vertices[(ultimoMantido - 1 + n) % n]);
        double dx2 = x2 - x1;
        double dy2 = y2 - y1;
        
        double cross = dx1 * dy2 - dy1 * dx2;
        
        // Manter se: distância grande OU mudança de direção significativa
        if(dist > tolerancia || fabs(cross) > 100.0) {
            manter[i] = true;
            ultimoMantido = i;
            printf("  Mantendo V%d (%.2f, %.2f) - dist=%.2f cross=%.2f\n", 
                   i, x2, y2, dist, cross);
        } else {
            printf("  Removendo V%d (%.2f, %.2f) - dist=%.2f cross=%.2f\n", 
                   i, x2, y2, dist, cross);
        }
    }
    
    // Criar nova lista com vértices mantidos
    Lista novaLista = criarLista();
    for(int i = 0; i < n; i++) {
        if(manter[i]) {
            insereLista(novaLista, vertices[i]);
        } else {
            destroiVertice(vertices[i]);
        }
    }
    
    // Substituir lista
    liberaEstruturaLista(pol->vertices);
    pol->vertices = novaLista;
    
    free(vertices);
    free(manter);
    
    printf("DEBUG: Polígono simplificado: %d vértices\n", getTamanhoLista(pol->vertices));
}
void gerarBoundingBox(Poligono p, double *minX, double *minY, double *maxX, double *maxY){
    if(p == NULL || minX == NULL || minY == NULL || maxX == NULL || maxY == NULL){
        printf("Erro em gerarBoundingBox: parametros invalidos\n");
        return;
    }
    
    stPoligono* pol = (stPoligono*)p;
    *minX = pol->minX;
    *minY = pol->minY;
    *maxX = pol->maxX;
    *maxY = pol->maxY;
}

void calcularVisibilidade(Poligono p, Lista anteparos, double xOrigem, double yOrigem, char* comando, int limiteInsertionSort){
    if(p == NULL || anteparos == NULL){
        printf("Erro em calcularVisibilidade: parametros invalidos\n");
        return;
    }

    Lista todosVertices = criarLista();
    if(todosVertices == NULL){
        printf("Erro em calcularVisibilidade: falha ao criar lista de vertices\n");
        return;
    }
    
    // COLETAR VÉRTICES DOS ANTEPAROS
    for(CelulaLista celula = getPrimeiraCelulaLista(anteparos);celula != NULL;celula = getProximaCelulaLista(celula)) {
        Pacote pac = (Pacote)getConteudoCelula(celula);
        char tipo = getTipoPacote(pac);
        
        if(tipo != 'l') continue; 
        
        Linha linha = (Linha)getFormaPacote(pac);
        
        Vertice v1 = criarVertice();
        if(v1 == NULL) continue;
        
        setXVertice(v1, getX1Linha(linha));
        setYVertice(v1, getY1Linha(linha));
        setAnguloVertice(v1, xOrigem, yOrigem);
        setAnteparoVertice(v1, (Anteparo)linha);

        Vertice v2 = criarVertice();
        if(v2 == NULL){
            destroiVertice(v1);
            continue;
        }
        
        setXVertice(v2, getX2Linha(linha));
        setYVertice(v2, getY2Linha(linha));
        setAnguloVertice(v2, xOrigem, yOrigem);
        setAnteparoVertice(v2, (Anteparo)linha);

        double ang1 = getAnguloVertice(v1);
        double ang2 = getAnguloVertice(v2);
        double diff = ang1 - ang2;

        if (diff > M_PI) diff -= 2 * M_PI;
        if (diff < -M_PI) diff += 2 * M_PI;

        if (diff < 0) {
            setTipoVertice(v1, 'i');
            setTipoVertice(v2, 'f');
        } else {
            setTipoVertice(v2, 'i');
            setTipoVertice(v1, 'f');
        }

        insereLista(todosVertices, v1);
        insereLista(todosVertices, v2);
    }

    
    int tamanho = getTamanhoLista(todosVertices);
    printf("DEBUG: Lista de anteparos tem %d elementos\n", getTamanhoLista(anteparos));
    printf("DEBUG: Coletados %d vértices de eventos\n", tamanho);   
    if (tamanho == 0) {
        liberaEstruturaLista(todosVertices);
        return;
    }
    
    // ORDENAR VÉRTICES POR ÂNGULO
    No* arrayOrdenado = gerarArray(todosVertices, tamanho);
    if(arrayOrdenado == NULL){
        printf("Erro em calcularVisibilidade: falha ao gerar array de vertices\n");
        liberarListaDeVertices(todosVertices);
        return;
    }
    
    char ordenacao = 'q';
    if(comando != NULL && strlen(comando) > 0) {
        if(strcmp(comando, "m") == 0) ordenacao = 'm';
        else if(strcmp(comando, "q") == 0) ordenacao = 'q';
    }
    
    if (ordenacao == 'm') {
        mergeSort(arrayOrdenado, tamanho, limiteInsertionSort);
    } else if (ordenacao == 'q') {
        quickSort(arrayOrdenado, 0, tamanho - 1);
    } else {
        insertionSort(arrayOrdenado, tamanho);
    }

    // CRIAR ÁRVORE DE SEGMENTOS ATIVOS
    Arvore arvoreSegmentosAtivos = criarArvore();
    if(arvoreSegmentosAtivos == NULL){
        printf("Erro em calcularVisibilidade: falha ao criar arvore de segmentos ativos\n");
        free(arrayOrdenado);
        liberarListaDeVertices(todosVertices);
        return;
    }
    
    
    // // INICIALIZAR ÁRVORE - inserir cada anteparo no seu próprio ângulo
    // if (tamanho > 0) {
    //     printf("DEBUG: Inserindo anteparos nas posições corretas\n");

    //     for(CelulaLista celula = getPrimeiraCelulaLista(anteparos);celula != NULL;celula = getProximaCelulaLista(celula)) {
    //         Pacote pac = (Pacote)getConteudoCelula(celula);
    //         if(getTipoPacote(pac) != 'l') continue;
            
    //         Linha linhaAnteparo = (Linha)getFormaPacote(pac);
    //         Anteparo ant = (Anteparo)linhaAnteparo;

    //         double x1 = getX1Linha(linhaAnteparo);
    //         double y1 = getY1Linha(linhaAnteparo);
            
    //         // ✅ Calcular ângulo DO PONTO MÉDIO do anteparo
    //         double xMid = (x1 + getX2Linha(linhaAnteparo)) / 2.0;
    //         double yMid = (y1 + getY2Linha(linhaAnteparo)) / 2.0;
            
    //         double anguloMid = atan2(yMid - yOrigem, xMid - xOrigem);
    //         double distMid = sqrt((xMid - xOrigem)*(xMid - xOrigem) + 
    //                             (yMid - yOrigem)*(yMid - yOrigem));
            
    //         insereArvore(arvoreSegmentosAtivos, ant, xOrigem, yOrigem, anguloMid, distMid);
    //         printf("  Anteparo (%.2f,%.2f)-(%.2f,%.2f) inserido com ângulo=%.4f dist=%.2f\n", 
    //             x1, y1, getX2Linha(linhaAnteparo), getY2Linha(linhaAnteparo),
    //             anguloMid, distMid);
    //     }
    // }

    for (int i = 0; i < tamanho; i++) {
        Vertice eventoVertice = getVerticeDoArray(arrayOrdenado, i);
        double eventoAngulo = getAnguloDoArray(arrayOrdenado, i);
        Anteparo eventoAnteparo = getAnteparoVertice(eventoVertice);
        char eventoTipo = getTipoVertice(eventoVertice);

        double epsilon = calcularEpsilon(eventoAngulo);
        double anguloAntes = eventoAngulo - epsilon;
        
        celulaArvore celMaisProxima = encontrarMinimoArvore(arvoreSegmentosAtivos);
        
        if(celMaisProxima != NULL) {
            Anteparo antMaisProximo = getAnteparoCelula(celMaisProxima);
            Linha linAnt = (Linha)antMaisProximo;
            printf("  Evento %d (ângulo=%.4f tipo=%c): Anteparo mais próximo (%.2f,%.2f)-(%.2f,%.2f)\n",i, eventoAngulo, eventoTipo,getX1Linha(linAnt), getY1Linha(linAnt),getX2Linha(linAnt), getY2Linha(linAnt));
            
            Vertice interseccaoAntes = calculaInterseccao(xOrigem, yOrigem, anguloAntes, antMaisProximo);
            if(interseccaoAntes != NULL) {
                adicionarVerticePoligono(p, interseccaoAntes);
            }
        }

        if (eventoTipo == 'i') {
            double dist = getDistanciaVertice(eventoVertice);
            insereArvore(arvoreSegmentosAtivos, eventoAnteparo, xOrigem, yOrigem, eventoAngulo, dist);
            
        } else if (eventoTipo == 'f') {
            int idAnteparo = getIDAnteparo(eventoAnteparo);
            double dist = getDistanciaVertice(eventoVertice);
            removerArvore(arvoreSegmentosAtivos, idAnteparo, xOrigem, yOrigem, eventoAngulo, dist);
            
            double anguloDepois = eventoAngulo + epsilon;
            celMaisProxima = encontrarMinimoArvore(arvoreSegmentosAtivos);
            
            if(celMaisProxima != NULL) {
                Anteparo antMaisProximo = getAnteparoCelula(celMaisProxima);
                Vertice interseccaoDepois = calculaInterseccao(xOrigem, yOrigem, anguloDepois, antMaisProximo);
                
                if(interseccaoDepois != NULL) {
                    adicionarVerticePoligono(p, interseccaoDepois);
                }
            }
        }
    }   

    simplificarPoligono(p,0.5);

    // FECHAR POLÍGONO (adicionar cópia do primeiro vértice no final)
    stPoligono* pol = (stPoligono*)p;
    if(getTamanhoLista(pol->vertices) > 0){
        Vertice primeiro = (Vertice)getConteudoCelula(getPrimeiraCelulaLista(pol->vertices));
        Vertice copiaPrimeiro = duplicarVertice(primeiro);
        if(copiaPrimeiro != NULL){
            adicionarVerticePoligono(p, copiaPrimeiro);
        }
    }

    // LIMPEZA
    liberarArvore(arvoreSegmentosAtivos);
    free(arrayOrdenado);
    liberarListaDeVertices(todosVertices);
}

bool pontoNoPoligono(Poligono p, double x, double y){
    if(p == NULL) return false;
    
    int numVertices = getNumeroVertices(p);
    if(numVertices < 3) return false;
    
    int interseccoes = 0;
    
    for(int i = 0; i < numVertices; i++){
        double x1, y1, x2, y2;
        getVerticePoligono(p, i, &x1, &y1);
        getVerticePoligono(p, (i + 1) % numVertices, &x2, &y2);
        
        if(((y1 > y) != (y2 > y)) && 
           (x < (x2 - x1) * (y - y1) / (y2 - y1) + x1)){
            interseccoes++;
        }
    }
    
    return (interseccoes % 2) == 1;
}

void getVerticePoligono(Poligono p, int n, double* x, double* y){
    if(p == NULL || x == NULL || y == NULL){
        printf("Erro em getVerticePoligono: parametros invalidos\n");
        return;
    }
    
    stPoligono* pol = (stPoligono*)p;
    
    if(n < 0 || n >= getTamanhoLista(pol->vertices)){
        printf("Erro em getVerticePoligono: indice fora dos limites\n");
        return;
    }

    CelulaLista atual = getPrimeiraCelulaLista(pol->vertices);
    for(int i = 0; i < n && atual != NULL; i++){
        atual = getProximaCelulaLista(atual);
    }
    
    if(atual == NULL){
        printf("Erro em getVerticePoligono: celula nao encontrada\n");
        return;
    }
    
    Vertice v = (Vertice)getConteudoCelula(atual);
    *x = getXVertice(v);
    *y = getYVertice(v);
}

void adicionarVerticePoligono(Poligono p, Vertice v){
    if(p == NULL || v == NULL){
        printf("Erro em adicionarVerticePoligono: parametros invalidos\n");
        return;
    }
    
    stPoligono* pol = (stPoligono*)p;
    
    double x = getXVertice(v);
    double y = getYVertice(v);

    // ✅ DEBUG: Imprimir vértice sendo testado
    printf("DEBUG ADD: Testando vértice (%.2f, %.2f)\n", x, y);

    // Verificar duplicata
    for(CelulaLista celula = getPrimeiraCelulaLista(pol->vertices);
        celula != NULL;
        celula = getProximaCelulaLista(celula)) {
        
        Vertice verticeExistente = (Vertice)getConteudoCelula(celula);
        double xExist = getXVertice(verticeExistente);
        double yExist = getYVertice(verticeExistente);
        
        double dist = sqrt((x - xExist)*(x - xExist) + (y - yExist)*(y - yExist));
        
        // ✅ Tolerância de 2.0 pixels
        if(dist < 2.0){
            printf("  → REJEITADO! Muito próximo de (%.2f, %.2f) - distância: %.4f\n", 
                   xExist, yExist, dist);
            destroiVertice(v);
            return;
        }
    }

    insereLista(pol->vertices, (void*)v);
    printf("  → ACEITO! Total de vértices: %d\n", getTamanhoLista(pol->vertices));
    
    atualizarBoundingBox(pol, x, y);
}

int getNumeroVertices(Poligono p){
    if(p == NULL){
        printf("Erro em getNumeroVertices: poligono nulo\n");
        return 0;
    }
    
    stPoligono* pol = (stPoligono*)p;
    return getTamanhoLista(pol->vertices);
}

void liberarPoligono(Poligono p){
    if(p == NULL) return;

    stPoligono* pol = (stPoligono*)p;

    liberarListaDeVertices(pol->vertices);
    
    free(pol);
}

Anteparo getAnteparoPoligono(Poligono p, int indice) {
    if (p == NULL) return NULL;
    stPoligono* pol = (stPoligono*)p;
    
    int count = 0;
    for (CelulaLista c = getPrimeiraCelulaLista(pol->vertices); c != NULL; c = getProximaCelulaLista(c)) {
        if (count == indice) {
            Vertice v = (Vertice)getConteudoCelula(c);
            return getAnteparoVertice(v);
        }
        count++;
    }
    return NULL;
}