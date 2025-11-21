#include "geo.h"

#define MAX_SIZE 200

void lerGeo(FILE* geo, FILE* svgEntrada, Lista list){
    if(geo == NULL || svgEntrada == NULL || list == NULL){
        printf("Erro em lerGeo\n");
        return;
    }

    char linha[MAX_SIZE];
    char tipoPacote;

    while(fgets(linha, sizeof(linha), geo) != NULL){
        if(linha[0] == '\n' || linha[0] == '#') continue;

        sscanf(linha, "%c", &tipoPacote);

        if(tipoPacote == 'c'){          //círculo
            int id;
            double x,y,r;
            char corb[32], corp[32];
            sscanf(linha, "c %d %lf %lf %lf %s %s", &id, &x, &y, &r, corb, corp);
            Pacote pac = criaPacote();
            Circulo c = criaCirculo(id,x,y,r,corb,corp);
            setFormaPacote(pac,(Forma)c);
            setTipoPacote(pac, 'c');
            insereLista(list, pac);
        }

    }

}