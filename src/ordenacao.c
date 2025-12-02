#include "ordenacao.h"

typedef struct NO{
    Pacote pac;
    double distanciaPto1;
    double distanciaPto2;
}stNo;

No gerarArray(Lista lista,int tamanho,double xOrigem, double yOrigem){
    if(lista == NULL){
        printf("Erro em gerarArray\n");
        return NULL;
    }
    
    stNo* array = (stNo*)calloc(tamanho, sizeof(stNo));
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
    return (No)array;
}

void liberarArray(No array){
    if(array == NULL){
        printf("Erro em liberarArray.\n");
        return;
    }
    stNo* ar = (stNo*)array;

    free(ar);
}

double distanciaEntrePontos(double x1, double y1, double x2, double y2){
    double horizontal = fabs(x2-x1);
    double potenciaX = pow(horizontal, 2);

    double vertical = fabs(y2-y1);
    double potenciaY = pow(vertical,2);

    double distancia = sqrt(potenciaX + potenciaY);

    return distancia;
}

void mergeSort(No array, int tamanho){
    if(array == NULL){
        printf("Erro em mergeSort\n");
        return;
    }
    
    if(tamanho <= 1){
        return;
    }
    
    stNo* ar = (stNo*)array;
    
    for(int tamanhoAtual = 1; tamanhoAtual < tamanho; tamanhoAtual *= 2){
        for(int inicio = 0; inicio < tamanho - 1; inicio += 2 * tamanhoAtual){
            int meio = inicio + tamanhoAtual - 1;
            int fim = (inicio + 2 * tamanhoAtual - 1 < tamanho - 1) ? 
                      inicio + 2 * tamanhoAtual - 1 : tamanho - 1;
            
            if(meio >= tamanho) continue;
            
            int n1 = meio - inicio + 1;
            int n2 = fim - meio;
            
            stNo* L = (stNo*)malloc(n1 * sizeof(stNo));
            stNo* R = (stNo*)malloc(n2 * sizeof(stNo));
            
            if(L == NULL || R == NULL){
                printf("Erro ao alocar memória em mergeSort\n");
                if(L) free(L);
                if(R) free(R);
                return;
            }
            
            for(int i = 0; i < n1; i++){
                L[i] = ar[inicio + i];
            }
            for(int j = 0; j < n2; j++){
                R[j] = ar[meio + 1 + j];
            }
            
            int i = 0, j = 0, k = inicio;
            
            while(i < n1 && j < n2){
                double somaL = L[i].distanciaPto1 + L[i].distanciaPto2;
                double somaR = R[j].distanciaPto1 + R[j].distanciaPto2;
                
                if(somaL <= somaR){
                    ar[k] = L[i];
                    i++;
                }else{
                    ar[k] = R[j];
                    j++;
                }
                k++;
            }
            
            while(i < n1){
                ar[k] = L[i];
                i++;
                k++;
            }
            
            while(j < n2){
                ar[k] = R[j];
                j++;
                k++;
            }
            
            free(L);
            free(R);
        }
    }
}

void insertionSort(Lista desordenada, double xOrigem, double yOrigem, Arvore arv){
    if(desordenada == NULL){
        printf("Erro em insertionSort\n");
        return;
    }
    
    int tamanho = getTamanhoLista(desordenada);
    No array = gerarArray(desordenada, tamanho, xOrigem, yOrigem); 
    if(array == NULL){
        printf("Erro em insertionSort\n");  
        return;
    }
    
    stNo* ar = (stNo*)array;  

    for(int i = 1; i < tamanho; i++){
        stNo chave = ar[i];  
        int j = i - 1;

        double somaChave = chave.distanciaPto1 + chave.distanciaPto2;

        while(j >= 0 && (ar[j].distanciaPto1 + ar[j].distanciaPto2) > somaChave){  
            ar[j + 1] = ar[j];  
            j--;
        }
        ar[j + 1] = chave;  
    }

    for(int i = 0; i < tamanho; i++){
        insereArvore(arv, ar[i].pac);
    }

    liberarArray(array);
}

double determinante(double x1, double y1, double x2, double y2, double x3, double y3){
    double det = x1*y2 + y1*x3 + x2*y3 - (y2*x3 + x1*y3 + y1*x2);
    return det;
}
