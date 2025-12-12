#include "qry.h"

void gerarArena(FILE* svgEntrada, Lista pacotes, Lista anteparos){
    if(svgEntrada == NULL || pacotes == NULL || anteparos == NULL){
        printf("Erro em gerarArena: parametros invalidos\n");
        return;
    }
    
    double menorXPacotes = getMenorValorLista(pacotes, 1);
    double menorXAnteparos = getMenorValorLista(anteparos, 1);
    double menorX = (menorXPacotes < menorXAnteparos) ? menorXPacotes : menorXAnteparos;
    
    double menorYPacotes = getMenorValorLista(pacotes, 2);
    double menorYAnteparos = getMenorValorLista(anteparos, 2);
    double menorY = (menorYPacotes < menorYAnteparos) ? menorYPacotes : menorYAnteparos;
    
    double maiorXPacotes = getMaiorValorLista(pacotes, 1);
    double maiorXAnteparos = getMaiorValorLista(anteparos, 1);
    double maiorX = (maiorXPacotes > maiorXAnteparos) ? maiorXPacotes : maiorXAnteparos;

    double maiorYPacotes = getMaiorValorLista(pacotes, 2);
    double maiorYAnteparos = getMaiorValorLista(anteparos, 2);
    double maiorY = (maiorYPacotes > maiorYAnteparos) ? maiorYPacotes : maiorYAnteparos;

    Retangulo arena = criaRetangulo(-100, menorX, menorY, maiorX, maiorY, "#000000", "#000000");
    if(arena == NULL){
        printf("Erro em gerarArena: falha ao criar retangulo\n");
        return;
    }
    
    desenharRetanguloSVG(svgEntrada, arena);
    liberaRetangulo(arena);
}

void comandoA(FILE* arqTxt, Lista formas, Lista anteparos, int inicio, int fim, char letra){
    if(arqTxt == NULL || formas == NULL || anteparos == NULL){
        printf("Erro em comandoA: parametros invalidos\n");
        return;
    }

    if(letra != 'h' && letra != 'v'){
        printf("Erro em comandoA: letra de comando invalida\n");
        return;
    }

    for(int contador = inicio; contador <= fim; contador++){
        Pacote pac = procuraPacoteLista(formas, contador);
        if(pac == NULL){
            printf("Pacote não encontrado em comandoA\n");
            continue;
        }

        char tipo = getTipoPacote(pac);
        Forma formaPac = getFormaPacote(pac);
        int id = getIDPacote(pac);
        
        if(tipo == 'c'){
            double x = getCoordXCirculo((Circulo)formaPac);
            double y = getCoordYCirculo((Circulo)formaPac);
            double raio = getRaioCirculo((Circulo)formaPac);
            char* cor = getCorPCirculo((Circulo)formaPac);

            Linha lin = NULL;
            if(letra == 'h'){
                lin = criarLinha(id, x - raio, y, x + raio, y, cor);
            } else {
                lin = criarLinha(id, x, y - raio, x, y + raio, cor);
            }
            if(lin == NULL){
                printf("Erro em comandoA: falha ao criar linha\n");
                continue;
            }

            Pacote transferPac = removeRetornaConteudo(formas, id);
            if(transferPac != NULL){
                liberaCirculo((Circulo)formaPac);
                setTipoPacote(transferPac, 'l');
                setFormaPacote(transferPac, (Forma)lin);
                insereLista(anteparos, transferPac);
                fprintf(arqTxt, "id do círculo: %d, figura original: círculo\n", id);
            } else {
                liberaLinha(lin);
            }
            
        } else if(tipo == 'r'){
            double x = getCoordXRetangulo((Retangulo)formaPac);
            double y = getCoordYRetangulo((Retangulo)formaPac);
            double w = getWRetangulo((Retangulo)formaPac);
            double h = getHRetangulo((Retangulo)formaPac);
            char* cor = getCorPRetangulo((Retangulo)formaPac);

            int maiorIDFormas = getMaiorIdLista(formas);
            int maiorIDAnteparos = getMaiorIdLista(anteparos);
            int maior = (maiorIDAnteparos > maiorIDFormas) ? maiorIDAnteparos : maiorIDFormas;
            if(maior < 0) maior = 0;

            Linha esquerda = criarLinha(maior + 1, x, y, x, y + h, cor);
            Linha direita = criarLinha(maior + 2, x + w, y, x + w, y + h, cor);
            Linha cima = criarLinha(maior + 3, x, y, x + w, y, cor);
            Linha baixo = criarLinha(maior + 4, x, y + h, x + w, y + h, cor);

            if(esquerda == NULL || direita == NULL || cima == NULL || baixo == NULL){
                printf("Erro em comandoA: falha ao criar linhas do retangulo\n");
                if(esquerda) liberaLinha(esquerda);
                if(direita) liberaLinha(direita);
                if(cima) liberaLinha(cima);
                if(baixo) liberaLinha(baixo);
                continue;
            }

            Pacote esq = criaPacote();
            Pacote dir = criaPacote();
            Pacote ci = criaPacote();
            Pacote ba = criaPacote();

            if(esq == NULL || dir == NULL || ci == NULL || ba == NULL){
                printf("Erro em comandoA: falha ao criar pacotes\n");
                liberaLinha(esquerda);
                liberaLinha(direita);
                liberaLinha(cima);
                liberaLinha(baixo);
                if(esq) liberarPacote(esq);
                if(dir) liberarPacote(dir);
                if(ci) liberarPacote(ci);
                if(ba) liberarPacote(ba);
                continue;
            }

            setTipoPacote(esq, 'l');
            setFormaPacote(esq, (Forma)esquerda);
            insereLista(anteparos, esq);

            setTipoPacote(dir, 'l');
            setFormaPacote(dir, (Forma)direita);
            insereLista(anteparos, dir);

            setTipoPacote(ci, 'l');
            setFormaPacote(ci, (Forma)cima);
            insereLista(anteparos, ci);

            setTipoPacote(ba, 'l');
            setFormaPacote(ba, (Forma)baixo);
            insereLista(anteparos, ba);

            fprintf(arqTxt, "ID do retangulo: %d, id do segmento da esquerda: %d, id do segmento da direita: %d, id do segmento de cima: %d, id do segmento de baixo: %d, figura original: Retângulo\n",
                id, maior + 1, maior + 2, maior + 3, maior + 4);

            Pacote pacRetangulo = removeRetornaConteudo(formas,id);
            if(pacRetangulo!=NULL){
                // liberaRetangulo((Retangulo)formaPac);
                liberarPacote(pacRetangulo);
            }
            
        } else if(tipo == 't'){
            char ancora = getATexto((Texto)formaPac);
            int tamanho = strlen(getTxtoTexto((Texto)formaPac));
            double x = getCoordXTexto((Texto)formaPac);
            double y = getCoordYTexto((Texto)formaPac);
            char* cor = getCorPTexto((Texto)formaPac);
            
            double x1, y1, x2, y2;

            if(ancora == 'i'){
                x1 = x;
                y1 = y;
                x2 = x + 10.0 * tamanho;
                y2 = y;
            } else if(ancora == 'f'){
                x1 = x - 10.0 * tamanho;
                y1 = y;
                x2 = x;
                y2 = y;
            } else if(ancora == 'm'){
                x1 = x - 10.0 * tamanho / 2;
                y1 = y;
                x2 = x + 10.0 * tamanho / 2;
                y2 = y;
            } else {
                printf("Erro em comandoA: ancora invalida\n");
                continue;
            }

            Linha posTxt = criarLinha(id, x1, y1, x2, y2, cor);
            if(posTxt == NULL){
                printf("Erro em comandoA: falha ao criar linha do texto\n");
                continue;
            }

            Pacote removedPac = removeRetornaConteudo(formas, id);
            if(removedPac != NULL){
                liberaTexto((Texto)formaPac);
                setTipoPacote(removedPac, 'l');
                setFormaPacote(removedPac, (Forma)posTxt);
                insereLista(anteparos, removedPac);
                fprintf(arqTxt, "Id do texto: %d, figura original: Texto\n", id);
            } else {
                liberaLinha(posTxt);
            }
        }
    }
}

void comandoD(FILE* arqTxt, FILE* svgSfx, Lista anteparos, Lista formas, double xBomba, double yBomba){
    if(arqTxt == NULL || svgSfx == NULL || anteparos == NULL || formas == NULL){
        printf("Erro em comandoD: parametros invalidos\n");
        return;
    }
    
    fprintf(stderr, "\n=== DEBUG comandoD: Início ===\n");
    fprintf(stderr, "Bomba em (%.2f, %.2f)\n", xBomba, yBomba);
    
    Poligono poligono = criarPoligono();
    if(poligono == NULL){
        printf("Erro em comandoD: falha ao criar poligono\n");
        return;
    }
    
    calcularVisibilidade(poligono, anteparos, xBomba, yBomba, "");
    
    int numVertices = getNumeroVertices(poligono);
    fprintf(stderr, "Polígono criado com %d vértices\n", numVertices);
    
    desenharPoligonoSVG(svgSfx, poligono, "#FF8A80", "#000000");
    
    fprintf(arqTxt, "Explosão na posição (%.2f, %.2f)\n", xBomba, yBomba);
    
    int totalFormas = getTamanhoLista(formas);
    fprintf(stderr, "Total de formas na lista: %d\n", totalFormas);
    
    int formasDestruidas = 0;
    int formasVerificadas = 0;
    CelulaLista celula = getPrimeiraCelulaLista(formas);
    
    while(celula != NULL){
        Pacote pac = (Pacote)getConteudoCelula(celula);
        CelulaLista proximaCelula = getProximaCelulaLista(celula);
        
        if(pac == NULL){
            fprintf(stderr, "AVISO: Pacote NULL encontrado na lista\n");
            celula = proximaCelula;
            continue;
        }
        
        char tipo = getTipoPacote(pac);
        int id = getIDPacote(pac);
        formasVerificadas++;
        
        fprintf(stderr, "Verificando forma ID %d (Tipo: %c)... ", id, tipo);
        
        bool dentro = formaDentroPoligono(pac, poligono);
        fprintf(stderr, "Dentro? %s\n", dentro ? "SIM" : "NÃO");
        
        if(dentro){
            fprintf(arqTxt, " Características da forma presente dentro do polígono de visibilidade: Tipo: %c, ID: %d\n", tipo, id);
            formasDestruidas++;
            
            fprintf(stderr, "  -> Tentando remover ID %d...\n", id);
            removeLista(formas, id);
            fprintf(stderr, "  -> Remoção concluída\n");
        }
        
        celula = proximaCelula;
    }
    
    fprintf(stderr, "Formas verificadas: %d\n", formasVerificadas);
    fprintf(stderr, "Formas destruídas: %d\n", formasDestruidas);
    fprintf(stderr, "=== DEBUG comandoD: Fim ===\n\n");
    
    fprintf(arqTxt, "Total de formas destruídas: %d\n\n", formasDestruidas);
    liberarPoligono(poligono);
}

void comandoP(FILE* txt, FILE* svg, Lista formas, Lista anteparos, double x, double y, char* cor){
    if(txt == NULL || svg == NULL || formas == NULL || anteparos == NULL || cor == NULL){
        printf("Erro em comandoP: parametros invalidos\n");
        return;
    }
    
    Poligono poligono = criarPoligono();
    if(poligono == NULL){
        printf("Erro em comandoP: falha ao criar poligono\n");
        return;
    }
    
    calcularVisibilidade(poligono, anteparos, x, y, "");
    
    Lista formasAtingidas = obterFormasAtingidas(formas, poligono);
    
    if(formasAtingidas != NULL){
        int numAtingidas = getTamanhoLista(formasAtingidas);
        
        fprintf(txt, "Bomba de pintura lançada em (%.2f, %.2f)\n", x, y);
        fprintf(txt, "Cor aplicada: %s\n", cor);
        fprintf(txt, "Número de formas atingidas: %d\n", numAtingidas);
        
        for(CelulaLista celula = getPrimeiraCelulaLista(formasAtingidas); 
            celula != NULL; 
            celula = getProximaCelulaLista(celula)){
            
            Pacote pac = (Pacote)getConteudoCelula(celula);
            char tipo = getTipoPacote(pac);
            Forma forma = getFormaPacote(pac);
            int id = getIDPacote(pac);
            
            switch(tipo){
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
        
        liberaLista(formasAtingidas);
    }
    
    desenharPoligonoSVG(svg, poligono, "#80D8FF", "#000000");
    liberarPoligono(poligono);
}

void comandoCln(FILE* txt, FILE* svg, Lista formas, Lista anteparos, double x, double y, double dx, double dy){
    if(txt == NULL || svg == NULL || formas == NULL || anteparos == NULL){
        printf("Erro em comandoCln: parametros invalidos\n");
        return;
    }
    
    Poligono poligono = criarPoligono();
    if(poligono == NULL){
        printf("Erro em comandoCln: falha ao criar poligono\n");
        return;
    }
    
    calcularVisibilidade(poligono, anteparos, x, y, "");
    desenharPoligonoSVG(svg, poligono, "#B388FF", "#000000");
    
    fprintf(txt, "Clonagem na posição (%.2f, %.2f)\n", x, y);
    fprintf(txt, "Deslocamento: dx=%.2f, dy=%.2f\n", dx, dy);
    fprintf(txt, "Formas clonadas:\n");
    
    int maiorID = getMaiorIdLista(formas);
    int proximoID = maiorID + 1;
    int formasClonadas = 0;
    
    Lista clones = criarLista();
    if(clones == NULL){
        printf("Erro em comandoCln: falha ao criar lista de clones\n");
        liberarPoligono(poligono);
        return;
    }
    
    for(CelulaLista celula = getPrimeiraCelulaLista(formas); 
        celula != NULL; 
        celula = getProximaCelulaLista(celula)){
        
        Pacote pac = (Pacote)getConteudoCelula(celula);
        
        if(formaDentroPoligono(pac, poligono)){
            char tipo = getTipoPacote(pac);
            Forma formaOriginal = getFormaPacote(pac);
            int idOriginal = getIDPacote(pac);
            
            Pacote clone = criaPacote();
            if(clone == NULL){
                printf("Erro em comandoCln: falha ao criar pacote clone\n");
                continue;
            }
            
            setTipoPacote(clone, tipo);
            Forma novaForma = NULL;
            
            switch(tipo){
                case 'c': {
                    Circulo original = (Circulo)formaOriginal;
                    double xOriginal = getCoordXCirculo(original);
                    double yOriginal = getCoordYCirculo(original);
                    double raio = getRaioCirculo(original);
                    char* corB = getCorBCirculo(original);
                    char* corP = getCorPCirculo(original);
                    
                    novaForma = (Forma)criaCirculo(proximoID, xOriginal + dx, yOriginal + dy, raio, corB, corP);
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
                    
                    novaForma = (Forma)criaRetangulo(proximoID, xOriginal + dx, yOriginal + dy, w, h, corB, corP);
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
                    
                    novaForma = (Forma)criarLinha(proximoID, x1 + dx, y1 + dy, x2 + dx, y2 + dy, cor);
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
                    
                    Estilo estiloOriginal = getEstiloTexto(original);
                    if(estiloOriginal != NULL){
                        char* family = getfFamily(estiloOriginal);
                        char* weight = getfWeight(estiloOriginal);
                        char* size = getfSize(estiloOriginal);
                        Estilo novoEstilo = criarEstilo(family, weight, size);
                        setEstiloTexto(novoTexto, novoEstilo);
                    }
                    
                    novaForma = (Forma)novoTexto;
                    fprintf(txt, "  - Texto ID %d clonado para ID %d\n", idOriginal, proximoID);
                    break;
                }
            }
            
            if(novaForma != NULL){
                setFormaPacote(clone, novaForma);
                insereLista(clones, clone);
                proximoID++;
                formasClonadas++;
            } else {
                liberarPacote(clone);
            }
        }
    }
    
    fprintf(txt, "Total de formas clonadas: %d\n\n", formasClonadas);
    
    for(CelulaLista celula = getPrimeiraCelulaLista(clones); 
        celula != NULL; 
        celula = getProximaCelulaLista(celula)){
        Pacote clone = (Pacote)getConteudoCelula(celula);
        insereLista(formas, clone);
    }
    
    liberaLista(clones);
    liberarPoligono(poligono);
}

void lerQry(FILE* qry, FILE* txt, FILE* svg, Lista formas){
    if(qry == NULL || txt == NULL || svg == NULL || formas == NULL){
        printf("Erro em lerQry: parametros invalidos\n");
        return;
    }
    
    char linha[256];
    char comando[50];

    Lista anteparos = criarLista();
    if(anteparos == NULL){
        printf("Erro em lerQry: falha ao criar lista de anteparos\n");
        return;
    }

    while(fgets(linha, sizeof(linha), qry)){
        if(linha[0] == '\n' || linha[0] == '\r'){
            continue;
        }
        if(sscanf(linha, "%s", comando) != 1){
            continue;
        }
        if(strcmp(comando, "a") == 0){
            int inicio, fim;
            char letra;
            if(sscanf(linha, "a %d %d %c", &inicio, &fim, &letra) == 3){
                comandoA(txt, formas, anteparos, inicio, fim, letra);
            }
        } else if(strcmp(comando, "d") == 0){
            double x, y;
            char sufixo[50];
            if(sscanf(linha, "d %lf %lf %s", &x, &y, sufixo) == 3){
                if(strcmp(sufixo, "-") != 0){
                    FILE* svgSfx = fopen(sufixo, "w");
                    if(svgSfx == NULL){
                        printf("Erro ao abrir arquivo svgSfx em comando d\n");
                        return;
                    }
                    comandoD(txt, svgSfx, anteparos, formas, x, y);
                }
                comandoD(txt, svg,anteparos,formas,x,y);
            }
        } else if(strcmp(comando, "p") == 0){
            double x, y;
            char cor[50];
            char sufixo[50];
            if(sscanf(linha, "p %lf %lf %s %s", &x, &y, cor, sufixo) == 4){
                if(strcmp(sufixo, "-") != 0){
                    FILE* svgSfx = fopen(sufixo, "w");
                    if(svgSfx == NULL){
                        printf("Erro ao abrir arquivo svgSfx em comando p\n");
                        return;
                    }
                    comandoP(txt, svgSfx, formas, anteparos, x, y, cor);
                }
                comandoP(txt,svg,formas,anteparos,x,y,cor);
            }
        } else if(strcmp(comando, "cln") == 0){
            double x, y, dx, dy;
            char sufixo[50];
            if(sscanf(linha, "cln %lf %lf %lf %lf %s", &x, &y, &dx, &dy, sufixo) == 5){
                if(strcmp(sufixo, "-") != 0){
                    FILE* svgSfx = fopen(sufixo, "w");
                    if(svgSfx == NULL){
                        printf("Erro ao abrir arquivo svgSfx em comando cln\n");
                        return;
                    }
                    comandoCln(txt, svgSfx, formas, anteparos, x, y, dx, dy);
                }
                comandoCln(txt,svg,formas,anteparos,x,y,dx,dy);
            }
        }
    }
    
    for(CelulaLista atual = getPrimeiraCelulaLista(formas); 
        atual != NULL; 
        atual = getProximaCelulaLista(atual)){
        
        Pacote pac = (Pacote)getConteudoCelula(atual);
        char tipo = getTipoPacote(pac);
        
        switch(tipo){
            case 'c':
                desenharCirculoSVG(svg, (Circulo)getFormaPacote(pac));
                break;
            case 'r':
                desenharRetanguloSVG(svg, (Retangulo)getFormaPacote(pac));
                break;
            case 'l':
                desenharLinhaSVG(svg, (Linha)getFormaPacote(pac));
                break;
            case 't':
                desenharTextoSVG(svg, (Texto)getFormaPacote(pac), NULL);
                break;
        }
    }
    
    liberaLista(anteparos);
}