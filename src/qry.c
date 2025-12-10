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
    while(contador < fim){  // Corrigido: < ao invés de !=
        Pacote pac = procuraPacoteLista(pacotes, contador);
        if(pac == NULL){//não achou o pacote
            printf("Pacote não encontrado em comandoA\n");
            contador++;
            continue;    
        }

        char tipo = getTipoPacote(pac);
        if(tipo == 'c'){
            Forma formaPac = getFormaPacote(pac);
            int id = getIDCirculo((Circulo)formaPac);
            double x = getCoordXCirculo((Circulo)formaPac);
            double y = getCoordYCirculo((Circulo)formaPac);
            double raio = getRaioCirculo((Circulo)formaPac);
            char* cor = getCorPCirculo((Circulo)formaPac);  // Corrigido: char* ao invés de char[]

            if(letra == 'h'){//círculo virará um anteparo(linha) horizontal
                Linha lin = criarLinha(id,x - raio, y,x+raio,y,cor);
                liberaCirculo((Circulo)formaPac);
                setTipoPacote(pac, 'l');
                setFormaPacote(pac, (Forma)lin);
                removeLista(pacotes, id);  // Remove da lista original
                insereLista(anteparos, pac);

            }else if(letra == 'v'){//círculo virará um anteparo(linha) vertical
                Linha lin = criarLinha(id,x, y-raio, x, y+raio,cor);
                liberaCirculo((Circulo)formaPac);
                setTipoPacote(pac, 'l');
                setFormaPacote(pac, (Forma)lin);
                removeLista(pacotes, id);  // Remove da lista original
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
            char* cor = getCorPTexto((Texto)t);  // Corrigido: char* ao invés de char[]

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
            removeLista(pacotes, id);  // Remove da lista original
            insereLista(anteparos, pac);
            fprintf(arqTxt,"Id do texto: %i, figura original: Texto\n", id);

        }
        contador++;
    }
}

void comandoD(FILE* arqTxt, FILE* svgSfx, Lista anteparos, Lista formas, double xBomba, double yBomba, char* comando){
    if(arqTxt == NULL || svgSfx == NULL || anteparos == NULL){
        printf("Erro em comandoD\n");
        return;
    }
    
    // Cria o polígono de visibilidade
    Poligono poligono = criarPoligono();
    
    // Cria árvore para ordenar os segmentos ativos
    Arvore arvoreSegmentos = criarArvore();
    
    // Calcula o polígono de visibilidade
    calcularVisibilidade(poligono, anteparos, formas,arvoreSegmentos, xBomba, yBomba, comando);
    
    // Desenha o polígono de visibilidade no SVG
    desenharPoligonoSVG(svgSfx, poligono, "#FF8A80", "#000000");
    
    // Reporta informações da explosão no arquivo de texto
    fprintf(arqTxt, "Explosão na posição (%.2f, %.2f)\n", xBomba, yBomba);
    fprintf(arqTxt, "Formas destruídas:\n");
    
    // Percorre a lista de formas verificando quais estão dentro do polígono
    CelulaLista celula = getPrimeiraCelulaLista(formas);
    int formasDestruidas = 0;
    
    while(celula != NULL) {
        Pacote pac = (Pacote)getConteudoCelula(celula);
        CelulaLista proximaCelula = getProximaCelulaLista(celula);
        
        // Usa a função de colisão para verificar se a forma está dentro do polígono
        if(formaDentroPoligono(pac, poligono)) {
            char tipo = getTipoPacote(pac);
            int id = getIDPacote(pac);
            
            fprintf(arqTxt, "  - Tipo: %c, ID: %d\n", tipo, id);
            formasDestruidas++;
            
            // Remove da lista e libera o pacote
            removeLista(formas, id);
            liberarPacote(pac);
        }
        
        // Avança para a próxima célula
        celula = proximaCelula;
    }
    
    fprintf(arqTxt, "Total de formas destruídas: %d\n\n", formasDestruidas);
    
    // Libera memória
    liberarArvore(arvoreSegmentos);
    liberarPoligono(poligono);
}

void comandoP(FILE* txt, FILE* svg, Lista formas, Lista anteparos,double x, double y, char* cor, char* comandoSfx){
    if(txt == NULL || svg == NULL || formas == NULL || anteparos == NULL){
        printf("Erro em comandoP\n");
        return;
    }
    
    // Bomba de pintura é lançada na coordenada (x,y). Formas dentro da região de visibilidade tem suas cores de preenchimento e de borda pintadas com a cor especificada.
    
    // Cria o polígono de visibilidade
    Poligono poligono = criarPoligono();
    Arvore arvoreSegmentos = criarArvore();
    
    // Calcula o polígono de visibilidade a partir da posição da bomba
    calcularVisibilidade(poligono, anteparos, formas, arvoreSegmentos, x, y, comandoSfx);
    
    // Obtém a lista de formas atingidas
    Lista formasAtingidas = obterFormasAtingidas(formas, poligono);
    
    if(formasAtingidas != NULL) {
        int numAtingidas = getTamanhoLista(formasAtingidas);
        
        fprintf(txt, "Bomba de pintura lançada em (%.2f, %.2f)\n", x, y);
        fprintf(txt, "Cor aplicada: %s\n", cor);
        fprintf(txt, "Número de formas atingidas: %d\n", numAtingidas);
        
        // Pinta cada forma atingida
        for(CelulaLista celula = getPrimeiraCelulaLista(formasAtingidas); celula != NULL; celula = getProximaCelulaLista(celula)) {
            Pacote pac = (Pacote)getConteudoCelula(celula);
            char tipo = getTipoPacote(pac);
            Forma forma = getFormaPacote(pac);
            int id = getIDPacote(pac);
            
            // Aplica a cor na forma
            switch(tipo) {
                case 'c':
                    setCorBCirculo((Circulo)forma, cor);
                    setCorPCirculo((Circulo)forma, cor);
                    fprintf(txt, "  - Círculo ID %d pintado\n", id);
                    break;
                case 'r':
                    setCorBRetangulo((Retangulo)forma, cor);
                    setCorPRetangulo((Retangulo)forma, cor);
                    fprintf(txt, "  - Retângulo ID %d pintado\n", id);
                    break;
                case 'l':
                    setCorLinha((Linha)forma, cor);
                    fprintf(txt, "  - Linha ID %d pintada\n", id);
                    break;
                case 't':
                    setCorBTexto((Texto)forma, cor);
                    setCorPTexto((Texto)forma, cor);
                    fprintf(txt, "  - Texto ID %d pintado\n", id);
                    break;
            }
        }
        
        // Libera a lista de formas atingidas (não libera os pacotes, apenas a lista)
        liberaLista(formasAtingidas);
    }
    
    // Desenha o polígono de visibilidade no SVG
    desenharPoligonoSVG(svg, poligono, "#80D8FF", "#000000");
    
    // Libera memória
    liberarArvore(arvoreSegmentos);
    liberarPoligono(poligono);
}

void comandoCln(FILE* txt, FILE* svg, Lista formas, Lista anteparos, double x, double y, double dx, double dy, char* comandoSfx){
    if(txt == NULL || svg == NULL || formas == NULL || anteparos == NULL){
        printf("Erro em comandoCln\n");
        return;
    }
    
    // Bomba de clonagem é lançada na coordenada (x,y)
    // Os clones são trasladados em dx, dy (nos eixos x e y, respectivamente)
    // Clones devem ter identificadores únicos
    
    // Cria o polígono de visibilidade
    Poligono poligono = criarPoligono();
    Arvore arvoreSegmentos = criarArvore();
    
    // Calcula o polígono de visibilidade
    calcularVisibilidade(poligono, anteparos, formas, arvoreSegmentos, x, y, comandoSfx);
    
    // Desenha o polígono de visibilidade no SVG
    desenharPoligonoSVG(svg, poligono, "#B388FF", "#000000");
    
    // Reporta informações da clonagem no arquivo de texto
    fprintf(txt, "Clonagem na posição (%.2f, %.2f)\n", x, y);
    fprintf(txt, "Deslocamento: dx=%.2f, dy=%.2f\n", dx, dy);
    fprintf(txt, "Formas clonadas:\n");
    
    // Obtém o maior ID atual para gerar IDs únicos para os clones
    int maiorID = getMaiorIdLista(formas);
    int proximoID = maiorID + 1;
    int formasClonadas = 0;
    
    // Cria uma lista temporária para armazenar os clones
    Lista clones = criarLista();
    
    // Percorre a lista de formas verificando quais estão dentro do polígono
    for(CelulaLista celula = getPrimeiraCelulaLista(formas); celula != NULL; celula = getProximaCelulaLista(celula)) {
        Pacote pac = (Pacote)getConteudoCelula(celula);
        
        // Usa a função de colisão para verificar se a forma está dentro do polígono
        if(formaDentroPoligono(pac, poligono)) {
            char tipo = getTipoPacote(pac);
            Forma formaOriginal = getFormaPacote(pac);
            int idOriginal = getIDPacote(pac);
            
            // Cria um clone da forma com novo ID e posição transladada
            Pacote clone = criaPacote();
            setTipoPacote(clone, tipo);
            
            switch(tipo) {
                case 'c': {
                    Circulo original = (Circulo)formaOriginal;
                    double xOriginal = getCoordXCirculo(original);
                    double yOriginal = getCoordYCirculo(original);
                    double raio = getRaioCirculo(original);
                    char* corB = getCorBCirculo(original);
                    char* corP = getCorPCirculo(original);
                    
                    Circulo novoCirculo = criaCirculo(proximoID, xOriginal + dx, yOriginal + dy, raio, corB, corP);
                    setFormaPacote(clone, (Forma)novoCirculo);
                    fprintf(txt, "  - Círculo ID %d clonado para ID %d\n", idOriginal, proximoID);
                    break;
                }
                case 'r': {
                    Retangulo original = (Retangulo)formaOriginal;
                    double xOriginal = getCoordXRetangulo(original);
                    double yOriginal = getCoordYRetangulo(original);
                    double w = getWRetangulo(original);
                    double h = getHRetangulo(original);
                    char* corB = getCorBRetangulo(original);
                    char* corP = getCorPRetangulo(original);
                    
                    Retangulo novoRetangulo = criaRetangulo(proximoID, xOriginal + dx, yOriginal + dy, w, h, corB, corP);
                    setFormaPacote(clone, (Forma)novoRetangulo);
                    fprintf(txt, "  - Retângulo ID %d clonado para ID %d\n", idOriginal, proximoID);
                    break;
                }
                case 'l': {
                    Linha original = (Linha)formaOriginal;
                    double x1 = getX1Linha(original);
                    double y1 = getY1Linha(original);
                    double x2 = getX2Linha(original);
                    double y2 = getY2Linha(original);
                    char* cor = getCorLinha(original);
                    
                    Linha novaLinha = criarLinha(proximoID, x1 + dx, y1 + dy, x2 + dx, y2 + dy, cor);
                    setFormaPacote(clone, (Forma)novaLinha);
                    fprintf(txt, "  - Linha ID %d clonada para ID %d\n", idOriginal, proximoID);
                    break;
                }
                case 't': {
                    Texto original = (Texto)formaOriginal;
                    double xOriginal = getCoordXTexto(original);
                    double yOriginal = getCoordYTexto(original);
                    char* corB = getCorBTexto(original);
                    char* corP = getCorPTexto(original);
                    char ancora = getATexto(original);
                    char* texto = getTxtoTexto(original);
                    
                    Texto novoTexto = criarTexto(proximoID, xOriginal + dx, yOriginal + dy, corB, corP, ancora, texto);
                    
                    // Copia o estilo se existir
                    Estilo estiloOriginal = getEstiloTexto(original);
                    if(estiloOriginal != NULL) {
                        char* family = getfFamily(estiloOriginal);
                        char* weight = getfWeight(estiloOriginal);
                        char* size = getfSize(estiloOriginal);
                        Estilo novoEstilo = criarEstilo(family, weight, size);
                        setEstiloTexto(novoTexto, novoEstilo);
                    }
                    
                    setFormaPacote(clone, (Forma)novoTexto);
                    fprintf(txt, "  - Texto ID %d clonado para ID %d\n", idOriginal, proximoID);
                    break;
                }
            }
            
            // Adiciona o clone à lista temporária
            insereLista(clones, clone);
            proximoID++;
            formasClonadas++;
        }
    }
    
    fprintf(txt, "Total de formas clonadas: %d\n\n", formasClonadas);
    
    // Adiciona todos os clones à lista de formas original
    for(CelulaLista celula = getPrimeiraCelulaLista(clones); celula != NULL; celula = getProximaCelulaLista(celula)) {
        Pacote clone = (Pacote)getConteudoCelula(celula);
        insereLista(formas, clone);
    }
    
    // Libera apenas a lista de clones (não os pacotes, pois foram adicionados à lista de formas)
    liberaLista(clones);
    
    // Libera memória
    liberarArvore(arvoreSegmentos);
    liberarPoligono(poligono);
}

void lerQry(FILE* qry,FILE* txt, FILE* svg, Lista formas){
    if(qry == NULL || txt == NULL || svg == NULL){
        printf("Erro em lerQry\n");
        return;
    }
    char linha[50];
    char comando[50];

    Lista anteparos = criarLista();

    while(fgets(linha, sizeof(linha), qry)){
        if(linha[0] == '\n' || linha[0] == '\r'){
            continue;
        }

        sscanf(linha, "%s", comando);

        if(strcmp(comando, "a") == 0){
            int inicio, fim;
            char letra;
            sscanf(linha, "a %d %d %c", &inicio, &fim, &letra);
            comandoA(txt,formas,anteparos,inicio,fim,letra);
        }else if(strcmp(comando, "d") == 0){
            double x,y;
            char sufixo[50];
            sscanf(linha, "d %lf %lf %s", &x, &y, sufixo);
            comandoD(txt,svg,anteparos,formas,x,y,sufixo);
        }else if(strcmp(comando, "p") == 0){
            double x,y;
            char cor[50];
            char sufixo[50];
            sscanf(linha,"p %lf %lf %s %s", &x,&y,&cor,sufixo);
            comandoP(txt,svg,formas,anteparos,x,y,cor,sufixo);
        }else if(strcmp(comando, "cln") == 0){
            double x,y,dx,dy;
            char sufixo[50];
            sscanf(linha,"cln %lf %lf %lf %lf %s",&x,&y,&dx,&dy,sufixo);
            comandoCln(txt,svg,formas,anteparos,x,y,dx,dy,sufixo);
        }
    }
    liberaLista(anteparos);
}
