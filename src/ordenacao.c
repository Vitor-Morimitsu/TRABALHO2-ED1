#include "ordenacao.h"

typedef struct NO{
    Pacote pac;
    double distanciaPto1;
    double distanciaPto2;
}No;

No* gerarArray(Lista lista,double xOrigem, double yOrigem){
    if(lista == NULL){
        printf("Erro em gerarArray\n");
        return NULL;
    }
    int tamanho = getTamanhoLista(lista);
    No* array = (No*)calloc(tamanho, sizeof(No));
    if(array == NULL){
        printf("Erro ao alocar memória em gerarArray\n");
        return NULL;
    }
    
    int contador = 0;
    void* atual = getPrimeiraCelulaLista(lista);

    while(contador < tamanho){
        Pacote pacAtual = getConteudoCelula(atual);
        Forma formaAtual = getFormaPacote(pacAtual);

        double x1 = getX1Linha((Linha)formaAtual);
        double y1 = getY1Linha((Linha)formaAtual);

        double x2 = getX2Linha((Linha)formaAtual);
        double y2 = getY2Linha((Linha)formaAtual);

        double distancia1 = distanciaEntrePontos(xOrigem,yOrigem,x1,y1);
        double distancia2 = distanciaEntrePontos(xOrigem,yOrigem,x2,y2);

        array[contador].pac = pacAtual;
        array[contador].distanciaPto1 = distancia1;
        array[contador].distanciaPto2 = distancia2;

        atual = getProximaCelulaLista(atual);
        contador++;
    }
    return array;
}


double distanciaEntrePontos(double x1, double y1, double x2, double y2){
    double horizontal = fabs(x2-x1);
    double potenciaX = pow(horizontal, 2);

    double vertical = fabs(y2-y1);
    double potenciaY = pow(vertical,2);

    double distancia = sqrt(potenciaX + potenciaY);

    return distancia;
}


void mergeSort(Lista desordenada){
    if(desordenada == NULL){
        printf("Erro em mergeSort\n");
        return;
    }
    



}

void insertionSort(Lista desordenada,double xOrigem, double yOrigem, Arvore arv){
    if(desordenada == NULL){
        printf("Erro em insertionSort\n");
        return NULL;
    }
    //desordenada = lista de anteparos

    int tamanho = getTamanhoLista(desordenada);
    int contador = 0;
    
    while(contador != tamanho){
        //obter x1,y1,x2,y2 de dois pacotes e calcular a distancia até o ponto em que a bomba está
        Pacote a = getPrimeiroPacoteLista(desordenada); 
        if(a == NULL){
            printf("lista vazia em insertion sort\n");
            return NULL;
        }
        
        Forma linhaA = getFormaPacote(a);
        double x1a = getX1Linha((Linha)linhaA);
        double y1a = getY1Linha((Linha)linhaA);
        double distancia1A = distanciaEntrePontos(xOrigem,yOrigem,x1a,y1a);

        double x2a = getX2Linha((Linha)linhaA);
        double y2a = getY2Linha((Linha)linhaA);
        double distancia2A = distanciaEntrePontos(xOrigem, yOrigem,x2a,y2a);

        void* celula = getPrimeiraCelulaLista(desordenada);
        void* proximaCelula = getProximaCelulaLista(celula);

        Pacote b = NULL;
        if(proximaCelula != NULL) b = getConteudoCelula(proximaCelula);
        Forma linhaB = getFormaPacote(b);
        double x1b = getX1Linha((Linha)linhaB);
        double y1b = getY1Linha((Linha)linhaB);
        double distancia1B = distanciaEntrePontos(xOrigem,yOrigem, x1b,y1b);

        double x2b = getX2Linha((Linha)linhaB);
        double y2b = getY2Linha((Linha)linhaB);
        double distancia2B = distanciaEntrePontos(xOrigem,yOrigem,x2b,y2b);        

        //ordenar com base na distancia
        double menorA = (distancia1A < distancia2A) ? distancia1A : distancia2A;
        double menorB = (distancia1B < distancia2B) ? distancia1B : distancia2B;

        if(menorA < menorB) {
            insereArvore(arv, a);
            insereArvore(arv, b);
        }else{
            insereArvore(arv, b);
            insereArvore(arv, a);
        }

        contador++;
    }
}

double determinante(double x1, double y1, double x2, double y2, double x3, double y3){
    double det = x1*y2 + y1*x3 + x2*y3 - (y2*x3 + x1*y3 + y1*x2);
    return det;
}
