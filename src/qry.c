#include "qry.h"

void comandoA(FILE* arqTxt,Lista pacotes,Lista anteparos,int inicio, int fim, char letra){
    if(arqTxt == NULL || pacotes == NULL){
        printf("Erro em comandoA\n");
        return;
    }

    int contador = inicio;
    while(contador != fim){
        Pacote pac = procuraPacoteLista(pacotes, contador);
        if(pac == NULL){//não achou o pacote
            printf("Pacote não encontrado em comandoA\n");
            break;    
        }else{
            insereLista(anteparos,pac);
        }

        char tipo = getTipoPacote(pac);
        if(tipo == 'c'){
            Forma formaPac = getFormaPacote(pac);
            int id = getIDCirculo((Circulo)formaPac);
            double x = getCoordXCirculo((Circulo)formaPac);
            double y = getCoordYCirculo((Circulo)formaPac);
            double raio = getRaioCirculo((Circulo)formaPac);
            char cor[50] = getCorPCirculo((Circulo)formaPac);

            if(letra == 'h'){//círculo virará um anteparo(linha) horizontal
                Linha lin = criarLinha(id,x - raio, y,x+raio,y,cor);
                setFormaPacote(pac, (Forma)lin);

            }else if(letra == 'v'){//círculo virará um anteparo(linha) vertical
                Linha lin = criarLinha(id,x, y-raio, x, y+raio,cor);
                setFormaPacote(pac, (Forma)lin);

            }else{
                printf("letra de comando invalida em comandoA\n");
                break;
            }
            fprintf(arqTxt, "id do segmento: %i, figura original: círculo\n");
        }else if(tipo == 'r'){}
        contador++;

    }

}