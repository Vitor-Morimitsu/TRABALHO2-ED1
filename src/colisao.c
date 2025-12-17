#include "colisao.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"
#include "poligono.h"
#include <stdbool.h>
#include <math.h>
#include <string.h> 

bool circuloDentroPoligono(Circulo c, Poligono p) {
    if (c == NULL || p == NULL) return false;
    
    double cx = getCoordXCirculo(c);
    double cy = getCoordYCirculo(c);
    double raio = getRaioCirculo(c);
    
    // 1. Centro dentro do polígono
    if (pontoNoPoligono(p, cx, cy)) return true;

    // 2. Arestas do polígono interceptam o círculo (distância < raio)
    int numVertices = getNumeroVertices(p);
    for (int i = 0; i < numVertices; i++) {
        double x1, y1, x2, y2;
        getVerticePoligono(p, i, &x1, &y1);
        getVerticePoligono(p, (i + 1) % numVertices, &x2, &y2);
        
        if (distanciaPontoSegmento(cx, cy, x1, y1, x2, y2) <= raio) return true;
        
        // 3. Vértices do polígono dentro do círculo
        if(distanciaEntrePontos(cx, cy, x1, y1) <= raio) return true;
    }
    
    return false;
}

bool retanguloDentroPoligono(Retangulo r, Poligono p) {
    if (r == NULL || p == NULL) return false;
    
    double rx = getCoordXRetangulo(r);
    double ry = getCoordYRetangulo(r);
    double w = getWRetangulo(r);
    double h = getHRetangulo(r);
    
    // Vértices do retângulo
    double rectPts[4][2] = {
        {rx, ry}, {rx + w, ry}, {rx + w, ry + h}, {rx, ry + h}
    };
    
    // 1. Algum vértice do retângulo dentro do polígono
    for (int i = 0; i < 4; i++) {
        if (pontoNoPoligono(p, rectPts[i][0], rectPts[i][1])) return true;
    }
    
    // 2. Algum vértice do polígono dentro do retângulo
    int n = getNumeroVertices(p);
    for (int i = 0; i < n; i++) {
        double px, py;
        getVerticePoligono(p, i, &px, &py);
        if (px >= rx && px <= rx + w && py >= ry && py <= ry + h) return true;
    }
    
    // 3. Interseção de arestas
    for (int i = 0; i < n; i++) {
        double px1, py1, px2, py2;
        getVerticePoligono(p, i, &px1, &py1);
        getVerticePoligono(p, (i + 1) % n, &px2, &py2);
        
        for (int j = 0; j < 4; j++) {
            double rx1 = rectPts[j][0];
            double ry1 = rectPts[j][1];
            double rx2 = rectPts[(j + 1) % 4][0];
            double ry2 = rectPts[(j + 1) % 4][1];
            
            if (segmentoIntersectaSegmento(px1, py1, px2, py2, rx1, ry1, rx2, ry2)) return true;
        }
    }
    
    return false;
}

bool linhaDentroPoligono(Linha l, Poligono p) {
    if (l == NULL || p == NULL) return false;
    
    double x1 = getX1Linha(l);
    double y1 = getY1Linha(l);
    double x2 = getX2Linha(l);
    double y2 = getY2Linha(l);
    
    // 1. Pontos extremos dentro
    if (pontoNoPoligono(p, x1, y1)) return true;
    if (pontoNoPoligono(p, x2, y2)) return true;
    
    // 2. Interseção com arestas do polígono
    int n = getNumeroVertices(p);
    for (int i = 0; i < n; i++) {
        double px1, py1, px2, py2;
        getVerticePoligono(p, i, &px1, &py1);
        getVerticePoligono(p, (i + 1) % n, &px2, &py2);
        
        if (segmentoIntersectaSegmento(x1, y1, x2, y2, px1, py1, px2, py2)) return true;
    }
    
    return false;
}

bool textoDentroPoligono(Texto t, Poligono p) {
    if (t == NULL || p == NULL) return false;
    
    double x = getCoordXTexto(t);
    double y = getCoordYTexto(t);
    char ancora = getATexto(t);
    const char* txt = getTxtoTexto(t);
    int len = (txt != NULL) ? strlen(txt) : 0;
    
    double x1, x2;
    if(ancora == 'i') {
        x1 = x; x2 = x + 10.0 * len;
    } else if(ancora == 'f') {
        x1 = x - 10.0 * len; x2 = x;
    } else { 
        x1 = x - 10.0 * len / 2.0; x2 = x + 10.0 * len / 2.0;
    }
    
    // Tratar texto como linha (base sublinhada)
    // 1. Pontos extremos
    if (pontoNoPoligono(p, x1, y)) return true;
    if (pontoNoPoligono(p, x2, y)) return true;
    
    // 2. Interseção
    int n = getNumeroVertices(p);
    for (int i = 0; i < n; i++) {
        double px1, py1, px2, py2;
        getVerticePoligono(p, i, &px1, &py1);
        getVerticePoligono(p, (i + 1) % n, &px2, &py2);
        
        if (segmentoIntersectaSegmento(x1, y, x2, y, px1, py1, px2, py2)) return true;
    }
    
    return false;
}

bool formaDentroPoligono(Pacote pac, Poligono p) {
    if (pac == NULL || p == NULL) {
        return false;
    }
    
    char tipo = getTipoPacote(pac);
    Forma forma = getFormaPacote(pac);
    
    switch (tipo) {
        case 'c':
            return circuloDentroPoligono((Circulo)forma, p);
        case 'r':
            return retanguloDentroPoligono((Retangulo)forma, p);
        case 'l':
            return linhaDentroPoligono((Linha)forma, p);
        case 't':
            return textoDentroPoligono((Texto)forma, p);
        default:
            return false;
    }
}

Lista obterFormasAtingidas(Lista formas, Poligono p) {
    if (formas == NULL || p == NULL) {
        printf("Erro em obterFormasAtingidas: parametros invalidos\n");
        return NULL;
    }
    
    Lista formasAtingidas = criarLista();
    if(formasAtingidas == NULL) {
        printf("Erro ao criar lista de formas atingidas\n");
        return NULL;
    }
    
    for (CelulaLista celula = getPrimeiraCelulaLista(formas); 
         celula != NULL; 
         celula = getProximaCelulaLista(celula)) {
        
        Pacote pac = (Pacote)getConteudoCelula(celula);
        
        if (formaDentroPoligono(pac, p)) {
            insereLista(formasAtingidas, pac);
        }
    }
    
    return formasAtingidas;
}


bool atingidaPelaExplosao(Lista formas, Poligono p) {
    if (formas == NULL || p == NULL) {
        return false;
    }
    
    for (CelulaLista celula = getPrimeiraCelulaLista(formas); 
         celula != NULL; 
         celula = getProximaCelulaLista(celula)) {
        
        Pacote pac = (Pacote)getConteudoCelula(celula);
        
        if (formaDentroPoligono(pac, p)) {
            return true;  
        }
    }
    
    return false; 
}