//####################### CLIENTE ########################
#include "main.h"

int main(int argc, char *argv[]) {

    //para gerar números aleatorios
    srand((unsigned)time(NULL));

    //para tarefas periódicas
	sigset_t alarm_sig;
	int i;
	//para leitura dos 3 sensores
	pthread_t biometria_th;
	pthread_t presenca_th;
	pthread_t temperatura_th;
    
    sigemptyset (&alarm_sig);
	for (i = SIGRTMIN; i <= SIGRTMAX; i++)
		sigaddset (&alarm_sig, i);
	sigprocmask (SIG_BLOCK, &alarm_sig, NULL);
	
	pthread_create (&biometria_th,     NULL, ler_biometria,    NULL) ;
	pthread_create (&presenca_th,      NULL, ler_presenca,     NULL) ;
	pthread_create (&temperatura_th,   NULL, ler_temperatura,  NULL) ;
	
    //rede
    int portno;
    //char buffer[50];
    struct sockaddr_in serv_addr;
    pthread_t rede_in, rede_out;

    if (argc < 3) {
       fprintf(stderr,"Uso: %s nomehost porta\n", argv[0]);
       exit(0);
    }
    
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("Erro criando socket!\n");
        close(sockfd);
        return -1;
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    //serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    inet_aton(argv[1], &serv_addr.sin_addr);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        printf("Erro conectando!\n");
        close(sockfd);
        return -1;
    }
    
    
    printf("\n\n\t Simulador ON.\n\n");
    pthread_create(&rede_in, NULL, receber_do_monitor, NULL);
    pthread_create(&rede_out, NULL, enviar_para_monitor, NULL);
    
    //para executar comandos recebidos do usuario
    pthread_t user;
    pthread_create(&user, NULL, executar_comando_usuario, NULL);
    
    inicia_sistemas();

    pthread_join(user, NULL);  
       
    return 0;
}
