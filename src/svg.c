#include "svg.h"

void abrirSvg(FILE* arqSvg){
    fprintf(arqSvg, "<svg xmlns:svg=\"http://www.w3.org/2000/svg\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n");
}

void desenharCirculoSVG(FILE* arqSvg, Circulo circ) {
    if(circ == NULL){
        fprintf(stderr, "ERRO: Forma NULL ao desenhar círculo SVG\n");
        return;
    }    

    fprintf(arqSvg, "\t<circle id=\"%i\" cx=\"%f\" cy=\"%f\" r=\"%f\" stroke=\"%s\" fill=\"%s\" fill-opacity=\"0.5\"/>\n",
        getIDCirculo(circ), 
        getCoordXCirculo(circ),
        getCoordYCirculo(circ),
        getRaioCirculo(circ), 
        getCorBCirculo(circ), 
        getCorPCirculo(circ));
}

void desenharRetanguloSVG(FILE* arqSvg, Retangulo r){
    if(r == NULL || arqSvg == NULL){
        fprintf(stderr, "ERRO: Forma ou arquivo NULL ao desenhar retângulo SVG\n");
        return;
    }
    fprintf(arqSvg, "\t<rect id=\"%d\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"%s\" fill=\"%s\" opacity=\"0.5\" />\n", 
        getIDRetangulo(r), 
        getCoordXRetangulo(r),
        getCoordYRetangulo(r), 
        getWRetangulo(r), 
        getHRetangulo(r), 
        getCorBRetangulo(r), 
        getCorPRetangulo(r));
}
void desenharLinhaSVG(FILE* arqSvg, Linha l){
    if(l == NULL || arqSvg == NULL){
        fprintf(arqSvg,"Erro: Forma ou arquivo NULL ao desenhar Linha Svg\n");
        return;
    }
     fprintf(arqSvg, "<line id=\"%d\" x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"%s\" stroke-width=\"2\" />\n", 
        getIDLinha(l), 
        getX1Linha(l), 
        getY1Linha(l), 
        getX2Linha(l), 
        getY2Linha(l), 
        getCorLinha(l));
}

void desenharTextoSVG(FILE* arqSvg, Texto t, Estilo ts){
    if(arqSvg == NULL || t == NULL){
        fprintf(stderr, "Erro: Forma ou arquivo NULL ao desenhar Texto Svg\n");
        return;
    }
    if (ts != NULL) {
        fprintf(arqSvg, "<text id=\"%d\" x=\"%lf\" y=\"%lf\" style=\"font-family: '%s'; font-size: %spx; font-weight: %s; fill: %s;\"> %s </text>\n",
        getIDTexto(t), 
        getCoordXTexto(t),  
        getCoordYTexto(t), 
        getfFamily(ts),
        getfSize(ts),
        getfWeight(ts), 
        getCorPTexto(t),  
        getTxtoTexto(t));
    } else {
        fprintf(arqSvg, "<text id=\"%d\" x=\"%lf\" y=\"%lf\" fill=\"%s\"> %s </text>\n",
        getIDTexto(t), 
        getCoordXTexto(t),  
        getCoordYTexto(t), 
        getCorPTexto(t),  
        getTxtoTexto(t));
    }
}

void desenharPoligonoSVG(FILE* arqSvg, Poligono p, char* corB, char* corP){
    if(arqSvg == NULL || p == NULL){
        printf("Erro em desenharPoligonoSvg\n");
        return;
    }

    int n = getNumeroVertices(p);
    if(n == 0) return;

    fprintf(arqSvg, "<polygon points=\"");

    for (int i = 0; i < n; i++){
        double x, y;
        getVerticePoligono(p, i, &x, &y);
        fprintf(arqSvg, "%.2f,%.2f ", x, y);
    }
    fprintf(arqSvg, "\" style=\"fill:%s; opacity:0.5; stroke:%s; stroke-width:2\" />\n", corP, corB);

    for(int i = 0; i < n;i++){
        double x1,y1,x2,y2;
        int proximo = (i+1)%n;

        getVerticePoligono(p,i,&x1,&y1);
        getVerticePoligono(p,proximo,&x2,&y2);

        Anteparo ant1 = getAnteparoPoligono(p,i);
        Anteparo ant2 = getAnteparoPoligono(p,proximo);
        if(ant1 != NULL && ant2 != NULL && ant1 == ant2) {
            // É PAREDE
            fprintf(arqSvg, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" "
                            "style=\"stroke:red; stroke-width:4\" />\n", 
                            x1, y1, x2, y2);
        } else {
            // É ESPAÇO VAZIO (RAIO)
            fprintf(arqSvg, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" ""style=\"stroke:%s; stroke-width:1; stroke-dasharray:5,5\" />\n",x1, y1, x2, y2, corB);
        }
    }
}

void fecharSVG(FILE* arqSvg) {
    fprintf(arqSvg, "</svg>\n");
}

void desenharTodasAsFormas(FILE* arqSvg, Lista formas) {
    if (arqSvg == NULL || formas == NULL) {
        return;
    }

    for (CelulaLista celula = getPrimeiraCelulaLista(formas); celula != NULL; celula = getProximaCelulaLista(celula)) {
        Pacote pac = (Pacote)getConteudoCelula(celula);
        if (pac == NULL) continue;

        char tipo = getTipoPacote(pac);
        Forma forma = getFormaPacote(pac);

        switch (tipo) {
            case 'c':
                desenharCirculoSVG(arqSvg, (Circulo)forma);
                break;
            case 'r':
                desenharRetanguloSVG(arqSvg, (Retangulo)forma);
                break;
            case 'l':
                desenharLinhaSVG(arqSvg, (Linha)forma);
                break;
            case 't':
                // Para textos, precisamos do estilo. Vamos assumir NULL se não houver um.
                // O ideal seria armazenar o estilo no pacote também.
                desenharTextoSVG(arqSvg, (Texto)forma, NULL); 
                break;
            default:
                // Forma desconhecida ou não visualizável
                break;
        }
    }
}