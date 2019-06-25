//####################### CLIENTE ########################
#pragma once

#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>      // inet_aton
#include <pthread.h>
//#include "../shr/protocolo.h"
#include "simulador.h"

#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)

#define TRUE 1
#define FALSE 0

// ####################### SIMULADOR #########################
// ########## ESTADOS ###########
// Temperatura varia entre 17 e 24
// 0 para ar condicionado desligado
// temperatura ajustada pelo usuário
int temperatura = 20;

// Luz varia entre 0 e 2
// 0 == desligada
// 1 == ligada, intensidade máxima
// 2 == ligada, meia luz
int luz = 0;

// Porta pode estar aberta ou fechada
// 0 = fechada
// 1 = aberta
int porta = 0;

// Biometria, para facilitar a implementação, será uma sequencia numérica nesta simulação
unsigned int biometria = 12;

// Janela pode estar aberta ou fechada
// 0 = fechada
// 1 = aberta
int janela = 0;

// Cortinas podem estar abertas, fechadas ou semi-abertas
// 0 = fechada
// 1 = aberta
// 2 = semiaberta
int cortinas = 0;

// Sensores
// Sensor de presença, para a iluminação
// 0 == FALSE == ninguém
// 1 == TRUE == alguém entrou
int sensor_presenca = FALSE;

// Sensor de biometria, para abrir a porta
// 0 == FALSE == leitura não detectada
// !=0 == TRUE == leitura detectada, valor na própria variável
int sensor_biometria = FALSE; 

// Sensor da leitura da temperatura
// 0 == FALSE == ar condicionado desligado
// !=0 == TRUE == leitura do sensor
int sensor_temperatura = 0;


// Comando recebido do monitor
PROTOCOLO comando;

// Para exclusão mútua na atualização das variáveis globais pelo usuario
pthread_mutex_t comm = PTHREAD_MUTEX_INITIALIZER;

// Para exclusão mútua na atualização das variáveis globais pelo programa
pthread_mutex_t prog = PTHREAD_MUTEX_INITIALIZER;

// Se o comando executou corretamente ou não -- Retornar para monitor
char retorno[30] = "\0";


//espera ter comando para poder executar
int flag_executar_comando = FALSE;
pthread_mutex_t exec      = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond_exec = PTHREAD_COND_INITIALIZER;

//espera precisar escrever para acordar escrita
int flag_escrita_rede = FALSE;
pthread_mutex_t c = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond_esc = PTHREAD_COND_INITIALIZER;


//############################## REDE ##############################
int sockfd;

/*  Threads responsáveis pela leitura e escrita na rede.
*   A leitura roda indefinidamente, a escrita, por ser bloqueante, 
*  só acorda quando é necessário (via variáveis de condição).
*/
void *receber_do_monitor(void *arg) {
    
    char buffer[30];
    int n;

    while (TRUE) {
    
        bzero(buffer,sizeof(buffer));
        
        //lendo o buffer
        n = read(sockfd,buffer,sizeof(buffer));
        if (n <= 0) {
            if (n < 0){
                printf("Erro lendo do socket!\n");
                close(sockfd);
                exit(1);
            }
        }
        else { 
            
            pthread_mutex_lock(&exec);
            gera_protocolo(buffer,&comando);
            flag_executar_comando = TRUE; 
            pthread_cond_signal(&cond_exec);
            pthread_mutex_unlock(&exec);
            
            printf("\n\n\t Comando recebido:\n\n");
            imprime_protocolo(&comando);
            printf("\n\n");
            
            if (strcmp(buffer,"sair\n") == 0) {
                printf("\n\n\t Simulador OFF.\n\n");
                close(sockfd);
                exit(0);
            }
        }
    }
}


void *enviar_para_monitor(void *arg){
    
    char buffer[30];
    int n;

    while(TRUE){

        bzero(buffer, sizeof(buffer));
        
        
        pthread_mutex_lock(&c);
        
        while(!flag_escrita_rede){  //ao contrário -> var cond
        
            pthread_cond_wait(&cond_esc,&c);
        }
        
        strcpy(buffer, retorno);
        
        flag_escrita_rede = FALSE;
        pthread_mutex_unlock(&c);
    
        n = write(sockfd, buffer, sizeof(buffer));
        if (n < 0) {
            printf("Erro escrevendo no socket!\n");
            close(sockfd);
            exit(1);
        }
    }    
}

//#################### Comando Usuário #############################
/* Thread que fica dormente até que seja recebido um comando válido.
*  Quando isso acontece, ela acorda, tenta a execução do comando e 
* dorme novamente (para não ocorrer espera ocupada). Se ela não con-
* seguir executar o comando, retorna FALSE, para poder enviar o 
* retorno ao servidor.
*/


void *executar_comando_usuario(void *arg){
    
    while(TRUE){
        
        pthread_mutex_lock(&exec);
        while(!flag_executar_comando){//ao contrário -> var cond
        
            pthread_cond_wait(&cond_exec,&exec);
        }
        
        executar_comando();
        printf("\n\t USER: %s \n\n", retorno);
        
        //esperar novo comando
        flag_executar_comando  = FALSE;
        pthread_mutex_unlock(&exec);
        
        //ativar escrita na rede
        pthread_mutex_lock(&c);
        flag_escrita_rede = TRUE;
        pthread_cond_signal(&cond_esc);
        pthread_mutex_unlock(&c);
    }
}

//############################# TIMER ##############################
/*  Timer responsável pelas tarefas periódicas. 
*   
*/

struct periodic_info{
    int sig;    
    sigset_t alarm_sig;  
};

static int make_periodic (int unsigned period, struct periodic_info *info){

    static int next_sig;
    int ret;
    unsigned int ns;
    unsigned int sec;
    struct sigevent sigev;
    timer_t timer_id;
    struct itimerspec itval;

    /* Initialise next_sig first time through. We can't use static
       initialisation because SIGRTMIN is a function call, not a constant */
    if (next_sig == 0)
        next_sig = SIGRTMIN;
    /* Check that we have not run out of signals */
    if (next_sig > SIGRTMAX)
        return -1;
    info->sig = next_sig;
    next_sig++;
    /* Create the signal mask that will be used in wait_period */
    sigemptyset (&(info->alarm_sig));
    sigaddset (&(info->alarm_sig), info->sig);

    /* Create a timer that will generate the signal we have chosen */
    sigev.sigev_notify = SIGEV_SIGNAL;
    sigev.sigev_signo = info->sig;
    sigev.sigev_value.sival_ptr = (void *) &timer_id;
    ret = timer_create (CLOCK_MONOTONIC, &sigev, &timer_id);
    if (ret == -1)
        return ret;

    /* Make the timer periodic */
    sec = period/1000000;
    ns = (period - (sec * 1000000)) * 1000;
    itval.it_interval.tv_sec = sec;
    itval.it_interval.tv_nsec = ns;
    itval.it_value.tv_sec = sec;
    itval.it_value.tv_nsec = ns;
    ret = timer_settime (timer_id, 0, &itval, NULL);
    return ret;
}

static void wait_period (struct periodic_info *info){

    int sig;
    sigwait (&(info->alarm_sig), &sig);
}


/*   As tarefas periódicas necessárias para a leitura dos sensores.
* Se a leitura feita diferir dos valores setados pelo usuário, será feito o ajuste 
* necessário aqui mesmo. */

/*  Se há a biometria correta, abre a porta*/
static void *ler_biometria (void *arg){

    struct periodic_info info;

    //periodo = 1s
    make_periodic (1000000, &info);
    while (TRUE){
        wait_period (&info);
        ler_sensor_biometria();
        printf("\t Leitura Sensor Biometria: %d \n", sensor_biometria);
        
        // Se é a biometria do usuário
        if (sensor_biometria == biometria){
            
           abrir_porta();
           printf("\n\t AUTO: Porta Aberta \n\n");
           sleep(3);
           fechar_porta(); 
           printf("\n\t AUTO: Porta Fechada \n\n");
           
           pthread_mutex_lock(&prog);
           sensor_biometria = 0;
           pthread_mutex_unlock(&prog);
           
        }
        else{
        
           pthread_mutex_lock(&prog);
           sensor_biometria = 0;
           pthread_mutex_unlock(&prog);
        }
    }
    
   // retorno[0] = "\0";
    return NULL;
}

/*  Se o sensor de presença captar alguem, acende a luz;*/
static void *ler_presenca (void *arg){
    struct periodic_info info;

   //período = 1s
    make_periodic (1000000, &info);
    while (TRUE){
    
        wait_period (&info);
        ler_sensor_presenca();
        printf("\t Leitura Sensor Presença: %d\n", sensor_presenca);
        
        if (sensor_presenca == TRUE && luz == 0){
            ligar_luz();
            printf("\n\t AUTO: Luz Ligada \n\n");
        } 
        else if (sensor_presenca == FALSE && (luz != 0)){
            desligar_luz();
            printf("\n\t AUTO: Luz Desligada \n\n");
        }
       
    }
    //retorno[0] = "\0";
    return NULL;
}

/*  Temperatura padrão: 20 graus*/
static void *ler_temperatura (void *arg){

    struct periodic_info info;
    
    //guardar temperatura setada pelo user
    int temperatura_usuario = temperatura;
    
    //período = 5s
    make_periodic (5000000, &info);
    while (TRUE){
    
        wait_period (&info);
        ler_sensor_temperatura();
        printf("\t Leitura Sensor Temperatura: %d \n", temperatura);
        
        if (sensor_temperatura != temperatura_usuario){
            pthread_mutex_lock(&prog);
            temperatura = temperatura_usuario;
            pthread_mutex_unlock(&prog);
            printf("\n\t AUTO: Temperatura ajustada para: %d \n\n", temperatura);
        }
        
    }
   // retorno[0] = "\0";
    return NULL;
}