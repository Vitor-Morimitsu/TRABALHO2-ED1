// teste_geometria.c

#include "geometria.h"
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

int testesPassaram = 0;
int testosFalharam = 0;

void verificar(const char* nome, bool condicao) {
    if(condicao) {
        printf("✅ PASS: %s\n", nome);
        testesPassaram++;
    } else {
        printf("❌ FAIL: %s\n", nome);
        testosFalharam++;
    }
}

void testarPontoEmPoligono() {
    printf("\n=== TESTANDO pontoEmPoligono ===\n");
    
    // Criar quadrado: (0,0), (10,0), (10,10), (0,10)
    Lista vertices = criarLista();
    
    Vertice v1 = criarVertice();
    setXVertice(v1, 0.0); 
    setYVertice(v1, 0.0);
    insereLista(vertices, v1);
    
    Vertice v2 = criarVertice();
    setXVertice(v2, 10.0); 
    setYVertice(v2, 0.0);
    insereLista(vertices, v2);
    
    Vertice v3 = criarVertice();
    setXVertice(v3, 10.0); 
    setYVertice(v3, 10.0);
    insereLista(vertices, v3);
    
    Vertice v4 = criarVertice();
    setXVertice(v4, 0.0); 
    setYVertice(v4, 10.0);
    insereLista(vertices, v4);
    
    // Testes
    verificar("Ponto (5,5) está dentro do quadrado", 
              pontoEmPoligono(5.0, 5.0, vertices) == true);
    
    verificar("Ponto (15,5) está fora do quadrado", 
              pontoEmPoligono(15.0, 5.0, vertices) == false);
    
    verificar("Ponto (-5,5) está fora do quadrado", 
              pontoEmPoligono(-5.0, 5.0, vertices) == false);
    
    verificar("Ponto (5,15) está fora do quadrado", 
              pontoEmPoligono(5.0, 15.0, vertices) == false);
    
    verificar("Ponto (0,0) no vértice está dentro", 
              pontoEmPoligono(0.0, 0.0, vertices) == true);
    
    verificar("Ponto (5,0) na borda está dentro", 
              pontoEmPoligono(5.0, 0.0, vertices) == true);
    
    liberaEstruturaLista(vertices);
}

void testarSegmentoIntersecta() {
    printf("\n=== TESTANDO segmentoIntersectaSegmento ===\n");
    
    // Teste 1: Segmentos que se cruzam no meio (X)
    verificar("Diagonais se cruzam", 
              segmentoIntersectaSegmento(0, 0, 10, 10,0, 10, 10, 0)== true);
    
    // Teste 2: Segmentos paralelos (não se tocam)
    verificar("Segmentos paralelos não se cruzam", 
              segmentoIntersectaSegmento(0, 0, 10, 0,    // Horizontal y=0
                                        0, 5, 10, 5)     // Horizontal y=5
              == false);
    
    // Teste 3: Segmentos perpendiculares que não se tocam
    verificar("Segmentos perpendiculares distantes não se cruzam", 
              segmentoIntersectaSegmento(0, 0, 5, 0,     // Horizontal curto
                                        10, -5, 10, 5)   // Vertical à direita
              == false);
    
    // Teste 4: Segmentos que se tocam na extremidade
    verificar("Segmentos que se tocam na extremidade", 
              segmentoIntersectaSegmento(0, 0, 5, 0,     // Horizontal
                                        5, 0, 5, 5)      // Vertical a partir do fim
              == true);
    
    // Teste 5: Segmentos perpendiculares que se cruzam (T)
    verificar("Segmentos em T se cruzam", 
              segmentoIntersectaSegmento(0, 0, 10, 0,    // Horizontal
                                        5, -5, 5, 5)     // Vertical pelo meio
              == true);
}

void testarDistanciaPontoSegmento() {
    printf("\n=== TESTANDO distanciaPontoSegmento ===\n");
    
    // Teste 1: Ponto acima do meio de segmento horizontal
    double dist1 = distanciaPontoSegmento(5.0, 5.0,      // Ponto
                                         0.0, 0.0, 10.0, 0.0); // Segmento
    verificar("Distância de (5,5) ao segmento horizontal é 5.0", 
              fabs(dist1 - 5.0) < 0.01);
    printf("   → Distância calculada: %.2f\n", dist1);
    
    // Teste 2: Ponto à direita da extremidade
    double dist2 = distanciaPontoSegmento(15.0, 0.0,     // Ponto à direita
                                         0.0, 0.0, 10.0, 0.0); // Segmento
    verificar("Distância de (15,0) ao segmento é 5.0", 
              fabs(dist2 - 5.0) < 0.01);
    printf("   → Distância calculada: %.2f\n", dist2);
    
    // Teste 3: Ponto à esquerda da extremidade
    double dist3 = distanciaPontoSegmento(-5.0, 0.0,     // Ponto à esquerda
                                         0.0, 0.0, 10.0, 0.0); // Segmento
    verificar("Distância de (-5,0) ao segmento é 5.0", 
              fabs(dist3 - 5.0) < 0.01);
    printf("   → Distância calculada: %.2f\n", dist3);
    
    // Teste 4: Ponto sobre o segmento
    double dist4 = distanciaPontoSegmento(5.0, 0.0,      // Ponto no meio
                                         0.0, 0.0, 10.0, 0.0); // Segmento
    verificar("Distância de (5,0) ao segmento é 0.0", 
              fabs(dist4 - 0.0) < 0.01);
    printf("   → Distância calculada: %.2f\n", dist4);
    
    // Teste 5: Segmento diagonal
    double dist5 = distanciaPontoSegmento(5.0, 5.0,      // Ponto
                                         0.0, 0.0, 10.0, 10.0); // Diagonal
    double distEsperada = sqrt(2) * 5.0 / 2.0; // Distância perpendicular à diagonal
    verificar("Distância ao segmento diagonal está correta", 
              fabs(dist5 - distEsperada) < 0.1);
    printf("   → Distância calculada: %.2f (esperado: %.2f)\n", dist5, distEsperada);
}

int main() {
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║   TESTES DE GEOMETRIA.C                ║\n");
    printf("╚════════════════════════════════════════╝\n");
    
    testarPontoEmPoligono();
    testarSegmentoIntersecta();
    testarDistanciaPontoSegmento();
    
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║   RESULTADO FINAL                      ║\n");
    printf("╠════════════════════════════════════════╣\n");
    printf("║   ✅ Testes passaram: %2d               ║\n", testesPassaram);
    printf("║   ❌ Testes falharam: %2d               ║\n", testosFalharam);
    printf("╚════════════════════════════════════════╝\n");
    
    if(testosFalharam == 0) {
        printf("\n🎉 TODOS OS TESTES PASSARAM!\n\n");
        return 0;
    } else {
        printf("\n⚠️  ALGUNS TESTES FALHARAM!\n\n");
        return 1;
    }
}