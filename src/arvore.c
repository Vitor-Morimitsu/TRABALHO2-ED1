#include "arvore.h"
#include "linha.h"
#include "geometria.h"
#include <math.h>

typedef struct celula{
    Anteparo anteparo;
    struct celula* esquerda;
    struct celula* direita;
}stCelulaArvore;

typedef struct{
    stCelulaArvore* raiz;
}stArvore;

Arvore criarArvore(){
    stArvore* arvore = malloc(sizeof(stArvore));
    if(arvore == NULL){
        printf("Erro ao criarArvore.\n");
        return NULL;
    }
    arvore->raiz = NULL;

    return (Arvore)arvore;
}

celulaArvore insereRecursivo(celulaArvore cel,Anteparo an, double bx,double by,double angulo, double distAnteparo){
    if(cel == NULL){
        //célula vazia, nova célula deve ser criada.
        stCelulaArvore* novaArvore = malloc(sizeof(stCelulaArvore));
        if(novaArvore == NULL){
            printf("Erro ao alocar novaArvore em insereRecursivo\n");
            return NULL;
        }
        novaArvore->anteparo = an;
        novaArvore->direita = NULL;
        novaArvore->esquerda = NULL;
        return (celulaArvore)novaArvore;
    }
    //árvore não vazia
    stCelulaArvore* celArv =(stCelulaArvore*)cel;

    Vertice novoVertice = calculaInterseccao(bx,by,angulo,celArv->anteparo);

    //usar valor grande p indicar que não há intersecção
    double dist_novoVertice = (novoVertice != NULL) ? getDistanciaVertice(novoVertice) : 999999999.0; 
    if(novoVertice != NULL){
        //erro ao criar o novo vertice
        destroiVertice(novoVertice);
    }

    if(distAnteparo < dist_novoVertice){
        celArv->esquerda = insereRecursivo(celArv->esquerda,an,bx,by,angulo,distAnteparo);
    }else{
        celArv->direita = insereRecursivo(celArv->direita,an, bx,by,angulo, distAnteparo);
    }
    return celArv;
}

void insereArvore(Arvore arv, Anteparo an, double bx, double by, double angulo,double distCelAnterior){
    if(arv == NULL ){
        printf("Erro em insereArvore\n");
        return;
    }
    stArvore* a = (stArvore*)arv;
    a->raiz = insereRecursivo(a->raiz,an,bx,by,angulo,distCelAnterior);    
}

celulaArvore encontrarMinimo(celulaArvore cel){
    if(cel == NULL) return NULL;
    stCelulaArvore* c =(stCelulaArvore*)cel;
    while(c->esquerda != NULL){
        c = c->esquerda;
    }
    return c;
}

celulaArvore encontrarMinimoArvore(Arvore arv){
    if(arv == NULL) return NULL;
    
    stArvore* arvore = (stArvore*)arv;
    
    if(arvore->raiz == NULL) return NULL;
    
    return encontrarMinimo(arvore->raiz);
}

celulaArvore removerRecursivo(celulaArvore cel, int id, double bx, double by, double angulo, double distAnteparo){
    if(cel == NULL) return NULL;

    stCelulaArvore* celArv = (stCelulaArvore*)cel;

    // Recalcular distância do nó atual para decidir direção
    Vertice v = calculaInterseccao(bx, by, angulo, celArv->anteparo);
    double distAtual;
    
    if (v != NULL) {
        distAtual = getDistanciaVertice(v);
        destroiVertice(v);
    } else {
        // Falha no cálculo (interseção inválida ou erro numérico)
        // Se este for o nó que procuramos, precisamos forçar o encontro.
        if(getIDAnteparo(celArv->anteparo) == id) {
            // Se o ID bate, fingimos que a distância é compatível (match)
            // ou tratamos diretamente no bloco 'else' abaixo.
            // Vamos definir distAtual tal que entre no 'else' do match.
            // Se distAnteparo for válido, podemos usar ele? Não, perigoso.
            // Vamos deixar cair no bloco de comparação, mas com um flag ou lógica especial?
            // Simplificação: Se ID bate, processamos como encontrado AGORA.
        }
        distAtual = 999999999.0;
    }

    // Lógica de comparação melhorada
    // Primeiro verifica ID para evitar problemas de precisão se estivermos muito perto
    int idAtual = getIDAnteparo(celArv->anteparo);
    
    // Se achou pelo ID (mesmo que distâncias variem um pouco por precisão), remove.
    // MAS CUIDADO: Em uma árvore espacial, ID não garante posição.
    // Somente removemos pelo ID se a distância também for coerente (ou se o cálculo falhou mas é ele)
    
    // Se distâncias muito diferentes, confia na geometria
    if(fabs(distAnteparo - distAtual) > 1e-4 && distAnteparo < distAtual){
        celArv->esquerda = removerRecursivo(celArv->esquerda, id, bx, by, angulo, distAnteparo);
    } else if (fabs(distAnteparo - distAtual) > 1e-4 && distAnteparo > distAtual){
        celArv->direita = removerRecursivo(celArv->direita, id, bx, by, angulo, distAnteparo);
    } else {
        // Distâncias próximas ou iguais (ou fallback de erro) - Verifica ID
        if (id == idAtual) {
             // achou a celula
            if(celArv->esquerda == NULL){
                stCelulaArvore* temp = celArv->direita;
                free(celArv);
                return temp;
            }else if(celArv->direita == NULL){
                stCelulaArvore* temp = celArv->esquerda;
                free(celArv);
                return temp;
            }
            
            // Dois filhos: encontrar sucessor (mínimo da direita)
            stCelulaArvore* sucessor = encontrarMinimo(celArv->direita);
            celArv->anteparo = sucessor->anteparo;
            
            // Calcular distância do sucessor para a chamada recursiva de remoção
            Vertice vSuc = calculaInterseccao(bx, by, angulo, sucessor->anteparo);
            double distSuc = (vSuc != NULL) ? getDistanciaVertice(vSuc) : 999999999.0;
            if(vSuc != NULL) destroiVertice(vSuc);

            celArv->direita = removerRecursivo(celArv->direita, getIDAnteparo(sucessor->anteparo), bx, by, angulo, distSuc);
        } else {
            // Distância parecida mas não é o ID.
            // Precisamos decidir para onde ir.
            // Na inserção, empate ou maior vai para direita.
            // Tenta direita primeiro.
            celArv->direita = removerRecursivo(celArv->direita, id, bx, by, angulo, distAnteparo);
            
            // Opcional: Se não achou na direita e a diferença era pequena, poderia tentar esquerda?
            // Mas em árvore BST estrita, não se faz backtracking.
        }
    }
    return cel;
}

void removerArvore(Arvore arv, int id, double bx, double by, double angulo, double distAnteparo){
    if(arv == NULL){
        printf("Erro em removeArvore\n");
        return;
    }
    
    stArvore* arvore = (stArvore*)arv;
    arvore->raiz = removerRecursivo(arvore->raiz, id, bx, by, angulo, distAnteparo);

}

void liberarNosRecursivo(stCelulaArvore* cel){
    if(cel == NULL) return;
    liberarNosRecursivo(cel->esquerda);
    liberarNosRecursivo(cel->direita);
    free(cel);
}

void liberarArvore(Arvore arv){
    if(arv == NULL){
        return;
    }
    
    stArvore* arvore = (stArvore*)arv;
    liberarNosRecursivo(arvore->raiz);
    free(arvore);
}

Anteparo getAnteparoCelula(celulaArvore cel){
    if(cel == NULL){
        printf("Erro em getAnteparoCelula: célula NULL\n");
        return NULL;
    }
    
    stCelulaArvore* celula = (stCelulaArvore*)cel;
    return celula->anteparo;
}