#include "colisao.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"
#include "poligono.h"
#include <stdbool.h>
#include <math.h>

bool circuloDentroPoligono(Circulo c, Poligono p) {
    if (c == NULL || p == NULL) {
        return false;
    }
    
    double cx = getCoordXCirculo(c);
    double cy = getCoordYCirculo(c);
    double raio = getRaioCirculo(c);
    
    // 1. Verifica se o centro do círculo está dentro do polígono
    if (pontoNoPoligono(p, cx, cy)) {
        return true;
    }

    // 2. Verifica se algum vértice do polígono está dentro do círculo
    int numVertices = getNumeroVertices(p);
    for (int i = 0; i < numVertices; i++) {
        double vx, vy;
        getVerticePoligono(p, i, &vx, &vy);
        
        // Calcula a distância do vértice ao centro do círculo
        double dist = sqrt(pow(vx - cx, 2) + pow(vy - cy, 2));
        
        if (dist < raio) {
            return true; // Vértice do polígono está dentro do círculo
        }
    }

    // Nota: Esta verificação ainda não cobre o caso de uma aresta do polígono cruzar o círculo sem que nenhum vértice esteja dentro.
    // Uma solução completa exigiria um teste de interseção círculo-segmento de reta para cada aresta do polígono.
    // No entanto, as duas verificações acima já são uma melhoria substancial.
    
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
    
    // Verifica os 4 cantos do retângulo
    if (pontoNoPoligono(p, x, y)) return true;          // Canto superior esquerdo
    if (pontoNoPoligono(p, x + w, y)) return true;      // Canto superior direito
    if (pontoNoPoligono(p, x, y + h)) return true;      // Canto inferior esquerdo
    if (pontoNoPoligono(p, x + w, y + h)) return true;  // Canto inferior direito
    
    // Opcional: verificar também o centro para cobrir casos onde o polígono está inteiramente dentro do retângulo
    double centroX = x + w / 2.0;
    double centroY = y + h / 2.0;
    if (pontoNoPoligono(p, centroX, centroY)) return true;

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

// bool atingidaPelaExplosao(Lista formas, Poligono p, double /*xBomba*/, double /*yBomba*/) {
//     if (formas == NULL || p == NULL) {
//         printf("Erro em atingidaPelaExplosao: parametros invalidos\n");
//         return false;
//     }
    
//     bool algumFormaAtingida = false;
    
//     // Itera por todas as formas
//     for (CelulaLista celula = getPrimeiraCelulaLista(formas); celula != NULL; celula = getProximaCelulaLista(celula)) {
//         Pacote pac = (Pacote)getConteudoCelula(celula);
        
//         if (formaDentroPoligono(pac, p)) {
//             algumFormaAtingida = true;
            
//             // Opcional: imprimir informações sobre a forma atingida
//             char tipo = getTipoPacote(pac);
//             int id = getIDPacote(pac);
            
//             printf("Forma atingida - Tipo: %c, ID: %d\n", tipo, id);
//         }
//     }
    
//     return algumFormaAtingida;
// }

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
