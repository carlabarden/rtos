#include "simulador.h"

void inicia_sistemas(){

    //  temperatura    
    pthread_mutex_lock(&m_temp);
    temperatura = 0;
    pthread_mutex_unlock(&m_temp); 
    
    //  sensor temperatura
    pthread_mutex_lock(&m_stemp);
    sensor_temperatura = gera_aleatorio (-3, 30);
    pthread_mutex_unlock(&m_stemp);
    
    //  luz
    pthread_mutex_lock(&m_luz);
    luz = -1;
    pthread_mutex_unlock(&m_luz);
    
    //  sensor presença
    pthread_mutex_lock(&m_spresenca);
    sensor_presenca = FALSE;
    pthread_mutex_unlock(&m_spresenca); 
    
    //  biometria
    pthread_mutex_lock(&m_bio);
    biometria = 1234;
    pthread_mutex_unlock(&m_bio); 
    
    //  sensor biometria
    pthread_mutex_lock(&m_sbio);
    sensor_biometria = FALSE;
    pthread_mutex_unlock(&m_sbio); 
    
    //  porta
    pthread_mutex_lock(&m_porta);
    porta = 0;
    pthread_mutex_unlock(&m_porta); 
    
    //  janela
    pthread_mutex_lock(&m_janela);
    janela = 0;
    pthread_mutex_unlock(&m_janela); 
    
    //  cortina
    pthread_mutex_lock(&m_cortina);
    cortina = 0;
    pthread_mutex_unlock(&m_cortina); 
}

void cadastrar_biometria(unsigned int n_bio){

    pthread_mutex_lock(&m_bio);
    biometria = n_bio;
    pthread_mutex_unlock(&m_bio);
    
    pthread_mutex_lock(&m_ret);
    strcpy(retorno, "\n\tBiometria cadastrada!\n\n\0");
    pthread_mutex_unlock(&m_ret);  
}

void comando_invalido(){
    
    pthread_mutex_lock(&m_ret);
    strcpy(retorno, "\n\tComando Inválido!!!\n\n\0");
    pthread_mutex_unlock(&m_ret);
}


void ligar_ar(int temp){

    pthread_mutex_lock(&m_temp);
    temperatura = temp;
    pthread_mutex_unlock(&m_temp);
    
    pthread_mutex_lock(&m_ret);    
    strcpy(retorno, "\n\tAr ligado!\n\n\0");
    pthread_mutex_unlock(&m_ret);
}


void desligar_ar(){

    pthread_mutex_lock(&m_temp);
    temperatura = 0;
    pthread_mutex_unlock(&m_temp);
    
    pthread_mutex_lock(&m_ret);
    strcpy(retorno, "\n\tAr desligado!\n\n\0");
    pthread_mutex_unlock(&m_ret);
}

void ajustar_temperatura(int temp){

    pthread_mutex_lock(&m_temp);
    temperatura = temp;
    if (temperatura < 17) temperatura = 17;
    if (temperatura > 24) temperatura = 24;
    pthread_mutex_unlock(&m_temp);

    pthread_mutex_lock(&m_ret);
    strcpy(retorno, "\n\tTemperatura ajustada!\n\n\0");  
    pthread_mutex_unlock(&m_ret);  
}

void retornar_temp(){

    int aux = temperatura;
    char b[20] = "\0";
    
    if (aux  == 0) sprintf(b, "\n\tAr desligado!\n\n");
    else sprintf(b, "\n\tTemperatura: %d\n\n", aux);
    
    pthread_mutex_lock(&m_ret);
    strcpy(retorno, b);
    pthread_mutex_unlock(&m_ret);

}

void retornar_temp_sensor(){
        
    int aux = sensor_temperatura;
    char b[20] = "\0";
    sprintf(b, "\n\tSensor: %d\n\n", aux);
    
    pthread_mutex_lock(&m_ret);
    strcpy(retorno, b);
    pthread_mutex_unlock(&m_ret);
}

void ligar_luz(int quem){

    pthread_mutex_lock(&m_luz);
    luz = 2 * quem;
    pthread_mutex_unlock(&m_luz);
    
    pthread_mutex_lock(&m_ret);
    strcpy(retorno, "\n\tLuz ligada!\n\n\0");
    pthread_mutex_unlock(&m_ret);

}

void desligar_luz(int quem){

    pthread_mutex_lock(&m_luz);
    luz = 1 * quem;
    pthread_mutex_unlock(&m_luz);
    
    pthread_mutex_lock(&m_ret);
    strcpy(retorno, "\n\tLuz desligada!\n\n\0");
    pthread_mutex_unlock(&m_ret);
}

void meia_luz(){

    pthread_mutex_lock(&m_luz);
    luz = 3;
    pthread_mutex_unlock(&m_luz);
    
    pthread_mutex_lock(&m_ret);
    strcpy(retorno, "\n\tMeia luz!\n\n\0");
    pthread_mutex_unlock(&m_ret);
}

void retornar_luz(){
    
    int aux = abs(luz);
    
    if (aux == 1){
        pthread_mutex_lock(&m_ret);
        strcpy(retorno, "\n\tLuz desligada!\n\n\0");
        pthread_mutex_unlock(&m_ret);
    }
    else if (aux == 2){
        pthread_mutex_lock(&m_ret);
        strcpy(retorno, "\n\tLuz ligada!\n\n\0");
        pthread_mutex_unlock(&m_ret);
    }
    else if (aux == 3){
        pthread_mutex_lock(&m_ret);
        strcpy(retorno, "\n\tMeia luz!\n\n\0");
        pthread_mutex_unlock(&m_ret);
    }
}

void abrir_porta(){

    pthread_mutex_lock(&m_porta);
    porta = 1;
    pthread_mutex_unlock(&m_porta);
    
    pthread_mutex_lock(&m_ret);
    strcpy(retorno, "\n\tPorta aberta!\n\n\0");
    pthread_mutex_unlock(&m_ret);
}

void fechar_porta(){

    pthread_mutex_lock(&m_porta);
    porta = 0;
    pthread_mutex_unlock(&m_porta);
    
    pthread_mutex_lock(&m_ret);
    strcpy(retorno, "\n\tPorta fechada!\n\n\0");
    pthread_mutex_unlock(&m_ret);
}

void retornar_porta(){
    
    int aux = porta;
    
    if (aux == 0){
        pthread_mutex_lock(&m_ret);
        strcpy(retorno, "\n\tPorta fechada!\n\n\0");
        pthread_mutex_unlock(&m_ret);
    }
    else{
        pthread_mutex_lock(&m_ret);
        strcpy(retorno, "\n\tPorta aberta!\n\n\0");
        pthread_mutex_unlock(&m_ret);
    }
}

void abrir_janela(){

    pthread_mutex_lock(&m_janela);
    janela = 1;
    pthread_mutex_unlock(&m_janela);
    
    pthread_mutex_lock(&m_ret);
    strcpy(retorno, "\n\tJanela aberta!\n\n\0");
    pthread_mutex_unlock(&m_ret);
}

void fechar_janela(){

    pthread_mutex_lock(&m_janela);
    janela = 0;
    pthread_mutex_unlock(&m_janela);
    
    pthread_mutex_lock(&m_ret);
    strcpy(retorno, "\n\tJanela fechada!\n\n\0");
    pthread_mutex_unlock(&m_ret);
}

void retornar_janela(){
    
    int aux = janela;
    
    if (aux == 0){
        pthread_mutex_lock(&m_ret);
        strcpy(retorno, "\n\tJanela fechada!\n\n\0");
        pthread_mutex_unlock(&m_ret);
    }
    else {
        pthread_mutex_lock(&m_ret);
        strcpy(retorno, "\n\tJanela aberta!\n\n\0");
        pthread_mutex_unlock(&m_ret);
    }

}

void abrir_cortina(){

    pthread_mutex_lock(&m_cortina);
    cortina = 1;
    pthread_mutex_unlock(&m_cortina);
    
    pthread_mutex_lock(&m_ret);
    strcpy(retorno, "\n\tCortina aberta!\n\n\0");
    pthread_mutex_unlock(&m_ret);
}

void fechar_cortina(){

    pthread_mutex_lock(&m_cortina);
    cortina = 0;
    pthread_mutex_unlock(&m_cortina);
    
    pthread_mutex_lock(&m_ret);
    strcpy(retorno, "\n\tCortina fechada!\n\0");
    pthread_mutex_unlock(&m_ret);
}

void cortina_entreaberta(){

    pthread_mutex_lock(&m_cortina);
    cortina = 2;
    pthread_mutex_unlock(&m_cortina);
    
    pthread_mutex_lock(&m_ret);
    strcpy(retorno, "\n\tCortina entreaberta!\n\n\0");
    pthread_mutex_unlock(&m_ret);
}

void retornar_cortina(){
    
    int aux = cortina;
    
    if (aux == 0){
        pthread_mutex_lock(&m_ret);
        strcpy(retorno, "\n\tCortina fechada!\n\n\0");
        pthread_mutex_unlock(&m_ret);
    }
    else if (aux == 1){
        pthread_mutex_lock(&m_ret);
        strcpy(retorno, "\n\tCortina aberta!\n\n\0");
        pthread_mutex_unlock(&m_ret);
    }
    else {
        pthread_mutex_lock(&m_ret);
        strcpy(retorno, "\n\tCortina entreaberta!\n\n\0");
        pthread_mutex_unlock(&m_ret);
    }
}


void ler_sensor_temperatura(){

    //se o ar não foi ligado
    if (temperatura == 0){
                    
        //gera uma variação aleatória para a leitura do sensor
        pthread_mutex_lock(&m_stemp);
        sensor_temperatura += gera_aleatorio(-2,1);
        pthread_mutex_unlock(&m_stemp);
      
    } //se o ar foi ligado
    else {
             
        if (temperatura > sensor_temperatura){
           
            pthread_mutex_lock(&m_stemp);
            sensor_temperatura += 1;
            pthread_mutex_unlock(&m_stemp);     
        }
        else if (temperatura < sensor_temperatura){
           
            pthread_mutex_lock(&m_stemp);
            sensor_temperatura -= 1;
            pthread_mutex_unlock(&m_stemp);     
        }
                       
    }
}


//acesso não autorizado - tendência
int cont_b = 0;
void ler_sensor_biometria(){

    int tem_gente = gera_aleatorio(-1, 1);
    
    if (cont_b == 3){
    
        cont_b = 0;
        if (tem_gente){
        
            unsigned int aux = biometria;
        
            pthread_mutex_lock(&m_sbio);
            sensor_biometria = gera_aleatorio((int)aux-2, (int)aux+1);
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
    PROTOCOLO cm = comando;
    
    //validação de comandos
    //comando
    if(strcmp(cm.comando, "abrir") == 0){
        
        if      (strcmp(cm.alvo, "porta") == 0)   abrir_porta();
        else if (strcmp(cm.alvo, "janela") == 0)  abrir_janela();
        else if (strcmp(cm.alvo, "cortina") == 0) abrir_cortina();
        else    comando_invalido();
        
    }
    else if(strcmp(cm.comando, "fechar") == 0){
    
        if      (strcmp(cm.alvo, "porta") == 0)   fechar_porta();
        else if (strcmp(cm.alvo, "janela") == 0)  fechar_janela();
        else if (strcmp(cm.alvo, "cortina") == 0) fechar_cortina();
        else    comando_invalido();    
    
    }
    else if(strcmp(cm.comando, "ligar") == 0){
        
        if      (strcmp(cm.alvo, "ar") == 0)     ligar_ar(20);
        else if (strcmp(cm.alvo, "luz") == 0)    ligar_luz(USER);
        else    comando_invalido();
    
    }
    else if(strcmp(cm.comando, "desligar") == 0){
        
        if      (strcmp(cm.alvo, "ar") == 0)     desligar_ar(20);
        else if (strcmp(cm.alvo, "luz") == 0)    desligar_luz(USER);
        else    comando_invalido();
    }
    else if(strcmp(cm.comando, "ajustar") == 0){
        
        if (strcmp(cm.alvo, "luz") == 0){
            
            if (strcmp(cm.opt, "mea") == 0)   meia_luz();
            else comando_invalido();
        }
        else if (strcmp(cm.alvo, "cortina") == 0){
            
            if (strcmp(cm.opt, "mea") == 0)   cortina_entreaberta();
            else comando_invalido();
        }
        else if (strcmp(cm.alvo, "temp") == 0){
        
            int valor = atoi(cm.valor);
        
            if (strcmp(cm.opt, "set") == 0){
                
                if ((valor < 17 ||  valor > 24) || (temperatura == 0)) comando_invalido();
                else ajustar_temperatura(valor);  
            }
            else if (strcmp(cm.opt, "add") == 0){
                
                int aux = temperatura;
                int temp = aux + valor;
                ajustar_temperatura(temp);
            }
            else if (strcmp(cm.opt, "dim") == 0){
                
                int aux = temperatura;
                int temp = aux - valor;
                ajustar_temperatura(temp);
            }
            else comando_invalido();
        }
        else comando_invalido();
        
    }
    else if(strcmp(cm.comando, "ver") == 0){
    
        if      (strcmp(cm.alvo, "porta") == 0)     retornar_porta();
        else if (strcmp(cm.alvo, "janela") == 0)    retornar_janela();
        else if (strcmp(cm.alvo, "cortina") == 0)   retornar_cortina();
        else if (strcmp(cm.alvo, "luz") == 0)       retornar_luz();
        else if (strcmp(cm.alvo, "temp") == 0)      retornar_temp();
        else if (strcmp(cm.alvo, "term") == 0)      retornar_temp_sensor();
        else comando_invalido();
    
    }
    else if(strcmp(cm.comando, "cadbio") == 0){
    
        unsigned int cad = atoi(cm.alvo);
        cadastrar_biometria(cad);
    }
    else if(strcmp(cm.comando, "reset") == 0){
    
        inicia_sistemas();   
        pthread_mutex_lock(&m_ret);
        strcpy(retorno, "\n\tReiniciar!\n\n\0");
        pthread_mutex_unlock(&m_ret);
         
    }
    else if(strcmp(cm.comando, "auto") == 0){
    
        if (strcmp(cm.alvo, "luz") == 0){
            
            pthread_mutex_lock(&m_luz);
            luz = luz * PROG;
            pthread_mutex_unlock(&m_luz);
            
            pthread_mutex_lock(&m_ret);
            strcpy(retorno, "\n\tLuz automática!\n\n\0");
            pthread_mutex_unlock(&m_ret);
        }
        else comando_invalido();
    }
    else if(strcmp(cm.comando, "sair") == 0){
        //sair na função de leitura da rede, pra fechar a conexão
    }
    else{
        comando_invalido();
    }
}

