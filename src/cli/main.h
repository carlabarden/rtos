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

#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)

#define TRUE 1
#define FALSE 0

int sockfd;
char comando[50];
pthread_mutex_t c = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

//TODO: parser, protocolo   


void *receber_do_monitor(void *arg) {
    char buffer[50];
    int n;
    //printf("Início\n");
    while (TRUE) {
        bzero(buffer,sizeof(buffer));
        n = read(sockfd,buffer,50);
        if (n <= 0) {
            printf("Erro lendo do socket!\n");
            close(sockfd);
            exit(1);
        }
        
        //pthread_mutex_lock(&c);
        strcpy(comando, buffer);
        //pthread_mutex_unlock(&c);
        printf("Comando:  %s", buffer);
        
        if (strcmp(buffer,"sair\n") == 0) {
            printf("\n\n\t Simulador OFF.\n\n");
            close(sockfd);
            exit(0);
        }
    }
}
///*
        //TODO:: var cond pra quando chegar comando,acordar (???)
void *enviar_para_monitor(void *arg){
    
    char buffer[50];
    int n;

    while(TRUE){

        bzero(buffer, sizeof(buffer));
    
        n = write(sockfd, buffer, 50);
        if (n < 0) {
            printf("Erro escrevendo no socket!\n");
            close(sockfd);
            exit(1);
        }
    }    
}
//*/