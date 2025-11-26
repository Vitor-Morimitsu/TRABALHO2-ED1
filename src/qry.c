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
                insereLista(anteparos, pac);

            }else if(letra == 'v'){//círculo virará um anteparo(linha) vertical
                Linha lin = criarLinha(id,x, y-raio, x, y+raio,cor);
                setFormaPacote(pac, (Forma)lin);
                insereLista(anteparos, pac);

            }else{
                printf("letra de comando invalida em comandoA\n");
                break;
            }
            fprintf(arqTxt, "id do segmento: %i, figura original: círculo\n");
        }else if(tipo == 'r'){
            Forma ret = getFormaPacote(pac);
            int id = getIDRetangulo((Retangulo)ret);
            double x,y,w,h;
            x = getCoordXRetangulo((Retangulo)ret);
            y = getCoordYRetangulo((Retangulo)ret);
            w = getWRetangulo((Retangulo)ret);
            h = getHRetangulo((Retangulo)ret);
            char cor[50] = getCorPRetangulo((Retangulo)ret);
            //criar 4 segmentos com base nos lados do retangulo a partir do maior id da lista.
            int maiorIDPacotes = getMaiorIdLista(pacotes);    
            int maiorIDAnteparos = getMaiorIdLista(anteparos);
            int maior = -1;
            maior = (maiorIDAnteparos > maiorIDPacotes) ? maiorIDAnteparos : maiorIDPacotes;
            //começar de cima p baixo e da esquerda p direita
            Linha esquerda = criarLinha(maior + 1,x,y,x+h, y+h,cor);
            setFormaPacote(pac, (Forma)esquerda);
            insereLista(anteparos,pac);

            Linha direita = criarLinha(maior + 2,x+w,y, x+w, y+h,cor);
            setFormaPacote(pac, (Forma)direita);
            insereLista(anteparos,pac);

            Linha cima = criarLinha(maior + 3,x,y,x+w,y,cor);
            setFormaPacote(pac, (Forma)cima);
            insereLista(anteparos,pac);

            Linha baixo = criarLinha(maior + 4,x,y+h,x+w,y+h,cor);
            setFormaPacote(pac, (Forma)baixo);
            insereLista(anteparos,pac);

        }else if(tipo == 't'){
            Forma t = getFormaPacote(pac);
            int id = getIDTexto((Texto)t);
            char ancora = getATexto((Texto)t);
            int tamanho = strlen(getTxtoTexto);
            double x = getCoordXTexto((Texto)t);
            double y = getCoordYTexto((Texto)t);
            double x1,y1,x2,y2;
            char cor[50] = getCorPTexto((Texto)t);

            if(ancora == 'i'){
                x1 = x;
                y1 = y;
                x2 = x + 10.0*tamanho;
                y2 = y;

            }else if(ancora == 'f'){
                x1 = 10*tamanho;
                y1 = y;
                x2 = x;
                y2 = y;

            }else if(ancora == 'm'){
                x1 = x - 10*tamanho/2;
                y1 = y;
                x2 = x + 10*tamanho/2;
                y2 = y;
            }

            Linha posTxt = criarLinha(id,x1,y1,x2,y2,cor);
            setFormaPacote(pac, (Forma)posTxt);
            insereLista(anteparos, pac);

        }
        contador++;
        //atualizar as listas
    }

}