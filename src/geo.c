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

        if(tipoPacote == 'c'){//círculo
            int id;
            double x,y,r;
            char corb[32], corp[32];
            sscanf(linha, "c %d %lf %lf %lf %s %s", &id, &x, &y, &r, corb, corp);
            Pacote pac = criaPacote();
            Circulo c = criaCirculo(id,x,y,r,corb,corp);
            setFormaPacote(pac,(Forma)c);
            setTipoPacote(pac, 'c');

            insereLista(list, pac);
            desenharCirculoSVG(svgEntrada,c);
        }else if(tipoPacote == 'r'){//retângulo
            int id;
            double x,y,w,h;
            char corb[32], corp[32];
            sscanf(linha, "r %d %lf %lf %lf %lf %s %s", &id, &x, &y, &w, &h, corb, corp);
            Pacote pac = criaPacote();
            Retangulo r = criaRetangulo(id,x,y,w,h,corb,corp);
            setFormaPacote(pac,(Forma)r);
            setTipoPacote(pac, 'r');

            insereLista(list, pac);
            desenharRetanguloSVG(svgEntrada, r);
        }else if(tipoPacote == 'l'){//Linha
            int i;
            double x1,y1,x2,y2;
            char cor[32];
            sscanf(linha, "l %d %lf %lf %lf %lf %s", &i, &x1, &y1, &x2, &y2, cor);
            Pacote pac = criaPacote();
            Linha lin = criarLinha(i,x1,y1,x2,y2,cor);
            setFormaPacote(pac, (Forma)lin);
            setTipoPacote(pac, 'l');
            
            insereLista(list, pac);    
            desenharLinhaSVG(svgEntrada,lin);       
        }else if(tipoPacote == 't'){//Texto
            int i;
            double x, y;
            char corb[32] = "";
            char corp[32] = "";
            char texto[50] = "";
            char a = '\0';
            sscanf(linha, "t %d %lf %lf %s %s %c %s", &i, &x, &y, corb, corp, &a, texto);

            Pacote pac = criaPacote();
            Texto text = criarTexto(i,x,y,corb,corp,a,texto);
            setFormaPacote(pac,(Texto)text);
            setTipoPacote(pac,'t');

            const char* ts_marker = strstr(linha, " ts ");
            if(ts_marker != NULL){
                char font[500] = "";
                char weight[500] = "";
                char size[500] = "";
                sscanf(ts_marker, " ts %s %s %s", font, weight, size);
                Estilo ts = criarEstilo(font, weight, size);
                setEstiloTexto(text, ts);
            }

            insereLista(list, pac);
            desenharTextoSVG(svgEntrada,text,getEstiloTexto(text));
        }

    }

}
