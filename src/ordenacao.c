#include "ordenacao.h"

stNo* gerarArray(Lista vertices, int tamanho, double /*xOrigem*/, double /*yOrigem*/){
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
    return array;
}

void liberarArray(stNo* array){
    if(array == NULL){
        printf("Erro em liberarArray.\n");
        return;
    }

    free(array);
}


void mergeSort(stNo* array, int tamanho){
    if(array == NULL){
        printf("Erro em mergeSort\n");
        return;
    }
    
    if(tamanho <= 1){
        return;
    }
    
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
                L[i] = array[inicio + i];
            }
            for(int j = 0; j < n2; j++){
                R[j] = array[meio + 1 + j];
            }
            
            int i = 0, j = 0, k = inicio;
            
            while(i < n1 && j < n2){
                
                if(L[i].angulo <= R[j].angulo){
                    array[k] = L[i];
                    i++;
                }else{
                    array[k] = R[j];
                    j++;
                }
                k++;
            }
            
            while(i < n1){
                array[k] = L[i];
                i++;
                k++;
            }
            
            while(j < n2){
                array[k] = R[j];
                j++;
                k++;
            }
            
            free(L);
            free(R);
        }
    }
}

void insertionSort(stNo* array, int tamanho){
    if(array == NULL){
        printf("Erro em insertionSort\n");
        return;
    }
    
    if(tamanho <= 1){
        return;
    }
    
    for(int i = 1; i < tamanho; i++){
        stNo chave = array[i];
        int j = i - 1;
        
        // Move elementos maiores que a chave uma posição à frente
        while(j >= 0 && array[j].angulo > chave.angulo){
            array[j + 1] = array[j];
            j--;
        }
        
        array[j + 1] = chave;
    }
}

static void swap(stNo* a, stNo* b) {
    stNo t = *a;
    *a = *b;
    *b = t;
}

static int partition(stNo* array, int low, int high) {
    double pivot = array[high].angulo;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (array[j].angulo <= pivot) {
            i++;
            swap(&array[i], &array[j]);
        }
    }
    swap(&array[i + 1], &array[high]);
    return (i + 1);
}

void quickSort(stNo* array, int low, int high) {
    if (array == NULL) {
        printf("Erro em quickSort: array nulo\n");
        return;
    }
    if (low < high) {
        int pi = partition(array, low, high);
        quickSort(array, low, pi - 1);
        quickSort(array, pi + 1, high);
    }
}
