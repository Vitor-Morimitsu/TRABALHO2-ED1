#include "poligono.h"
#include "ordenacao.h"
#include "linha.h"
#include "geometria.h"
#include <math.h>
#include <string.h>

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
    
    if (fabs(cross) < 1e-10) return false;
    
    double t = (segOrigX * segDirY - segOrigY * segDirX) / cross;
    double u = (segOrigX * rayDirY - segOrigY * rayDirX) / cross;
    
    return (t > 1e-10 && u >= -1e-10 && u <= 1.0 + 1e-10);
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

void calcularVisibilidade(Poligono p, Lista anteparos, double xOrigem, double yOrigem, char* comando){
    if(p == NULL || anteparos == NULL){
        printf("Erro em calcularVisibilidade: parametros invalidos\n");
        return;
    }

    Lista todosVertices = criarLista();
    if(todosVertices == NULL){
        printf("Erro em calcularVisibilidade: falha ao criar lista de vertices\n");
        return;
    }
    
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
    if (tamanho == 0) {
        liberaEstruturaLista(todosVertices);
        return;
    }
    
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
        mergeSort(arrayOrdenado, tamanho);
    } else if (ordenacao == 'q') {
        quickSort(arrayOrdenado, 0, tamanho - 1);
    } else {
        insertionSort(arrayOrdenado, tamanho);
    }

    Lista segmentosAtivos = criarLista();
    if(segmentosAtivos == NULL){
        printf("Erro em calcularVisibilidade: falha ao criar lista de segmentos ativos\n");
        free(arrayOrdenado);
        liberarListaDeVertices(todosVertices);
        return;
    }
    
    if (tamanho > 0) {
        double primeiroAngulo = getAnguloVertice(arrayOrdenado[0]);

        for(CelulaLista celula = getPrimeiraCelulaLista(anteparos); 
            celula != NULL; 
            celula = getProximaCelulaLista(celula)) {
            
            Pacote pac = (Pacote)getConteudoCelula(celula);
            if(getTipoPacote(pac) != 'l') continue;
            
            Linha linhaAnteparo = (Linha)getFormaPacote(pac);

            if (segmentoIntersectaRaio(linhaAnteparo, xOrigem, yOrigem, primeiroAngulo)) {
                insereLista(segmentosAtivos, linhaAnteparo);
            }
        }
    }

    for (int i = 0; i < tamanho; i++) {
        No* noAtual = &arrayOrdenado[i];
        Vertice eventoVertice = getVerticeDoArray(arrayOrdenado,i);
        double eventoAngulo = getAnguloDoArray(arrayOrdenado,i);
        Anteparo eventoAnteparo = getAnteparoVertice(eventoVertice);

        double epsilon = calcularEpsilon(eventoAngulo);
        double anguloAntes = eventoAngulo - epsilon;
        
        Vertice v_antes = encontraInterseccaoMaisProxima(segmentosAtivos, xOrigem, yOrigem, anguloAntes);
        if (v_antes != NULL) {
            Anteparo alvo = getAnteparoVertice(v_antes);
            if(alvo!=NULL){
                //******************************************************************************************************** */
            }
            adicionarVerticePoligono(p, v_antes);
        }

        char eventoTipo = getTipoVertice(eventoVertice);
        if (eventoTipo == 'i') {
            insereLista(segmentosAtivos, eventoAnteparo);
        } else if (eventoTipo == 'f') {
            removeDaListaPorConteudo(segmentosAtivos, eventoAnteparo);
        }

        Vertice v_depois = encontraInterseccaoMaisProxima(segmentosAtivos, xOrigem, yOrigem, eventoAngulo);
        if (v_depois != NULL) {
            Anteparo alvo = getAnteparoVertice(v_depois);
            if(alvo != NULL){
                //*********************************************************************************************************** */
            }
            adicionarVerticePoligono(p, v_depois);
        }
    }

    stPoligono* pol = (stPoligono*)p;
    if(getTamanhoLista(pol->vertices) > 0){
        Vertice primeiro = (Vertice)getConteudoCelula(getPrimeiraCelulaLista(pol->vertices));
        Vertice copiaPrimeiro = duplicarVertice(primeiro);
        if(copiaPrimeiro != NULL){
            adicionarVerticePoligono(p, copiaPrimeiro);
        }
    }

    free(arrayOrdenado);
    liberarListaDeVertices(todosVertices);
    liberaEstruturaLista(segmentosAtivos);
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

    Anteparo anteparoNovo = getAnteparoVertice(v);

    CelulaLista ultima = getUltimaCelulaLista(pol->vertices);
    if(ultima != NULL){
        Vertice ultimoVertice = (Vertice)getConteudoCelula(ultima);
        if(getXVertice(ultimoVertice) == x && getYVertice(ultimoVertice) == y){
            Anteparo anteparoUltimo = getAnteparoVertice(ultimoVertice);
            if(anteparoUltimo == anteparoNovo){
                destroiVertice(v);
                return;
            }
        }
    }

    insereLista(pol->vertices, (void*)v);
    
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
    
    // Como é uma lista, precisamos percorrer até o índice
    // (Se fosse um vetor seria acesso direto)
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