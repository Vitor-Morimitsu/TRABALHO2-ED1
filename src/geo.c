#include "geo.h"

#define MAX_SIZE 200

void lerGeo(FILE* geo, Lista list){
    if(geo == NULL || list == NULL){
        printf("Erro em lerGeo\n");
        return;
    }
    char linha[MAX_SIZE];
    char comando[10];

    while(fgets(linha, sizeof(linha), geo) != NULL){
        if(linha[0] == '\n' || linha[0] == '#') continue;

        int lidos_cmd = sscanf(linha, "%9s", comando);
        if (lidos_cmd != 1) {
            continue;
        }

        if (strcmp(comando, "c") == 0) {
            int id;
            double x, y, r;
            char corb[32] = "";
            char corp[32] = "";
            
            int lidos = sscanf(&linha[strlen(comando)], " %d %lf %lf %lf %31s %31s", &id, &x, &y, &r, corb, corp);
            if(lidos != 6) {
                fprintf(stderr, "Erro ao ler círculo: campos insuficientes (%d/6) na linha: %s\n", lidos, linha);
                continue;
            }
            
            Pacote pac = criaPacote();
            Circulo c = criaCirculo(id, x, y, r, corb, corp);
            setFormaPacote(pac, (Forma)c);
            setTipoPacote(pac, 'c');

            insereLista(list, pac);

        } else if (strcmp(comando, "r") == 0) {
            int id;
            double x, y, w, h;
            char corb[32] = "";  
            char corp[32] = "";  
            
            int lidos = sscanf(&linha[strlen(comando)], " %d %lf %lf %lf %lf %31s %31s", &id, &x, &y, &w, &h, corb, corp);
            if(lidos != 7) {
                fprintf(stderr, "Erro ao ler retângulo: campos insuficientes (%d/7) na linha: %s\n", lidos, linha);
                continue;
            }
            
            Pacote pac = criaPacote();
            Retangulo r = criaRetangulo(id, x, y, w, h, corb, corp);
            setFormaPacote(pac, (Forma)r);
            setTipoPacote(pac, 'r');

            insereLista(list, pac);

        } else if (strcmp(comando, "l") == 0) {
            int i;
            double x1, y1, x2, y2;
            char cor[32] = "";  
            
            int lidos = sscanf(&linha[strlen(comando)], " %d %lf %lf %lf %lf %31s", &i, &x1, &y1, &x2, &y2, cor);
            if(lidos != 6) {
                fprintf(stderr, "Erro ao ler linha: campos insuficientes (%d/6) na linha: %s\n", lidos, linha);
                continue;
            }
            
            Pacote pac = criaPacote();
            Linha lin = criarLinha(i, x1, y1, x2, y2, cor);
            setFormaPacote(pac, (Forma)lin);
            setTipoPacote(pac, 'l');
            
            insereLista(list, pac);    

        } else if (strcmp(comando, "t") == 0) {
            int i;
            double x, y;
            char corb[32] = "";
            char corp[32] = "";
            char texto[150] = ""; // Increased buffer size for text
            char a = '\0';
            
            int lidos = sscanf(&linha[strlen(comando)], " %d %lf %lf %31s %31s %c %149[^\n]", &i, &x, &y, corb, corp, &a, texto);
            if(lidos != 7) {
                fprintf(stderr, "Erro ao ler texto: campos insuficientes (%d/7) na linha: %s\n", lidos, linha);
                continue;
            }

            Pacote pac = criaPacote();
            Texto text = criarTexto(i, x, y, corb, corp, a, texto);
            setFormaPacote(pac, (Forma)text);
            setTipoPacote(pac, 't');
            insereLista(list, pac);

        } else if (strcmp(comando, "ts") == 0) {
            char font[50] = "";
            char weight[50] = "";
            char size[50] = "";
            
            int lidos_ts = sscanf(&linha[strlen(comando)], " %49s %49s %49s", font, weight, size);
        } else {
            fprintf(stderr, "Aviso: Comando desconhecido '%s' na linha: %s", comando, linha);
        }
    }
}