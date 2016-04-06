#include "SanUSB1X.h"

void interrupt interrupcao(){
}

long int tensao,resultado;

void main(){
    clock_int_4MHz();
    habilita_canal_AD(AN0);
    taxa_serial(19200);

    while(1){ //ANALÓGICO DIGITAL(10 bits)a
        if(!entrada_pin_e3){//pressionar o botão para gravação
            Reset();
        }
        // 5000 mV 1023
        // tensao le_AD10bits(0)
        resultado = le_AD10bits(0);
        tensao= (5000* resultado)/1023;
        sendsw((char *)"Tensao: "); // Imprime pela serial
        sendnum(tensao);
        sendsw((char *)"mv.\r\n"); // Imprime pela serial

        //Estou vivo!
        nivel_alto(pin_b7);
        tempo_ms(500);
        nivel_baixo(pin_b7);
        tempo_ms(500);
    }
}