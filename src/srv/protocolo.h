#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)
#define TRUE 1
#define FALSE 0

typedef struct __prot__{
    char comando[10];
    char alvo[10];
    char opt[4];
    char valor[3]; //atoi
}PROTOCOLO, *PTR_PROTOCOLO;


//converte em um stream de bytes
//int serializar (PROTOCOLO *estrutura, char *stream[]);

//converte string em protocolo
void gera_protocolo (char stream[], PROTOCOLO *estrutura);

//imprime protocolo
void imprime_protocolo (PROTOCOLO *estrutura);

//validar linha == numero de caracteres válidos
int valida_linha (char stream[]);