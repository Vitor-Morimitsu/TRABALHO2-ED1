#include "svg.h"

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
