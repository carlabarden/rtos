#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
//#include <math.h>
#include "../shr/protocolo.h"

#define TRUE  1
#define FALSE 0

#define USER  1
#define PROG -1


// ########################## SIMULADOR #####################################
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
extern int temperatura;
//  *
//  para garantir a exclusão mútua
extern pthread_mutex_t m_temp;
//  *
//  Para armazenar a leitura do sensor de temperatura
//  Valor padrão é sensor_temperatura = aleatorio entre -5 e 45;
extern int sensor_temperatura;
//  *
//  para garantir a exclusão mútua
extern pthread_mutex_t m_stemp;
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
extern int luz;
//  *
//  para garantir a exclusão mútua
extern pthread_mutex_t m_luz;
//  *
//  Para armazenar a leitura do sensor de presença.
//  sensor_presenca = 0     ==  não há ninguém  (FALSE)
//  sensor_presenca = 1     ==  há alguém       (TRUE)
//  O valor padrão é sensor_presença = FALSE
extern int sensor_presenca;
//  *
//  para garantir a exclusão mútua
extern pthread_mutex_t m_spresenca;
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
extern unsigned int biometria;
//  *
//  para garantir a exclusão mútua
extern pthread_mutex_t m_bio;
//  * 
//  Para armazenar a leitura do sensor de biometria.
//  sensor_biometria = 0    ==  não há leitura
//  sensor_biometria != 0   ==  o valor lido
extern unsigned int sensor_biometria;
//  *
//  para garantir a exclusão mútua
extern pthread_mutex_t m_sbio;
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
extern int porta;
//  *
//  para garantir a exclusão mútua
extern pthread_mutex_t m_porta;
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
extern int janela;
//  *
//  para garantir a exclusão mútua
extern pthread_mutex_t m_janela;
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
extern int cortina;
//  *
//  para garantir a exclusão mútua
extern pthread_mutex_t m_cortina;
//  *
// ########################## ########## #####################################
//  *
//  Comando recebido do monitor
//  *
extern PROTOCOLO comando;
extern pthread_mutex_t m_comm;
//  *
//  Se o comando executou corretamente ou não -- Retornar para monitor
//  *
extern char retorno[30];
extern pthread_mutex_t m_ret;
//  *
 
//  seta todos os sistemas para os valores padrão
void inicia_sistemas();

//  cadastra biometria
void cadastrar_biometria(unsigned int n_bio);


//  FUNÇÕES RETORNAR_...    ==  escrevem na variável retorno[]
//  para retornar comando inválido
void comando_invalido();

//  ar condicionado - temperatura
void ligar_ar(int temp);
void desligar_ar();
void ajustar_temperatura(int temp);
void retornar_temp();
void retornar_temp_sensor();

//  luz
//  int quem  ==  para saber se foi o user ou o prog
//      user  ==  1
//      prog  ==  0
void ligar_luz(int quem);
void desligar_luz(int quem);
void meia_luz();
void retornar_luz();

//  porta
void abrir_porta();
void fechar_porta();
void retornar_porta();

//  janela
void abrir_janela();
void fechar_janela();
void retornar_janela();

//  cortinas
void abrir_cortina();
void fechar_cortina();
void cortina_entreaberta();
void retornar_cortina();

//  executa o comando do usuário
void executar_comando();

//  gerar números aleatórios entre a e b
//  a < b
//  primeiro parametro é dado por x - 1
int gera_aleatorio(int a, int b);

//  simula uma perturbação na temperatura, para a tarefa periódica ajustar
void ler_sensor_temperatura();

//  simula uma leitura no sensor de biometria
void ler_sensor_biometria();

//  simula uma leitura no sensor de presença
void ler_sensor_presenca();


