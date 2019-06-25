#include "protocolo.h"

void gera_protocolo (char stream[], PROTOCOLO *estrutura){
    char aux[4][10];
    bzero(aux, sizeof(char)*4*10);
    bzero((char*) estrutura, sizeof(*estrutura));
    
    int tam_buffer = strlen(stream);   
    int i = 0;
    int j = 0; 
    int k = 0;
    
    int v = valida_linha(stream);
    
    if (v){
        for (i = 0; i < tam_buffer; i ++){
        
            if (isspace(stream[i]) || stream[i] == '\n'){
                aux[j][k] = '\0';
                j++;
                k = 0;
            }
            else {
                aux[j][k] = stream[i];
                k++;
            }
        }
        
        strncpy(estrutura->comando, aux[0], 9);
        //strcat(estrutura->comando, "\0");
        int a = strlen(estrutura->comando);
        estrutura->comando[a] = '\0';
        
        strncpy(estrutura->alvo, aux[1], 9);
        //strcat(estrutura->alvo, "\0");
        a = strlen(estrutura->alvo);
        estrutura->alvo[a] = '\0';
        
        strncpy(estrutura->opt, aux[2], 3);
        //strcat(estrutura->opt, "\0"); 
        a = strlen(estrutura->opt);
        estrutura->opt[a] = '\0';
        
        strncpy(estrutura->valor, aux[3], 2);
        //strcat(estrutura->valor, "\0"); 
        a = strlen(estrutura->valor);
        estrutura->valor[a] = '\0';  
       
       /*
        printf("\n%s, %lu\n", estrutura->comando , strlen(estrutura->comando)); 
        printf("\n%s, %lu\n", estrutura->alvo , strlen(estrutura->alvo)); 
        printf("\n%s, %lu\n", estrutura->opt , strlen(estrutura->opt)); 
        printf("\n%s, %lu\n", estrutura->valor , strlen(estrutura->valor)); 
        
        */
        

    }
    else {
        printf("Comando Inválido");
    }
}


void imprime_protocolo (PROTOCOLO *estrutura){
    
    printf("COMANDO   ALVO      OPÇÃO     VALOR\n");
    printf("%7s %7s %10s %5s\n",
            estrutura->comando, estrutura->alvo, estrutura->opt, estrutura->valor);
   /*
    printf("%lu   %s\n", strlen(estrutura->comando), estrutura->comando);
    printf("%lu   %s\n", strlen(estrutura->alvo), estrutura->alvo);
    printf("%lu   %s\n", strlen(estrutura->opt), estrutura->opt);
    printf("%lu   %s\n", strlen(estrutura->valor), estrutura->valor);
    */
   
    
}

int valida_linha (char stream[]){

    int tam_buffer = strlen(stream);   
    int i = 0;
    int j = 0;//conta palavras 
    int k = 0;//conta caracteres
    
    //evitar buffer overflow
    if (tam_buffer > 27){
        //printf ("\tEntrada Inválida\n");
        return  FALSE;
    }
    
    for (i = 0; i < tam_buffer; i ++){
        
        if(isalnum(stream[i])){
            k++;
        }
        
        if (stream[i] == ' ' || stream[i] == '\n' || stream[i] == '\0'){
            j++;
            
            if (j == 1 && k > 9){
                //printf ("\tEntrada Inválida\n");
                return  FALSE;
            }
            if (j == 2 && k > 9){
                //printf ("\tEntrada Inválida\n");
                return  FALSE;
            }
            if (j == 3 && k > 3){
                //printf ("\tEntrada Inválida\n");
                return  FALSE;
            }
            if (j == 4 && k > 2){
                //printf ("\tEntrada Inválida\n");
                return  FALSE;
            }

            k = 0;
        }
    }
    
    return TRUE;    
}


