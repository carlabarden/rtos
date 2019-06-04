//####################### CLIENTE ########################
#include "main.h"

int main(int argc, char *argv[]) {

    int portno, n;
    char buffer[50];
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
     
    pthread_join(rede_in, NULL);  
    return 0;
}
