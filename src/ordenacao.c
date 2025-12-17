#include "ordenacao.h"

struct stNo {
    Vertice ver;
    double angulo;
};
typedef struct stNo stNo;

No* gerarArray(Lista vertices, int tamanho){
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

Vertice getVerticeNo(No* n){
    return n->ver;
}

void liberarArray(No* array){
    if(array == NULL){
        printf("Erro em liberarArray.\n");
        return;
    }

    free(array);
}

// Helper para comparação consistente
static int compareStNoValues(stNo noA, stNo noB) {
    // Tolerância para ruído numérico
    if (fabs(noA.angulo - noB.angulo) > 1e-6) {
        if (noA.angulo < noB.angulo) return -1;
        if (noA.angulo > noB.angulo) return 1;
    }

    // Ângulos "iguais": desempate por tipo ('i' antes de 'f')
    char tipoA = getTipoVertice(noA.ver);
    char tipoB = getTipoVertice(noB.ver);

    if (tipoA == 'i' && tipoB == 'f') return -1;
    if (tipoA == 'f' && tipoB == 'i') return 1;

    // Empate de tipo: desempate por distância (opcional, mas bom pra estabilidade)
    double distA = getDistanciaVertice(noA.ver);
    double distB = getDistanciaVertice(noB.ver);
    if (distA < distB) return -1;
    if (distA > distB) return 1;

    return 0;
}

void insertionSortRange(stNo* array, int inicio, int fim){
    for(int i = inicio + 1; i <= fim; i++){
        stNo chave = array[i];
        int j = i - 1;
        
        // CORREÇÃO: Usar compareStNoValues
        // Movemos se array[j] > chave
        while(j >= inicio && compareStNoValues(array[j], chave) > 0){
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = chave;
    }
}

void mergeSort(No* array, int tamanho, int limiteInsertionSort){
    if(array == NULL){
        printf("Erro em mergeSort\n");
        return;
    }
    stNo* a =(stNo*)array;
    
    if(tamanho <= 1) return;
    
    for(int tamanhoAtual = 1; tamanhoAtual < tamanho; tamanhoAtual *= 2){
        for(int inicio = 0; inicio < tamanho - 1; inicio += 2 * tamanhoAtual){
            int meio = inicio + tamanhoAtual - 1;
            int fim = (inicio + 2 * tamanhoAtual - 1 < tamanho - 1) ? inicio + 2 * tamanhoAtual - 1 : tamanho - 1;
            
            if(meio >= tamanho) continue;

            int tamSubvetor = fim - inicio +1;
            if(tamSubvetor <= limiteInsertionSort){
                insertionSortRange(a,inicio,fim);
                continue;
            }
            
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
                L[i] = a[inicio + i];
            }
            for(int j = 0; j < n2; j++){
                R[j] = array[meio + 1 + j];
            }
            
            int i = 0, j = 0, k = inicio;
            
            while(i < n1 && j < n2){
                // CORREÇÃO: Usar compareStNoValues
                if(compareStNoValues(L[i], R[j]) <= 0){
                    a[k] = L[i];
                    i++;
                }else{
                    a[k] = R[j];
                    j++;
                }
                k++;
            }
            
            while(i < n1){
                a[k] = L[i];
                i++;
                k++;
            }
            
            while(j < n2){
                a[k] = R[j];
                j++;
                k++;
            }
            
            free(L);
            free(R);
        }
    }
}

Vertice getVerticeDoArray(No* array, int indice) {
    return array[indice].ver;
}

double getAnguloDoArray(No* array, int indice) {
    return array[indice].angulo;
}

void insertionSort(No* array, int tamanho){
    if(array == NULL){
        printf("Erro em insertionSort\n");
        return;
    }
    stNo* a =(stNo*)array;
    
    if(tamanho <= 1){
        return;
    }
    
    // Reutilizar a implementação Range para evitar duplicação
    insertionSortRange(a, 0, tamanho - 1);
}

int compararNos(const void* a, const void* b) {
    stNo* noA = (stNo*)a;
    stNo* noB = (stNo*)b;
    return compareStNoValues(*noA, *noB);
}

// function used via qsort
// static void swap(stNo* a, stNo* b) {
//     stNo t = *a;
//     *a = *b;
//     *b = t;
// }

void quickSort(No* arrayParametro, int low, int high) {
    if(low >= high) return;

    stNo* array = (stNo*)arrayParametro;
    qsort(&array[low], high - low + 1, sizeof(stNo), compararNos);
}
