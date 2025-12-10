#include "colisao.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"
#include <stdbool.h>

bool circuloDentroPoligono(Circulo c, Poligono p) {
    if (c == NULL || p == NULL) {
        return false;
    }
    
    double x = getCoordXCirculo(c);
    double y = getCoordYCirculo(c);
    
    return pontoNoPoligono(p, x, y);
}

bool retanguloDentroPoligono(Retangulo r, Poligono p) {
    if (r == NULL || p == NULL) {
        return false;
    }
    
    double x = getCoordXRetangulo(r);
    double y = getCoordYRetangulo(r);
    double w = getWRetangulo(r);
    double h = getHRetangulo(r);
    
    // Verifica o centro do retângulo
    double centroX = x + w / 2.0;
    double centroY = y + h / 2.0;
    
    return pontoNoPoligono(p, centroX, centroY);
}

bool linhaDentroPoligono(Linha l, Poligono p) {
    if (l == NULL || p == NULL) {
        return false;
    }
    
    double x1 = getX1Linha(l);
    double y1 = getY1Linha(l);
    double x2 = getX2Linha(l);
    double y2 = getY2Linha(l);
    
    // Verifica o ponto médio da linha
    double meioX = (x1 + x2) / 2.0;
    double meioY = (y1 + y2) / 2.0;
    
    return pontoNoPoligono(p, meioX, meioY);
}

bool textoDentroPoligono(Texto t, Poligono p) {
    if (t == NULL || p == NULL) {
        return false;
    }
    
    double x = getCoordXTexto(t);
    double y = getCoordYTexto(t);
    
    return pontoNoPoligono(p, x, y);
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

bool atingidaPelaExplosao(Lista formas, Poligono p, double /*xBomba*/, double /*yBomba*/) {
    if (formas == NULL || p == NULL) {
        printf("Erro em atingidaPelaExplosao: parametros invalidos\n");
        return false;
    }
    
    bool algumFormaAtingida = false;
    
    // Itera por todas as formas
    for (CelulaLista celula = getPrimeiraCelulaLista(formas); celula != NULL; celula = getProximaCelulaLista(celula)) {
        Pacote pac = (Pacote)getConteudoCelula(celula);
        
        if (formaDentroPoligono(pac, p)) {
            algumFormaAtingida = true;
            
            // Opcional: imprimir informações sobre a forma atingida
            char tipo = getTipoPacote(pac);
            int id = getIDPacote(pac);
            
            printf("Forma atingida - Tipo: %c, ID: %d\n", tipo, id);
        }
    }
    
    return algumFormaAtingida;
}

Lista obterFormasAtingidas(Lista formas, Poligono p) {
    if (formas == NULL || p == NULL) {
        printf("Erro em obterFormasAtingidas: parametros invalidos\n");
        return NULL;
    }
    
    Lista formasAtingidas = criarLista();
    
    // Itera por todas as formas
    for (CelulaLista celula = getPrimeiraCelulaLista(formas); celula != NULL; celula = getProximaCelulaLista(celula)) {
        Pacote pac = (Pacote)getConteudoCelula(celula);
        
        if (formaDentroPoligono(pac, p)) {
            // Adiciona a forma à lista de atingidas (apenas referência, não cópia)
            insereLista(formasAtingidas, pac);
        }
    }
    
    return formasAtingidas;
}
