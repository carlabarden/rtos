#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include "../shr/protocolo.h"

#define TRUE  1
#define FALSE 0

// Temperatura varia entre 17 e 24
// 0 para ar condicionado desligado
// temperatura ajustada pelo usuário
extern int temperatura ;

// Luz varia entre 0 e 2
// 0 == desligada
// 1 == ligada, intensidade máxima
// 2 == ligada, meia luz
extern int luz;

// Porta pode estar aberta ou fechada
// 0 = fechada
// 1 = aberta
extern int porta; 

// Biometria, para facilitar a implementação, será uma sequencia numérica nesta simulação
extern unsigned int biometria;

// Janela pode estar aberta ou fechada
// 0 = fechada
// 1 = aberta
extern int janela;

// Cortinas podem estar abertas, fechadas ou semi-abertas
// 0 = fechada
// 1 = aberta
// 2 = semiaberta
extern int cortinas; 

// Sensores == PARA A LEITURA
// Sensor de presença, para a iluminação
// 0 == FALSE == ninguém
// 1 == TRUE == alguém entrou
extern int sensor_presenca;

// Sensor de biometria, para abrir a porta
// 0 == FALSE == leitura não detectada
// !=0 == TRUE == leitura detectada, valor na própria variável
extern int sensor_biometria;

// Sensor da leitura da temperatura
// 0 == FALSE == ar condicionado desligado
// !=0 == TRUE == leitura do sensor
extern int sensor_temperatura;

// Comando recebido do monitor
extern PROTOCOLO comando;

// Se o comando executou corretamente ou não -- Retornar para monitor
extern char retorno[30];

//Para exclusão mútua na atualização das variáveis globais
extern pthread_mutex_t comm;

// Para exclusão mútua na atualização das variáveis globais pelo programa
extern pthread_mutex_t prog;
 
 
//seta todos os sistemas para os valores padrão
int inicia_sistemas();

//cadastra biometria
int cadastrar_biometria(unsigned int n_bio);

//ar condicionado - temperatura
void ligar_ar();
void desligar_ar();
void ajustar_temperatura(int temp);

//luz
void ligar_luz();
void desligar_luz();
void meia_luz();

//porta
void abrir_porta();
void fechar_porta();

//janela
void abrir_janela();
void fechar_janela();

//cortinas
void abrir_cortinas();
void fechar_cortinas();
void cortinas_entreabertas();

//executa o comando do usuário
int executar_comando();


//gerar números aleatórios entre a e b
// a < b
// primeiro parametro é dado por x - 1
int gera_aleatorio(int a, int b);

//simula uma perturbação na temperatura, para a tarefa periódica ajustar
void ler_sensor_temperatura();

//simula uma leitura no sensor de biometria
void ler_sensor_biometria();

//simula uma leitura no sensor de presença
void ler_sensor_presenca();




//TODO:
//executar comando é um switch case que chama a função daqui
//no protocolo, terá a validação do comando (se a palavra é um comando msm)
//o main chamará só a função executar comando, que tratará de chamar a função correta aqui
// terá que ter  mutex no comando e nas variáveis daqui
// thread com var_cond pra executar comando só quando tiver comando

//são dois subsistemas

// 1- ler comando da rede
// 2- executar comando
// 3- retornar para monitor

// 1- ler do sensor
// 2- verificar se é o que foi pedido
//      gerar variação aleatoria, aleatoriamente
//      efetuar correção
//   se não for, ajustar
//  se for, ok

// sempre setar NULL no comando para o sensor de presença funcionar

// Fazer os comandos (validação em SM
// Implementar executar_comando



