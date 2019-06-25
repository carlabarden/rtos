#include "simulador.h"

int inicia_sistemas(){
    
    pthread_mutex_lock(&comm);
    temperatura = 20;
    luz = 0;
    porta = 0;
    biometria = 13;
    janela = 0;
    cortinas = 0;
    pthread_mutex_unlock(&comm);
    
    return TRUE;
}

int cadastrar_biometria(unsigned int n_bio){
    pthread_mutex_lock(&comm);
    biometria = n_bio;
    strcpy(retorno, "\n\tBiometria cadastrada!\n\0");
    pthread_mutex_unlock(&comm);
    return TRUE;
}

void ligar_ar(){
    pthread_mutex_lock(&comm);
    temperatura = 20;
    strcpy(retorno, "\n\tAr ligado!\n\0");
    pthread_mutex_unlock(&comm);
}

void desligar_ar(){
    pthread_mutex_lock(&comm);
    temperatura = 0;
    strcpy(retorno, "\n\tAr desligado!\n\0");
    pthread_mutex_unlock(&comm);
}

void ajustar_temperatura(int temp){
    pthread_mutex_lock(&comm);
    temperatura = temp;
    if (temperatura < 17) temperatura = 17;
    if (temperatura > 24) temperatura = 24;
    strcpy(retorno, "\n\tTemperatura ajustada!\n\0");
    pthread_mutex_unlock(&comm);
}

void ligar_luz(){
    pthread_mutex_lock(&comm);
    luz = 1;
    strcpy(retorno, "\n\tLuz ligada!\n\0");
    pthread_mutex_unlock(&comm);
}

void desligar_luz(){
    pthread_mutex_lock(&comm);
    luz = 0;
    strcpy(retorno, "\n\tLuz desligada!\n\0");
    pthread_mutex_unlock(&comm);
}

void meia_luz(){
    pthread_mutex_lock(&comm);
    luz = 2;
    strcpy(retorno, "\n\tMeia luz!\n\0");
    pthread_mutex_unlock(&comm);
}

void abrir_porta(){
    pthread_mutex_lock(&comm);
    porta = 1;
    strcpy(retorno, "\n\tPorta aberta!\n\0");
    pthread_mutex_unlock(&comm);
}

void fechar_porta(){
    pthread_mutex_lock(&comm);
    porta = 0;
    strcpy(retorno, "\n\tPorta fechada!\n\0");
    pthread_mutex_unlock(&comm);
}

void abrir_janela(){
    pthread_mutex_lock(&comm);
    janela = 1;
    strcpy(retorno, "\n\tJanela aberta!\n\0");
    pthread_mutex_unlock(&comm);
}

void fechar_janela(){
    pthread_mutex_lock(&comm);
    janela = 0;
    strcpy(retorno, "\n\tJanela fechada!\n\0");
    pthread_mutex_unlock(&comm);
}

void abrir_cortinas(){
    pthread_mutex_lock(&comm);
    cortinas = 1;
    strcpy(retorno, "\n\tCortina aberta!\n\0");
    pthread_mutex_unlock(&comm);
}

void fechar_cortinas(){
    pthread_mutex_lock(&comm);
    cortinas = 0;
    strcpy(retorno, "\n\tCortina fechada!\n\0");
    pthread_mutex_unlock(&comm);
}

void cortinas_entreabertas(){
    pthread_mutex_lock(&comm);
    cortinas = 2;
    strcpy(retorno, "\n\tCortina entreaberta!\n\0");
    pthread_mutex_unlock(&comm);
}


//ler o mesmo número 5x
int cont_t = 0;
void ler_sensor_temperatura(){
    
    if (cont_t == 5){
    
        cont_t = 0;
        //variação a ser somada na temperatura, entre 0 e 5
        int var = gera_aleatorio(0, 5);

        pthread_mutex_lock(&prog);
        sensor_temperatura = temperatura + var;
        temperatura = sensor_temperatura;
        pthread_mutex_unlock(&prog);
    }
    else{
 
        cont_t += 1;
        pthread_mutex_lock(&prog);
        sensor_temperatura = temperatura;
        pthread_mutex_unlock(&prog);
    }  
}


//acesso não autorizado - tendência
int cont_b = 0;
void ler_sensor_biometria(){

    int tem_gente = gera_aleatorio(-1, 1);
    
    if (cont_b == 3){
    
        cont_b = 0;
        if (tem_gente){
        
            pthread_mutex_lock(&prog);
            sensor_biometria = gera_aleatorio(biometria-2, biometria+1);
            pthread_mutex_unlock(&prog);
        }  
    }
    else{
    
        cont_b += 1;
    }   
}


int cont_p = 0;
void ler_sensor_presenca(){

    int tem_gente = gera_aleatorio(-1, 1);
    
    if (cont_p == 3){
    
        cont_p = 0;
        if (tem_gente){
        
            pthread_mutex_lock(&prog);
            sensor_presenca = 1;
            pthread_mutex_unlock(&prog);
        }
        else {
            pthread_mutex_lock(&prog);
            sensor_presenca = 0;
            pthread_mutex_unlock(&prog);
        }   
    }
    else{
    
        cont_p += 1;
    }
}


int gera_aleatorio (int a, int b){

    int aux = 0;
    aux = 1 + a + rand() % (b - a);

    return aux;
}


int executar_comando(){
    
    //caso o comando seja atualizado enquanto aqui está em execução
    PROTOCOLO buff = comando;
    
    //validação de comandos
    //comando
    if( strcmp(buff.comando, "porta") == 0 ){ 
    
        if (strcmp(buff.alvo, "abrir") == 0) abrir_porta();
        else if (strcmp(buff.alvo, "fechar") == 0) fechar_porta();
        else strcpy(retorno, "\n\tComando Inválido!!!\n\0");
    }
    else if( strcmp(buff.comando, "janela") == 0 ){
    
        if (strcmp(buff.alvo, "abrir") == 0) abrir_janela();
        else if (strcmp(buff.alvo, "fechar") == 0) fechar_janela(); 
        else strcpy(retorno, "\n\tComando Inválido!!!\n\0");  
    }
    else if( strcmp(buff.comando, "cortina") == 0 ){
    
        if (strcmp(buff.alvo, "abrir") == 0) abrir_cortinas();
        else if (strcmp(buff.alvo, "fechar") == 0) fechar_cortinas();
        else if (strcmp(buff.alvo, "meia") ==  0) cortinas_entreabertas();
        else strcpy(retorno, "\n\tComando Inválido!!!\n\0");
    }
    else if( strcmp(buff.comando, "luz") == 0 ){
    
        if (strcmp(buff.alvo, "ligar") == 0) ligar_luz();
        else if (strcmp(buff.alvo, "desligar") == 0) desligar_luz();
        else if (strcmp(buff.alvo, "meia") ==  0) meia_luz();
        else strcpy(retorno, "\n\tComando Inválido!!!\n\0");
    
    }
    else if( strcmp(buff.comando, "temp") == 0 ){
    
        if (strcmp(buff.alvo, "ajustar") == 0){
        
            int valor = atoi(buff.valor);
            
            if(strcmp(buff.opt, "set") == 0){
                
                if (valor < 17 && valor > 24) strcpy(retorno, "\n\tComando Inválido!!!\n\0");
                else ajustar_temperatura(valor);           
            }
            else if(strcmp(buff.opt, "add") == 0) {
                
                int aux = temperatura;
                int temp = aux + valor;
                ajustar_temperatura(temp);
            
            }
            else if(strcmp(buff.opt, "dim") == 0){
            
                int aux = temperatura;
                int temp = aux - valor;
                ajustar_temperatura(temp);
            
            }
            else strcpy(retorno, "\n\tComando Inválido!!!\n\0");
        }
        else strcpy(retorno, "\n\tComando Inválido!!!\n\0");

    }
    else if ( strcmp(buff.comando, "ver") == 0 ){
    
        if (strcmp(buff.alvo, "porta") == 0){
        
            int aux = porta;
            if (aux == 0) strcpy(retorno, "\n\tPorta fechada!\n\0");
            else if (aux == 1) strcpy(retorno, "\n\tPorta aberta!\n\0");
        }
        else if (strcmp(buff.alvo, "janela") == 0){
        
            int aux = janela;
            if (aux == 0) strcpy(retorno, "\n\tJanela fechada!\n\0");
            else if (aux == 1) strcpy(retorno, "\n\tJanela aberta!\n\0");
        }
        else if (strcmp(buff.alvo, "cortina") == 0){
        
            int aux = cortinas;
            if (aux == 0) strcpy(retorno, "\n\tCortina fechada!\n\0");
            else if (aux == 1) strcpy(retorno, "\n\tCortina aberta!\n\0");
            else if (aux == 2) strcpy(retorno, "\n\tCortina entreaberta!\n\0");
        }
        else if (strcmp(buff.alvo, "luz") == 0){
        
            int aux = luz;
            if (aux == 0) strcpy(retorno, "\n\tLuz apagada!\n\0");
            else if (aux == 1) strcpy(retorno, "\n\tLuz acesa!\n\0");
            else if (aux == 2) strcpy(retorno, "\n\tMeia luz!\n\0");
        }
        else if (strcmp(buff.alvo, "temp") == 0){
        
            int aux = temperatura;
            char b[20] = "\0";
            sprintf(b, "\n\tTemperatura: %d\n\0", aux);
            strcpy(retorno, b);
            
        }
        else strcpy(retorno, "\n\tComando Inválido!!!\n\0");
    }
    else if ( strcmp(buff.comando, "cadbio") == 0 ){
    
        int aux = atoi(buff.alvo);
        cadastrar_biometria(aux);
    }
    else if ( strcmp(buff.comando, "sair") == 0 ){
        //sair na função de leitura da rede, pra fechar a conexão
        return TRUE;
    }
    else{
        strcpy(retorno, "\n\tComando Inválido!!!\n\0");
    }
    
    return FALSE;
}



