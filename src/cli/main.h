//############################### CLIENTE ###################################
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
#include "simulador.h"

#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)

#define TRUE 1
#define FALSE 0

// ########################## SIMULADOR #####################################
//      Todos os valores serão inicializados na função inicia_sistemas.
//  *
//  *
//  *
//                  Controle de Temperatura
//  *
//  temperatura         ==  temperatura ajustada pelo usuário
//  sensor_temperatura  ==  leitura do sensor de temperatura
//  m_temp              ==  mutex da temperatura
//  m_stemp             ==  mutex do sensor_temperatura
//  *
//  Temperatura do ar condicionado varia entre 17 e 24 graus
//  Como a temperatura com o ar ligado tem um intervalo fixo, não há 
//  necessidade de ter uma variável específica para salvar o estado da
//  temperatura, como há no caso da iluminação.
//  temperatura = 0            ==  ar condicionado desligado
//  temperatura entre 17 e 24   ==  ar condicionado ligado 
//  temperatura = 0 indica que o usuário ainda não ajustou suas preferências.
//  Valor padrão é temperatura = 0 (desligado, sem pref do user)
int temperatura;
//  *
//  para garantir a exclusão mútua
pthread_mutex_t m_temp = PTHREAD_MUTEX_INITIALIZER;
//  *
//  Para armazenar a leitura do sensor de temperatura
//  Valor padrão é sensor_temperatura = aleatorio entre -5 e 45;
int sensor_temperatura;
//  *
//  para garantir a exclusão mútua
pthread_mutex_t m_stemp = PTHREAD_MUTEX_INITIALIZER;
//  *
//  *
//  *
//                  Controle de Iluminação
//  *
//  luz                 ==  iluminação atual
//  sensor_presenca     ==  leitura do sensor de presenca
//  m_luz               ==  mutex de luz
//  m_spresenca         ==  mutex de sensor_presenca
//  *
//  A luz armazena o estado atual da iluminação. A iluminação pode tanto ter
//  sido ajustada pelo usuário (valores positivos) quanto pelo programa
//  (valores negativos). Os valores que indicam o estado são simétricos, para
//  que através do cálculo do valor absoluto do estado, saiba-se qual é a 
//  iluminação, independente de quem (usuário ou programa) a ajustou. Saber quem
//  ajustou a iluminação é necessário, por exemplo, caso o usuário desligue a luz.
//  Mesmo que o sensor de presença indique alguém, a luz não deverá ser acesa.
//  A luz pode estar ligada em 100% da intensidade, ligada em 50% da intensidade
//  (meia luz) ou desligada.
//  *
//  luz = -2        ==  luz ligada pelo programa, 100% de intensidade
//  luz = -1        ==  luz desligada pelo programa
//  luz = 0         ==  luz desligada (padrão)
//  luz = 1         ==  luz desligada pelo usuário
//  luz = 2         ==  luz ligada pelo usuário, 100% de intensidade
//  luz = 3         ==  luz ligada pelo usuário, 50% de intensidade   
//  O valor padrão é luz = 0
int luz;
//  *
//  para garantir a exclusão mútua
pthread_mutex_t m_luz = PTHREAD_MUTEX_INITIALIZER;
//  *
//  Para armazenar a leitura do sensor de presença.
//  sensor_presenca = 0     ==  não há ninguém  (FALSE)
//  sensor_presenca = 1     ==  há alguém       (TRUE)
//  O valor padrão é sensor_presença = FALSE
int sensor_presenca;
//  *
//  para garantir a exclusão mútua
pthread_mutex_t m_spresenca = PTHREAD_MUTEX_INITIALIZER;
//  *
//  *
//  *
//                  Controle de Biometria
//  *
//  A biometria será simulada como uma sequência numérica, para facilitar a
//  implementação. 
//  biometria           ==  valor salvo de biometria
//  sensor_biometria    ==  leitura do sensor de biometria
//  m_bio               ==  mutex biometria
//  m_sbio              ==  mutex sensor_biometria
//  *
//  Biometria é um unsigned int com um valor salvo para abrir a porta.
//  Valor padrão é 1234
unsigned int biometria;
//  *
//  para garantir a exclusão mútua
pthread_mutex_t m_bio = PTHREAD_MUTEX_INITIALIZER;
//  * 
//  Para armazenar a leitura do sensor de biometria.
//  sensor_biometria = 0    ==  não há leitura
//  sensor_biometria != 0   ==  o valor lido
unsigned int sensor_biometria;
//  *
//  para garantir a exclusão mútua
pthread_mutex_t m_sbio = PTHREAD_MUTEX_INITIALIZER;
//  *
//  *
//  *
//                  Controle da Porta
//  *
//  porta       ==  controla o estado da porta (aberta ou fechada)
//  m_porta     ==  mutex associado
//  *
//  A porta pode estar aberta ou fechada.
//  porta = 0       ==  porta fechada
//  porta = 1       ==  porta aberta
int porta;
//  *
//  para garantir a exclusão mútua
pthread_mutex_t m_porta = PTHREAD_MUTEX_INITIALIZER;
//  *
//  *
//  *
//                  Controle da Janela
//  *
//  janela       ==  controla o estado da janela (aberta ou fechada)
//  m_janela     ==  mutex associado
//  *
//  A janela pode estar aberta ou fechada.
//  janela = 0       ==  janela fechada
//  janela = 1       ==  janela aberta
int janela;
//  *
//  para garantir a exclusão mútua
pthread_mutex_t m_janela = PTHREAD_MUTEX_INITIALIZER;
//  *
//  *
//  *
//                  Controle da Cortina (persiana)
//  *
//  cortina       ==  controla o estado da cortina (aberta, fechada, ou entreaberta)
//  m_cortina     ==  mutex associado
//  *
//  A cortina pode estar aberta, fechada ou entreaberta.
//  cortina = 0       ==  cortina fechada
//  cortina = 1       ==  cortina aberta
//  cortina = 2       ==  cortina entreaberta
int cortina;
//  *
//  para garantir a exclusão mútua
pthread_mutex_t m_cortina = PTHREAD_MUTEX_INITIALIZER;
//  *
// ########################## ########## #####################################
//  *
//  Comando recebido do monitor
//  *
PROTOCOLO comando;
pthread_mutex_t m_comm = PTHREAD_MUTEX_INITIALIZER;

//  *
//  Se o comando executou corretamente ou não -- Retornar para monitor
//  *
char retorno[30] = "\0";
pthread_mutex_t m_ret = PTHREAD_MUTEX_INITIALIZER;

//  *
//  Espera ter comando para poder executar
//  *
int flag_executar_comando = FALSE;
pthread_mutex_t exec      = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond_exec = PTHREAD_COND_INITIALIZER;

//  *
//  Espera precisar escrever para acordar escrita
//  *
int flag_escrita_rede = FALSE;
pthread_mutex_t c = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond_esc = PTHREAD_COND_INITIALIZER;


//#################################### REDE ###################################
int sockfd;

//  *
//  Threads responsáveis pela leitura e escrita na rede.
//  A leitura roda indefinidamente, a escrita, por ser bloqueante, 
//  só acorda quando é necessário (via variáveis de condição).
//  *

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

//######################## Comando Usuário ###############################
//  *
//  Thread que fica dormente até que seja recebido um comando válido.
//  Quando isso acontece, ela acorda, tenta a execução do comando e 
//  dorme novamente (para não ocorrer espera ocupada). Se ela não con-
//  seguir executar o comando, retorna FALSE, para poder enviar o 
//  retorno ao servidor.
//  *

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

//################################# TIMER ################################
//  *
//    Timer responsável pelas tarefas periódicas. 
//  *

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
//  *
//   As tarefas periódicas necessárias para a leitura dos sensores.
//   Se a leitura feita diferir dos valores setados pelo usuário, será feito o ajuste 
//   necessário aqui mesmo. 
//  *

//  Se há a biometria correta, abre a porta
static void *ler_biometria (void *arg){

    struct periodic_info info;

    //periodo = 1s
    make_periodic (1000000, &info);
    while (TRUE){
        wait_period (&info);
        ler_sensor_biometria();
        printf("Leitura Sensor Biometria: %d \n", sensor_biometria);
        
        // Se é a biometria do usuário
        if (sensor_biometria == biometria && porta == 0){
            
           abrir_porta();
           printf("\n\t AUTO: Porta Aberta \n\n");
           sleep(3);
           fechar_porta(); 
           printf("\n\t AUTO: Porta Fechada \n\n");
           
           pthread_mutex_lock(&m_sbio);
           sensor_biometria = 0;
           pthread_mutex_unlock(&m_sbio);
           
        }
        else{
        
           pthread_mutex_lock(&m_sbio);
           sensor_biometria = 0;
           pthread_mutex_unlock(&m_sbio);
        }
    }
    return NULL;
}

//  Se o sensor de presença captar alguem, acende a luz
static void *ler_presenca (void *arg){
   
    struct periodic_info info;

   //período = 1s
    make_periodic (1000000, &info);
    while (TRUE){
    
        wait_period (&info);
        ler_sensor_presenca();
        printf("Leitura Sensor Presença: %d\n", sensor_presenca);
        
        if (sensor_presenca == TRUE && luz == -1){
            ligar_luz(PROG);
            printf("\n\t AUTO: Luz Ligada \n\n");
        } 
        else if (sensor_presenca == FALSE && luz == -2){
            desligar_luz(PROG);
            printf("\n\t AUTO: Luz Desligada \n\n");
        }
       
    }
    return NULL;
}

//  Mantém a temperatura
static void *ler_temperatura (void *arg){

    struct periodic_info info;

   //período = 5s
    make_periodic (5000000, &info);
    while (TRUE){
    
        wait_period (&info);
        ler_sensor_temperatura();
        printf("Leitura Sensor Temperatura: %d \n", sensor_temperatura);
       
    }
    return NULL;
}

