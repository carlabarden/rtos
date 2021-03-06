//####################### SERVIDOR ########################
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "../shr/protocolo.h"

#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)

#define TRUE 1
#define FALSE 0

//para acordar thread de escrita
int flag = FALSE; //se flag == TRUE, executa escrita na rede
pthread_mutex_t c = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// para passar os parametros 
typedef struct __parametros__{
    long id;  //id da thread
    int sockfd; //file descriptor do socket srv
}ARG, *PTR_ARG;

// para cada nodo: socket de conexão (obtido através de um accept), porta desse nodo, ip desse nodo
typedef struct __nodo__ {
    int newsockfd;
    int porta;
    char ip[12];
}CONN_INFO;


CONN_INFO nodo;
char comando[30];

void *enviar_para_simulador(void *arg){
    PTR_ARG a = (PTR_ARG) arg;
    int n;
    char buffer[30];
    
    while(TRUE){
    
        bzero(buffer,sizeof(buffer));

        pthread_mutex_lock(&c);
        
        while(!flag) //ao contrário -> var cond
            pthread_cond_wait(&cond,&c);
            
        strncpy(buffer, comando,sizeof(comando));
        flag=FALSE;
        
        pthread_mutex_unlock(&c);
        
        //se o comando for sair, encerra aplicação
        //strcmp retorna 0 se forem iguais
        if (strcmp(buffer, "sair\n") == 0){
        
            //sair do simulador (cliente)
            n = write(nodo.newsockfd,buffer, sizeof(buffer));
            if (n < 0) {
                printf("Erro!\n");
                close(nodo.newsockfd);
                exit(1);
            }
            
            printf("\n\n\t Simulador OFF.\n\n");
            
            close(a->sockfd);
            printf("\n\n\t Monitor OFF.\n\n");
            exit(0);
        }
        
        //escrevendo o comando no socket  -- no cliente, chamar parser --
        n = write(nodo.newsockfd,buffer, sizeof(buffer));
        if (n < 0) {
            printf("Erro escrevendo no socket!\n");
            close(nodo.newsockfd);
            exit(1);
        }       
    }
}


void *receber_do_simulador(void *arg){

    int n;
    char buffer[30];
    
    while(TRUE){
    
        bzero(buffer,sizeof(buffer));
        
        //lendo o buffer
        n = read(nodo.newsockfd,buffer, sizeof(buffer));
        if (n <= 0) {
            if (n < 0){
                printf("Erro lendo do socket!\n");
                close(nodo.newsockfd);
                exit(1);
            }
        }
        else {
            printf("\t%s",buffer);
        }

    }
}



