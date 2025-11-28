#include "qry.h"

void gerarArena(FILE* svgEntrada, Lista pacotes,Lista anteparos){
    if(svgEntrada == NULL || pacotes == NULL || anteparos == NULL){
        printf("erro em gerarArena no qry\n");
        return;
    }
    //busca dos menores valores
    double menorXPacotes = getMenorValorLista(pacotes,1);
    double menorXAnteparos = getMenorValorLista(anteparos,1);
    double menorX = (menorXPacotes < menorXAnteparos) ? menorXPacotes : menorXAnteparos;
    
    double menorYPacotes = getMenorValorLista(pacotes,2);
    double menorYAnteparos = getMenorValorLista(anteparos, 2);
    double menorY = (menorYPacotes < menorYAnteparos) ?  menorYPacotes : menorYAnteparos;
    
    //busca dos maiores valores
    double maiorXPacotes = getMaiorValorLista(pacotes, 1);
    double maiorXAnteparos = getMaiorValorLista(anteparos,1);
    double maiorX = (maiorXPacotes > maiorXAnteparos) ? maiorXPacotes : maiorXAnteparos;

    double maiorYPacotes = getMaiorValorLista(pacotes, 2);
    double maiorYAnteparos = getMaiorValorLista(anteparos,2);
    double maiorY = (maiorYPacotes > maiorYAnteparos) ? maiorYPacotes : maiorYAnteparos;

    //criação do retangulo que será a arena
    Retangulo arena = criaRetangulo(-100,menorX,menorY,maiorX, maiorY,"#000000", "#000000");
    desenharRetanguloSVG(svgEntrada, arena);

    liberaRetangulo(arena);

}

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
                liberaCirculo((Circulo)formaPac);
                setTipoPacote(pac, 'l');
                setFormaPacote(pac, (Forma)lin);
                insereLista(anteparos, pac);

            }else if(letra == 'v'){//círculo virará um anteparo(linha) vertical
                Linha lin = criarLinha(id,x, y-raio, x, y+raio,cor);
                liberaCirculo((Circulo)formaPac);
                setTipoPacote(pac, 'l');
                setFormaPacote(pac, (Forma)lin);
                insereLista(anteparos, pac);

            }else{
                printf("letra de comando invalida em comandoA\n");
                break;
            }
            fprintf(arqTxt, "id do círculo: %i, figura original: círculo\n", id);
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
            if(maior < 0) maior = 0;

            //criar os novos pacotes que conterão os anteparos
            Linha esquerda = criarLinha(maior + 1,x,y,x, y+h,cor);
            Pacote esq = criarPacote();
            setTipoPacote(esq, 'l');
            setFormaPacote(esq, (Forma)esquerda);
            insereLista(anteparos,esq);

            Linha direita = criarLinha(maior + 2,x+w,y, x+w, y+h,cor);
            Pacote dir = criarPacote();
            setTipoPacote(dir, 'l');
            setFormaPacote(dir, (Forma)direita);
            insereLista(anteparos,dir);

            Linha cima = criarLinha(maior + 3,x,y,x+w,y,cor);
            Pacote ci = criarPacote();
            setTipoPacote(ci, 'l');
            setFormaPacote(ci, (Forma)cima);
            insereLista(anteparos,ci);

            Linha baixo = criarLinha(maior + 4,x,y+h,x+w,y+h,cor);
            Pacote ba = criarPacote();
            setTipoPacote(ba, 'l');
            setFormaPacote(ba, (Forma)baixo);
            insereLista(anteparos,ba);

            fprintf(arqTxt, "ID do retangulo: %i, id do segmento da esquerda: %i, id do segmento da direita: %i, id do segmento de cima: %i, id do segmento de baixo: %i, figura original: Retângulo\n",id,
                getIDLinha(esquerda),
                getIDLinha(direita),
                getIDLinha(cima),
                getIDLinha(baixo));

            //liberar o pacote original e remover da lista de pacotes
            removeLista(pacotes, getIDRetangulo((Retangulo)ret));
            liberarPacote(pac);

        }else if(tipo == 't'){
            Forma t = getFormaPacote(pac);
            int id = getIDTexto((Texto)t);
            char ancora = getATexto((Texto)t);
            int tamanho = strlen(getTxtoTexto((Texto)t));
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
                x1 = x - 10*tamanho;
                y1 = y;
                x2 = x;
                y2 = y;

            }else if(ancora == 'm'){
                x1 = x - 10*tamanho/2;
                y1 = y;
                x2 = x + 10*tamanho/2;
                y2 = y;
            }else{
                printf("Âncora inválida em comandoA\n");
            }

            Linha posTxt = criarLinha(id,x1,y1,x2,y2,cor);
            liberaTexto((Texto)t);

            setTipoPacote(pac, 'l');
            setFormaPacote(pac, (Forma)posTxt);
            insereLista(anteparos, pac);
            fprintf(arqTxt,"Id do texto: %i, figura original: Texto\n", id);

        }
        contador++;
    }
}

void comandoD(FILE* arqTxt, FILE* svgSfx, Lista anteparos){
    if(arqTxt == NULL || svgSfx == NULL || anteparos == NULL){
        printf("Erro em comandoD\n");
        return;
    }
}
