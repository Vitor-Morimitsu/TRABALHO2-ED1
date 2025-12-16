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
    if (c == NULL || p == NULL) {
        return false;
    }
    
    double cx = getCoordXCirculo(c);
    double cy = getCoordYCirculo(c);
    double raio = getRaioCirculo(c);
    
    if (pontoNoPoligono(p, cx, cy)) {
        return true;
    }

    int numVertices = getNumeroVertices(p);
    for (int i = 0; i < numVertices; i++) {
        double vx, vy;
        getVerticePoligono(p, i, &vx, &vy);
        
        double dist = sqrt(pow(vx - cx, 2) + pow(vy - cy, 2));
        
        if (dist < raio) {
            return true; 
        }
    }
    
    return false;
}

bool retanguloDentroPoligono(Retangulo r, Poligono p) {
    if (r == NULL || p == NULL) {
        return false;
    }
    
    double x = getCoordXRetangulo(r);
    double y = getCoordYRetangulo(r);
    double w = getWRetangulo(r);
    double h = getHRetangulo(r);
    
    if (pontoNoPoligono(p, x, y)) return true;         
    if (pontoNoPoligono(p, x + w, y)) return true;      
    if (pontoNoPoligono(p, x, y + h)) return true;      
    if (pontoNoPoligono(p, x + w, y + h)) return true; 
    
    return false;
}

bool linhaDentroPoligono(Linha l, Poligono p) {
    if (l == NULL || p == NULL) {
        return false;
    }
    
    double x1 = getX1Linha(l);
    double y1 = getY1Linha(l);
    double x2 = getX2Linha(l);
    double y2 = getY2Linha(l);
    
    if (pontoNoPoligono(p, x1, y1)) return true;
    if (pontoNoPoligono(p, x2, y2)) return true;
    
    double meioX = (x1 + x2) / 2.0;
    double meioY = (y1 + y2) / 2.0;
    if (pontoNoPoligono(p, meioX, meioY)) return true;
    
    return false;
}

bool textoDentroPoligono(Texto t, Poligono p) {
    if (t == NULL || p == NULL) {
        return false;
    }
    
    double x = getCoordXTexto(t);
    double y = getCoordYTexto(t);
    
    char ancora = getATexto(t);
    const char* texto = getTxtoTexto(t);
    int len = (texto != NULL) ? strlen(texto) : 0;
    
    double x1, x2;
    if(ancora == 'i') {
        x1 = x;
        x2 = x + 10.0 * len;
    } else if(ancora == 'f') {
        x1 = x - 10.0 * len;
        x2 = x;
    } else { // 'm'
        x1 = x - 10.0 * len / 2.0;
        x2 = x + 10.0 * len / 2.0;
    }
    
    //testar extremidades do texto
    if (pontoNoPoligono(p, x1, y)) return true;
    if (pontoNoPoligono(p, x2, y)) return true;
    if (pontoNoPoligono(p, x, y)) return true;  
    
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


bool atingidaPelaExplosao(Lista formas, Poligono p, double xBomba, double yBomba) {
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