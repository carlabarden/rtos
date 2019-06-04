//####################### SERVIDOR ########################
#include "main.h"

int main(int argc, char *argv[]) {

    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    int sockfd, portno;
    pthread_t rede_out, rede_in;
    
    if (argc < 2) {
        printf("Erro, porta nao definida!\n");
        exit(1);
    }
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("Erro abrindo o socket!\n");
        close(sockfd);
        exit(1);
    }
    
    //reusar porta após timeout
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0){
        printf("setsockopt(SO_REUSEADDR) failed");
        close(sockfd);
        exit(1);
    }  
         
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        printf("Erro fazendo bind!\n");
        close(sockfd);
        exit(1);
    }
    
    listen(sockfd,1);

    ARG arg;
    arg.id = 0;
    arg.sockfd = sockfd;  
    long id = 1;  

    nodo.newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
    if (nodo.newsockfd < 0) {
        printf("Erro no accept!\n");
        close(nodo.newsockfd);
        close(sockfd);
        exit(1);
    }
    
    printf("\n\n\t Monitor ON.\n\n");
    pthread_create(&rede_out, NULL, enviar_para_simulador, (void *)&arg);
    pthread_create(&rede_in, NULL, receber_do_simulador, (void *)id);

   
   //ler comandos
    do{   
        fgets(comando, 50, stdin);
        pthread_mutex_lock(&c);
        flag = TRUE;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&c);
     
    }while(TRUE);
    
    return 0; 
}