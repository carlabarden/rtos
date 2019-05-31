#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

// indexador do vetor de nodos conhecidos
long id[5] = {-1, -1, -1, -1, -1};

// para cada nodo: socket de conexão (obtido através de um accept), porta desse nodo, ip desse nodo
struct nodo {
    int newsockfd;
    int porta;
    char ip[12];
};

struct nodo nodo[5];

void *cliente(void *arg) {
    long cid = (long)arg;
    int i, n;
    char buffer[256];
    while (1) {
        bzero(buffer,sizeof(buffer));
        n = read(nodo[cid].newsockfd,buffer,50);
        //printf("Recebeu: %s - %lu\n", buffer,strlen(buffer));
        printf("Cliente %ld: %s\n", cid, buffer);
        if (n < 0) {
            printf("Erro lendo do socket!\n");
            exit(1);
        }
        //se forem iguais, strcmp retorna 0
        if (strcmp(buffer, "sair\n") == 0){
            printf("\n\n\t Cliente %ld saiu.\n\n",cid);
            pthread_mutex_lock(&m);
            id[cid] = -1;
            pthread_mutex_unlock(&m);
            pthread_exit(NULL);
        }
        
        // MUTEX LOCK - GERAL
        pthread_mutex_lock(&m);
        for (i = 0;i < 5 ; i++) {
            if (i != cid && id[i] >= 0) {
                n = write(nodo[i].newsockfd,buffer,50);
                if (n < 0) {
                    printf("Erro escrevendo no socket!\n");
                    exit(1);
                }
            }
        }
        pthread_mutex_unlock(&m);
        // MUTEX UNLOCK - GERAL
    }
}

int main(int argc, char *argv[]) {
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    int sockfd, portno;
    //     char buffer[256];
    //     int n;
    pthread_t t;
    if (argc < 2) {
        printf("Erro, porta nao definida!\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("Erro abrindo o socket!\n");
        exit(1);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        printf("Erro fazendo bind!\n");
        exit(1);
    }
    listen(sockfd,5);
    while (1) {
        long i;
        for ( i = 0; i < 5; i++){
            if (id[i] == -1){
               nodo[i].newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
               // MUTEX LOCK - GERAL
               pthread_mutex_lock(&m);
                if (nodo[i].newsockfd < 0) {
                    printf("Erro no accept!\n");
                    exit(1);
                }
                pthread_create(&t, NULL, cliente, (void *)i);
                id[i] = i;
                pthread_mutex_unlock(&m);
                // MUTEX UNLOCK - GERAL
            }
        }
    }
    //    close(newsockfd);
    //    close(sockfd);
    return 0; 
}
