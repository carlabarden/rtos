#include "simulador.h"

void inicia_sistemas(){

    //  temperatura    
    pthread_mutex_lock(&m_temp);
    temperatura = -1;
    pthread_mutex_lock(&m_temp); 
    
    //  sensor temperatura
    pthread_mutex_lock(&m_stemp);
    sensor_temperatura = gera_aleatorio (-6, 45);
    pthread_mutex_lock(&m_temp);
    
    //  luz
    pthread_mutex_lock(&m_luz);
    luz = 0;
    pthread_mutex_lock(&m_luz);
    
    //  sensor presença
    pthread_mutex_lock(&m_spresenca);
    sensor_presenca = FALSE;
    pthread_mutex_lock(&m_spresenca); 
    
    //  biometria
    pthread_mutex_lock(&m_bio);
    biometria = 12345;
    pthread_mutex_lock(&m_bio); 
    
    //  sensor biometria
    pthread_mutex_lock(&m_sbio);
    sensor_biometria = FALSE;
    pthread_mutex_lock(&m_sbio); 
    
    //  porta
    pthread_mutex_lock(&m_porta);
    porta = FECHADA;
    pthread_mutex_lock(&m_porta); 
    
    //  janela
    pthread_mutex_lock(&m_janela);
    janela = FECHADA;
    pthread_mutex_lock(&m_janela); 
    
    //  cortina
    pthread_mutex_lock(&m_cortina);
    cortina = FECHADA;
    pthread_mutex_lock(&m_cortina); 
}

void cadastrar_biometria(unsigned int n_bio){

    pthread_mutex_lock(&m_bio);
    biometria = n_bio;
    strcpy(retorno, "\n\tBiometria cadastrada!\n\0");
    pthread_mutex_unlock(&m_bio);
}


void ligar_ar(int temp){

    pthread_mutex_lock(&m_temp);
    temperatura = temp;
    strcpy(retorno, "\n\tAr ligado!\n\0");
    pthread_mutex_unlock(&m_temp);
}


void desligar_ar(){

    pthread_mutex_lock(&m_temp);
    temperatura = 0;
    strcpy(retorno, "\n\tAr desligado!\n\0");
    pthread_mutex_unlock(&m_temp);
}

void ajustar_temperatura(int temp){

    pthread_mutex_lock(&m_temp);
    temperatura = temp;
    if (temperatura < 17) temperatura = 17;
    if (temperatura > 24) temperatura = 24;
    strcpy(retorno, "\n\tTemperatura ajustada!\n\0");
    pthread_mutex_unlock(&m_temp);
}

void ligar_luz(){

    pthread_mutex_lock(&m_luz);
    luz = 2;
    strcpy(retorno, "\n\tLuz ligada!\n\0");
    pthread_mutex_unlock(&m_luz);
}

void desligar_luz(){

    pthread_mutex_lock(&m_luz);
    luz = 1;
    strcpy(retorno, "\n\tLuz desligada!\n\0");
    pthread_mutex_unlock(&m_luz);
}

void meia_luz(){

    pthread_mutex_lock(&m_luz);
    luz = 3;
    strcpy(retorno, "\n\tMeia luz!\n\0");
    pthread_mutex_unlock(&m_luz);
}

void abrir_porta(){

    pthread_mutex_lock(&m_porta);
    porta = ABERTA;
    strcpy(retorno, "\n\tPorta aberta!\n\0");
    pthread_mutex_unlock(&m_porta);
}

void fechar_porta(){

    pthread_mutex_lock(&m_porta);
    porta = 0;
    strcpy(retorno, "\n\tPorta fechada!\n\0");
    pthread_mutex_unlock(&m_porta);
}

void abrir_janela(){

    pthread_mutex_lock(&m_janela);
    janela = 1;
    strcpy(retorno, "\n\tJanela aberta!\n\0");
    pthread_mutex_unlock(&m_janela);
}

void fechar_janela(){

    pthread_mutex_lock(&m_janela);
    janela = 0;
    strcpy(retorno, "\n\tJanela fechada!\n\0");
    pthread_mutex_unlock(&m_janela);
}

void abrir_cortina(){

    pthread_mutex_lock(&m_cortina);
    cortina = 1;
    strcpy(retorno, "\n\tCortina aberta!\n\0");
    pthread_mutex_unlock(&m_cortina);
}

void fechar_cortina(){

    pthread_mutex_lock(&m_cortina);
    cortina = 0;
    strcpy(retorno, "\n\tCortina fechada!\n\0");
    pthread_mutex_unlock(&m_cortina);
}

void cortina_entreabertas(){

    pthread_mutex_lock(&m_cortina);
    cortina = 2;
    strcpy(retorno, "\n\tCortina entreaberta!\n\0");
    pthread_mutex_unlock(&m_cortina);
}


//ler o mesmo número 3x
int cont_t = 0;
void ler_sensor_temperatura(){
    
    if (cont_t == 3){
    
        cont_t = 0;
        //variação a ser somada na temperatura, entre 0 e 5
        int var = gera_aleatorio(0, 5);

        pthread_mutex_lock(&m_stemp);
        sensor_temperatura = temperatura + var;
        temperatura = sensor_temperatura;
        pthread_mutex_unlock(&m_stemp);
    }
    else{
 
        cont_t += 1;
        pthread_mutex_lock(&m_stemp);
        sensor_temperatura = temperatura;
        pthread_mutex_unlock(&m_stemp);
    }  
}


//acesso não autorizado - tendência
int cont_b = 0;
void ler_sensor_biometria(){

    int tem_gente = gera_aleatorio(-1, 1);
    
    if (cont_b == 3){
    
        cont_b = 0;
        if (tem_gente){
        
            pthread_mutex_lock(&m_sbio);
            sensor_biometria = gera_aleatorio(biometria-2, biometria+1);
            pthread_mutex_unlock(&m_sbio);
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
        
            pthread_mutex_lock(&m_spresenca);
            sensor_presenca = 1;
            pthread_mutex_unlock(&m_spresenca);
        }
        else {
            pthread_mutex_lock(&m_spresenca);
            sensor_presenca = 0;
            pthread_mutex_unlock(&m_spresenca);
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


void executar_comando(){
    
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
    
        if (strcmp(buff.alvo, "abrir") == 0) abrir_cortina();
        else if (strcmp(buff.alvo, "fechar") == 0) fechar_cortina();
        else if (strcmp(buff.alvo, "meia") ==  0) cortina_entreabertas();
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
        
            int aux = cortina;
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
            sprintf(b, "\n\tTemperatura: %d\n", aux);
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
        
    }
    else{
        strcpy(retorno, "\n\tComando Inválido!!!\n\0");
    }
}

