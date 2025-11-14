#ifndef PACOTE_H
#define PACOTE_H

/* 
    Arquivo .h destinado à criação e utilização de pacotes genéricos que contém formas e tipos
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void* Pacote;
typedef void* Forma;

/*
    Cria e retorna um pacote vazio.
*/
Pacote criaPacote();

/*
    Retorna a forma contida no pacote.
*/
Forma getFormaPacote(Pacote pac);

/*
    Retorna o tipo da forma contida no pacote.
*/
char getTipoPacote(Pacote pac);

/*
    Define a forma em um pacote.
*/
void setFormaPacote(Pacote pac, Forma form);

/*
    Define o tipo do pacote.
*/
void setTipoPacote(Pacote pac, char type);

#endif