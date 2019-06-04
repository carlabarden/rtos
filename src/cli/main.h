//####################### CLIENTE ########################
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>		// inet_aton
#include <pthread.h>
#include "protocolo.h"

#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)

#define TRUE 1
#define FALSE 0

int sockfd;
PROTOCOLO comando;
//char retorno[50] = "ok\n"; //se o comando executou corretamente ou não
int retorno = FALSE;

//espera receber comando para acordar escrita
int flag = FALSE;
pthread_mutex_t c = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

//TODO: tarefas periodicas, estados 


void *receber_do_monitor(void *arg) {
    
    char buffer[50];
    int n;

    while (TRUE) {
    
        bzero(buffer,50);
        
        //lendo o buffer
        n = read(sockfd,buffer,50);
        if (n <= 0) {
            if (n < 0){
                printf("Erro lendo do socket!\n");
                close(sockfd);
                exit(1);
            }
        }
        else {  
        
            pthread_mutex_lock(&c);
            gera_protocolo(buffer,&comando);
            //strcpy(comando, buffer);
            flag = TRUE;
            pthread_cond_signal(&cond);
            pthread_mutex_unlock(&c);
            
            //printf("Comando:  %s", comando);
            imprime_protocolo(&comando);
            
            // mudar para execucao_comando
            if (strcmp(buffer,"sair\n") == 0) {
                printf("\n\n\t Simulador OFF.\n\n");
                close(sockfd);
                exit(0);
            }
        }
    }
}


void *enviar_para_monitor(void *arg){
    
    char buffer[50];
    int n;

    while(TRUE){

        bzero(buffer, sizeof(buffer));
        
        
        pthread_mutex_lock(&c);
        
        while(!flag){//ao contrário -> var cond
            pthread_cond_wait(&cond,&c);
        }
        //if (valida_comando){}
        //por enquanto
       /* if (comando.comando[1] != '\n' && comando.comando[1] && '\0' && comando.comando[1] != ' '){
        
            char aux[5] = "ok\n";
            strcpy(buffer, aux);
        }
        else{*/
        
            char aux[5] = "ok\n";
            strcpy(buffer, aux);
      //  }
        
        flag=FALSE;
        pthread_mutex_unlock(&c);
    
        n = write(sockfd, buffer, 50);
        if (n < 0) {
            printf("Erro escrevendo no socket!\n");
            close(sockfd);
            exit(1);
        }
    }    
}


