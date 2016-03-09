#include "SanUSB1X.h"

//#pragma interrupt interrupcao
//void interrupt interrupcao(){}

#define TIME 2000
#define DELAY TIME*0.1

int pedestre = 0;

void interrupt interrupcao(){
    if(ext1_interrompeu){
        ext1_interrompeu = 0;

        pedestre = 1;
    }

}

void semaforo_carros(int time){
    //pin_a0 = vermelho
    //pin_a1 = amarelo
    //pin_a2 = verde

    //Vermelho - pedestre
    nivel_alto(pin_a3);

    //Verde
    nivel_alto(pin_a2);
    nivel_baixo(pin_a1);
    nivel_baixo(pin_a0);
    tempo_ms(time);

    //Amarelo
    nivel_alto(pin_a1);
    nivel_baixo(pin_a0);
    nivel_baixo(pin_a2);

    tempo_ms(time-time*.5);

    //Vermelho
    nivel_alto(pin_a0);
    nivel_baixo(pin_a1);
    nivel_baixo(pin_a2);
    if(pedestre == 0){
        tempo_ms(time);
    }else{
        tempo_ms(DELAY);
    }

}

void semaforo_pedestre(int time){
    //Tempo total 2*time
    //pin_a4 - Verde
    //pin_a3 - Vermelho

    int i = 8;//vezes que o verde irá piscar antes de comutar pra vermelho

    //Vermelho - Carros
    nivel_alto(pin_a0);
    nivel_baixo(pin_a1);
    nivel_baixo(pin_a2);

    //Pedestres
    nivel_baixo(pin_a3);
    tempo_ms(DELAY);

    //Semaforo pedestres
    nivel_alto(pin_a4);//Verde
    tempo_ms(time);

    while(i){//Pisca por algum tempo pra alertar pedestres
        i--;

        inverte_saida(pin_a4);
        tempo_ms(200);
    }

    nivel_alto(pin_a3);//Vermelho
    nivel_baixo(pin_a4);
    tempo_ms(DELAY);
}

void main(){

    //Página 115 da apostila
    float time = TIME;
    pedestre = 0;

    clock_int_4MHz();
    habilita_interrupcao(ext1);//pin_b0
    TRISB = 0b00000010;

    while(1){

        if(!entrada_pin_e3){//pressionar o botão para gravação
            Reset();
        }

        semaforo_carros(time);
        if(pedestre == 1){
            inverte_saida(pin_a4);
            pedestre = 0;

            semaforo_pedestre(time);
        }
    }
}
