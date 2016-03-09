#include "SanUSB1X.h"

int botao = 0;

void interrupt interrupcao();
void testa_7seg(int time);
void count_display(int time, int forward);

void main(){

    clock_int_4MHz();
    habilita_interrupcao(ext1);//pin_b0
    TRISB = 0b00000010;

    while(1){
        botao = 0;

        if(!entrada_pin_e3){//pressionar o botão para gravação
            Reset();
        }

        inverte_saida(pin_a0);
        tempo_ms(400);

        if(botao == 1){
            testa_7seg(200);
            count_display(300, 0);
        }
    }
}

void interrupt interrupcao(){
    if(ext1_interrompeu){
        ext1_interrompeu = 0;

        botao = 1;
    }

}

void testa_7seg(int time){
    inverte_saida(pin_a1);

    nivel_baixo(pin_b7);
    nivel_alto(pin_b0);
    tempo_ms(time);

    nivel_baixo(pin_b0);
    nivel_alto(pin_b2);
    tempo_ms(time);

    nivel_baixo(pin_b2);
    nivel_alto(pin_b3);
    tempo_ms(time);

    nivel_baixo(pin_b3);
    nivel_alto(pin_b4);
    tempo_ms(time);

    nivel_baixo(pin_b4);
    nivel_alto(pin_b5);
    tempo_ms(time);

    nivel_baixo(pin_b5);
    nivel_alto(pin_b6);
    tempo_ms(time);

    nivel_baixo(pin_b6);
    nivel_alto(pin_b7);
    tempo_ms(time);

    nivel_baixo(pin_b7);

}

void count_display(int time, int forward){
    //Se o usuário quer contar em order crescente: forward = 1.
    //Caso contrário 0: Conta em ordem decrescente.

    unsigned char set_segF[10] = {0x7D,0x0C,0xB5,0x9D,0xCC,
                                 0xD9,0xF9,0x0D,0xFD,0xDD};

    unsigned char set_segB[10] = {0xDD,0xFD,0x0D,0xF9,0xD9,
                                  0xCC,0x9D,0xB5,0x0C,0x7D};
    int i;
    for(i = 0; i<10; i++){
        
        if(forward) PORTB = set_segF[i];
        else PORTB = set_segB[i];
        tempo_ms(time);
    }

    PORTB = 0xFD;

}