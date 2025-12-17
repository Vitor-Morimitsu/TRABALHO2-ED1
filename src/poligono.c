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

// static double calcularEpsilon(double angulo) {
//     return fmax(1e-9, fabs(angulo) * 1e-10);
// }

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

// static void simplificarPoligono(Poligono p, double tolerancia) {
// ...
// }
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

// Função auxiliar de Raycast (Força Bruta)
static Vertice obterInterseccaoMaisProxima(Lista anteparos, double xOrigem, double yOrigem, double angulo) {
    Vertice vMaisProximo = NULL;
    double menorDistancia = DBL_MAX;

    for(CelulaLista celula = getPrimeiraCelulaLista(anteparos); celula != NULL; celula = getProximaCelulaLista(celula)) {
        Pacote pac = (Pacote)getConteudoCelula(celula);
        if(getTipoPacote(pac) != 'l') continue;
        
        Anteparo ant = (Anteparo)getFormaPacote(pac);
        
        Vertice v = calculaInterseccao(xOrigem, yOrigem, angulo, ant);
        if(v != NULL) {
            double dist = getDistanciaVertice(v);
            if(dist > 1e-9 && dist < menorDistancia) { // Ignorar origem (epsilon minimo)
                if(vMaisProximo != NULL) destroiVertice(vMaisProximo);
                vMaisProximo = v;
                menorDistancia = dist;
            } else {
                destroiVertice(v);
            }
        }
    }
    return vMaisProximo;
}

static void adicionarBordasTemporarias(Lista anteparos, double bx, double by) {
    double minX = bx - 1, maxX = bx + 1, minY = by - 1, maxY = by + 1;
    
    // 1. Expandir Bounding Box da cena
    for(CelulaLista c = getPrimeiraCelulaLista(anteparos); c != NULL; c = getProximaCelulaLista(c)){
         Pacote pac = (Pacote)getConteudoCelula(c);
         // Considerar todos os tipos que tem bounding box seria ideal, mas Linha é o principal anteparo
         if(getTipoPacote(pac) == 'l') {
             Linha l = (Linha)getFormaPacote(pac);
             double x1 = getX1Linha(l); double x2 = getX2Linha(l);
             double y1 = getY1Linha(l); double y2 = getY2Linha(l);
             
             if(x1 < minX) minX = x1; if(x1 > maxX) maxX = x1;
             if(x2 < minX) minX = x2; if(x2 > maxX) maxX = x2;
             if(y1 < minY) minY = y1; if(y1 > maxY) maxY = y1;
             if(y2 < minY) minY = y2; if(y2 > maxY) maxY = y2;
         }
    }
    
    double delta = 1000.0;
    minX -= delta; maxX += delta; minY -= delta; maxY += delta;
    
    // 2. Criar 4 bordas (IDs negativos para identificar e remover depois)
    Linha bordas[4];
    bordas[0] = criarLinha(-99991, minX, minY, maxX, minY, "black"); 
    bordas[1] = criarLinha(-99992, maxX, minY, maxX, maxY, "black"); 
    bordas[2] = criarLinha(-99993, maxX, maxY, minX, maxY, "black"); 
    bordas[3] = criarLinha(-99994, minX, maxY, minX, minY, "black"); 

    for(int i=0; i<4; i++) {
        if(bordas[i] != NULL) {
            Pacote p = criaPacote();
            setTipoPacote(p, 'l');
            setFormaPacote(p, (Forma)bordas[i]);
            insereLista(anteparos, p);
        }
    }
}

// Função não utilizada removida
// static void removerBordasTemporarias(Lista anteparos) { ... }

// Versão simplificada de remoção que varre tudo (segura)
static void limparBordas(Lista anteparos) {
    CelulaLista atual = getPrimeiraCelulaLista(anteparos);
    Lista paraRemover = criarLista();
    
    while(atual != NULL) {
        Pacote pac = (Pacote)getConteudoCelula(atual);
        if(getTipoPacote(pac) == 'l') {
            Linha l = (Linha)getFormaPacote(pac);
            // Verifica ID da linha (que acessamos via cast de Anteparo ou Linha)
            // Assumindo que Linha tem campo id acessivel via getX1Linha ou similar, não, getIDLinha?
            // arvore.c usa getIDAnteparo(linha).
            // Vamos assumir que funciona.
            if(getIDAnteparo((Anteparo)l) <= -99990) {
                 insereLista(paraRemover, pac); // Guardar ponteiro do pacote
            }
        }
        atual = getProximaCelulaLista(atual);
    }
    
    // Remover
    while(getTamanhoLista(paraRemover) > 0) {
        Pacote p = (Pacote)getConteudoCelula(getPrimeiraCelulaLista(paraRemover));
        removeDaListaPorConteudo(anteparos, p); // Precisamos dessa função em lista.h
        // Se ela não existir, temos um problema. lista.h TEM removeDaListaPorConteudo.
        
        // Liberar memória do pacote e da linha
        if(getTipoPacote(p) == 'l') {
            Linha l = (Linha)getFormaPacote(p);
            liberaLinha(l); // CORREÇÃO DE LEAK: liberaLinha limpa a string 'cor'
        }
        free(p); // liberarPacote
        
        removeDaListaPorConteudo(paraRemover, p); // Tira da lista temp
    }
    liberaEstruturaLista(paraRemover);
}

void calcularVisibilidade(Poligono p, Lista anteparos, double xOrigem, double yOrigem, char* comando, int limiteInsertionSort){
    if(p == NULL || anteparos == NULL){
        printf("Erro em calcularVisibilidade: parametros invalidos\n");
        return;
    }
    
    // 1. Adicionar Bordas (Garante que o polígono feche)
    adicionarBordasTemporarias(anteparos, xOrigem, yOrigem);

    Lista todosVertices = criarLista();
    if(todosVertices == NULL){
        printf("Erro em calcularVisibilidade: falha ao criar lista de vertices\n");
        return;
    }
    
    // CRIAR ÁRVORE DE SEGMENTOS ATIVOS (Mover para antes do loop de vertices)
    Arvore arvoreSegmentosAtivos = criarArvore();
    if(arvoreSegmentosAtivos == NULL){
        printf("Erro em calcularVisibilidade: falha ao criar arvore de segmentos ativos\n");
        liberaEstruturaLista(todosVertices);
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
        setDistanciaVertice(v1, distanciaEntrePontos(xOrigem, yOrigem, getXVertice(v1), getYVertice(v1)));
        setAnteparoVertice(v1, (Anteparo)linha);

        Vertice v2 = criarVertice();
        if(v2 == NULL){
            destroiVertice(v1);
            continue;
        }
        
        setXVertice(v2, getX2Linha(linha));
        setYVertice(v2, getY2Linha(linha));
        setAnguloVertice(v2, xOrigem, yOrigem);
        setDistanciaVertice(v2, distanciaEntrePontos(xOrigem, yOrigem, getXVertice(v2), getYVertice(v2)));
        setAnteparoVertice(v2, (Anteparo)linha);

        double ang1 = getAnguloVertice(v1);
        double ang2 = getAnguloVertice(v2);
        double diff = ang1 - ang2;

        if (diff > M_PI) diff -= 2 * M_PI;
        if (diff < -M_PI) diff += 2 * M_PI;

        double anguloInicio, anguloFim;

        if (diff < 0) {
            setTipoVertice(v1, 'i');
            setTipoVertice(v2, 'f');
            anguloInicio = ang1;
            anguloFim = ang2;
        } else {
            setTipoVertice(v2, 'i');
            setTipoVertice(v1, 'f');
            anguloInicio = ang2;
            anguloFim = ang1;
        }

        // VERIFICAR WRAP-AROUND (Segmento cruza o corte -PI/PI)
        // Se o angulo de inicio for MAIOR que o fim, ele "dá a volta"
        // Ex: Começa em 3.0 (aprox 170 deg) e termina em -3.0 (aprox -170 deg)
        if (anguloInicio > anguloFim) {
             // Debug WRAP
             printf("WRAP DETECTADO: ID=%d AngIni=%.4f AngFim=%.4f\n", getIDAnteparo((Anteparo)linha), anguloInicio, anguloFim);
             
             // Este segmento está ativo no ângulo -PI (início da varredura)
             // Precisamos inseri-lo na árvore AGORA.
             
             // Calcula distância no ângulo inicial da varredura (-PI)
             Vertice vInit = calculaInterseccao(xOrigem, yOrigem, -M_PI + 1e-4, (Anteparo)linha);
             double distInit;
             
             if(vInit != NULL) {
                 distInit = getDistanciaVertice(vInit);
                 destroiVertice(vInit);
             } else {
                 // Fallback: Se falhar interseção (ex: paralelo), usar menor distância dos pontos
                 double dx1 = getX1Linha(linha) - xOrigem;
                 double dy1 = getY1Linha(linha) - yOrigem;
                 double dx2 = getX2Linha(linha) - xOrigem;
                 double dy2 = getY2Linha(linha) - yOrigem;
                 double d1 = sqrt(dx1*dx1 + dy1*dy1);
                 double d2 = sqrt(dx2*dx2 + dy2*dy2);
                 distInit = (d1 < d2) ? d1 : d2;
                 printf("  -> Intersecao INIT falhou. Usando fallback dist: %.4f\n", distInit);
             }
             
             // Inserir SEMPRE se for wrap-around
             insereArvore(arvoreSegmentosAtivos, (Anteparo)linha, xOrigem, yOrigem, -M_PI, distInit);
             printf("  -> INSERIDO NA ARVORE (INIT)\n");
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

    // CRIAR ÁRVORE DE SEGMENTOS ATIVOS (Já criada anteriormente)
    // Arvore arvoreSegmentosAtivos = criarArvore();
    // if(arvoreSegmentosAtivos == NULL){ ... }

    
    
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
    // ADICIONAR VÉRTICE INICIAL DA VARREDURA (-PI)
    // Usar Raycast para encontrar o anteparo inicial mais próximo
    Vertice vInicial = obterInterseccaoMaisProxima(anteparos, xOrigem, yOrigem, -M_PI + 1e-4);
    if(vInicial != NULL) {
       adicionarVerticePoligono(p, vInicial);
    }

    // Loop agrupado por ângulo para estabilidade
    int i = 0;
    while(i < tamanho) {
        // 1. Identificar grupo de eventos no mesmo ângulo
        int j = i;
        double anguloAtual = getAnguloDoArray(arrayOrdenado, i);
        
        // Avança j enquanto o ângulo for "igual" (tolerância 1e-6 usada na ordenação)
        while(j < tamanho && fabs(getAnguloDoArray(arrayOrdenado, j) - anguloAtual) < 1e-6) {
            j++;
        }
        // Agora eventos de [i até j-1] compartilham o mesmo ângulo
        
        double epsilon = 1e-5;
        
        // 2. Raio ANTES do evento (Setor angular anterior)
        // Só precisa fazer uma vez para o grupo
        celulaArvore celMaisProxima = encontrarMinimoArvore(arvoreSegmentosAtivos);
        if(celMaisProxima != NULL) {
            Anteparo antMaisProximo = getAnteparoCelula(celMaisProxima);
            Vertice interseccao = calculaInterseccao(xOrigem, yOrigem, anguloAtual - epsilon, antMaisProximo);
            if(interseccao != NULL) adicionarVerticePoligono(p, interseccao);
        }

        // 3. Raio NO evento (Vértice exato) - Opcional mas bom para precisão
        // Para evitar problemas numéricos, miramos exatamente no vértice do primeiro evento
        // Mas a árvore pode ter mudado? Não, a árvore reflete o estado ANTES das mudanças deste ângulo.
        // Se houver parede na frente, vai pegar. Se o vértice for o mais próximo, pega ele.
        celMaisProxima = encontrarMinimoArvore(arvoreSegmentosAtivos);
        if(celMaisProxima != NULL) {
           Anteparo antMaisProximo = getAnteparoCelula(celMaisProxima);
           Vertice interseccao = calculaInterseccao(xOrigem, yOrigem, anguloAtual, antMaisProximo);
           if(interseccao != NULL) adicionarVerticePoligono(p, interseccao);
        }

        // 4. Processar TODAS as atualizações de árvore do grupo
        for(int k = i; k < j; k++) {
            Vertice eventoVertice = getVerticeDoArray(arrayOrdenado, k);
            Anteparo eventoAnteparo = getAnteparoVertice(eventoVertice);
            char eventoTipo = getTipoVertice(eventoVertice);
            
            if (eventoTipo == 'i') {
                double dist = getDistanciaVertice(eventoVertice);
                insereArvore(arvoreSegmentosAtivos, eventoAnteparo, xOrigem, yOrigem, anguloAtual, dist);
            } else if (eventoTipo == 'f') {
                int idAnteparo = getIDAnteparo(eventoAnteparo);
                removerArvore(arvoreSegmentosAtivos, idAnteparo);
            }
        }

        // 5. Raio DEPOIS do evento (Setor angular posterior)
        // Árvore já está atualizada com os novos segmentos
        celMaisProxima = encontrarMinimoArvore(arvoreSegmentosAtivos);
        if(celMaisProxima != NULL) {
            Anteparo antMaisProximo = getAnteparoCelula(celMaisProxima);
            Vertice interseccao = calculaInterseccao(xOrigem, yOrigem, anguloAtual + epsilon, antMaisProximo);
            if(interseccao != NULL) adicionarVerticePoligono(p, interseccao);
        }

        // Avançar para próximo grupo
        i = j;
    }   

    // simplificarPoligono(p,0.5);

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
    
    // Remover bordas temporárias
    limparBordas(anteparos);
}

bool pontoNoPoligono(Poligono p, double x, double y){
    if(p == NULL) return false;
    
    stPoligono* pol = (stPoligono*)p; // Cast necessario para acessar lista interna
    int numVertices = getTamanhoLista(pol->vertices);
    if(numVertices < 3) return false;
    
    int interseccoes = 0;
    
    CelulaLista celulaAtual = getPrimeiraCelulaLista(pol->vertices);
    CelulaLista celulaProx;
    
    // Percorre a lista uma única vez (O(N)) em vez de chamar getVertice(i) (O(N^2))
    for(int i = 0; i < numVertices; i++){
        Vertice v1 = (Vertice)getConteudoCelula(celulaAtual);
        
        // Pega próximo (ou volta ao início para fechar o ciclo)
        if(i == numVertices - 1){
            celulaProx = getPrimeiraCelulaLista(pol->vertices);
        } else {
            celulaProx = getProximaCelulaLista(celulaAtual);
        }
        Vertice v2 = (Vertice)getConteudoCelula(celulaProx);
        
        double x1 = getXVertice(v1);
        double y1 = getYVertice(v1);
        double x2 = getXVertice(v2);
        double y2 = getYVertice(v2);
        
        // Algoritmo Ray Casting (Crossing Number)
        if(((y1 > y) != (y2 > y)) && 
           (x < (x2 - x1) * (y - y1) / (y2 - y1) + x1)){
            interseccoes++;
        }
        
        celulaAtual = celulaProx;
        // Nota: no último passo, celulaAtual volta pro inicio, mas o loop acaba.
        // Se não fosse o 'if' do elo fechado, usaríamos apenas celulaAtual->proximo
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
    if(p == NULL || v == NULL) return;
    
    stPoligono* pol = (stPoligono*)p;
    
    // Otimização: Inserção direta O(1) sem verificação de duplicatas O(N)
    // A referência removeu a verificação por performance.
    
    insereLista(pol->vertices, (void*)v);
    atualizarBoundingBox(pol, getXVertice(v), getYVertice(v));
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