#include "ordenacao.h"

typedef struct NO{
    Vertice ver;
    double angulo;
}stNo;

No* gerarArray(Lista vertices, int tamanho, double /*xOrigem*/, double /*yOrigem*/){
    if(vertices == NULL){
        printf("Erro em gerarArray\n");
        return NULL;
    }
    
    stNo* array = (stNo*)calloc(tamanho, sizeof(stNo));
    if(array == NULL){
        printf("Erro ao alocar memória em gerarArray\n");
        return NULL;
    }
    
    int contador = 0;
    CelulaLista atual = getPrimeiraCelulaLista(vertices);

    while(contador < tamanho && atual != NULL){
        Vertice v = (Vertice)getConteudoCelula(atual);      
        
        array[contador].ver = v;
        array[contador].angulo = getAnguloVertice(v);

        atual = getProximaCelulaLista(atual);
        contador++;
    }
    return (No*)array;
}

void liberarArray(No array){
    if(array == NULL){
        printf("Erro em liberarArray.\n");
        return;
    }

    stNo* ar = (stNo*)array;
    free(ar->ver);

    free(ar);
}


void mergeSort(No* array, int tamanho){
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
                
                if(L[i].angulo <= R[j].angulo){
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

void insertionSort(No* array, int tamanho){
    if(array == NULL){
        printf("Erro em insertionSort\n");
        return;
    }
    
    if(tamanho <= 1){
        return;
    }
    
    stNo* ar = (stNo*)array;
    
    for(int i = 1; i < tamanho; i++){
        stNo chave = ar[i];
        int j = i - 1;
        
        // Move elementos maiores que a chave uma posição à frente
        while(j >= 0 && ar[j].angulo > chave.angulo){
            ar[j + 1] = ar[j];
            j--;
        }
        
        ar[j + 1] = chave;
    }
}

