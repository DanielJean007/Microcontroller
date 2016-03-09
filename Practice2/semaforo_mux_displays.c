#include "SanUSB1X.h"

//#pragma interrupt interrupcao
//void interrupt interrupcao(){}

#define TIME 2000
#define DELAY TIME*0.1

int pedestre = 0;

void interrupt interrupcao();
void semaforo_carros(int time);                 //Method to control the traffic lights for cars
void semaforo_pedestre(int time);               //Method to control the traffic lights for pedestrians
void count_display(int time, int forward);      //Method that counts a display increasingly or decreasingly for one 7seg-display
void count_display_mux(int time, int forward);  ////Method that counts a display increasingly or decreasingly by multiplexing 2 7seg-displays.

void main(){

    float time = TIME;
    pedestre = 0;

    clock_int_4MHz();

    //To enable external interruption in pin b7
    habilita_interrupcao(ext1);//pin_b0
    
    //Setting all B pins to be outputs, excepting pin b1
    TRISB = 0b00000010;
    PORTB = 0x00;


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

//When PIC receives an interruption in pin b1, it pauses all the processing and comes to this function
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

    //Not such a good idea to do this calculation: time-time*.5 in the method tempo_ms, but... here we go...
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

    //This variable stablishes number of times the green led is gonna blink before the red led comes along.
    int i = 6;//vezes que o verde irá piscar antes de comutar pra vermelho

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

    //Before switching to red, we'll warn the pedestrian, so they can run for their lives.
    count_display_mux(100, 0);

    nivel_alto(pin_a5);
    nivel_baixo(pin_a4);
    while(i){//Pisca por algum tempo pra alertar pedestres
        i--;

        //On the top of blinking and counting we added a buzzer to indicate the green led is gonna to switch soon.
        inverte_saida(pin_a5);//buzzer
        inverte_saida(pin_a4);
        tempo_ms(300);

        if(i == 0){
            nivel_baixo(pin_a5);
            nivel_alto(pin_a4);
            tempo_ms(TIME/4);
        }        
    }
    nivel_alto(pin_a5);
    nivel_baixo(pin_a4);

    nivel_alto(pin_a3);//Vermelho
    nivel_baixo(pin_a4);
    tempo_ms(DELAY);
}

void count_display(int time, int forward){
    //Se o usuário quer contar em order crescente: forward = 1.
    //Caso contrário 0: Conta em ordem decrescente.

    unsigned char set_segF[10] = {0x7D,0x0C,0xB5,0x9D,0xCC,
                                 0xD9,0xF9,0x0D,0xFD,0xDD};
                                 //{9, }

    unsigned char set_segB[10] = {0xDD,0xFD,0x0D,0xF9,0xD9,
                                  0xCC,0x9D,0xB5,0x0C,0x7D};
    int i;
    for(i = 0; i<10; i++){

        if(forward) PORTB = set_segF[i];
        else PORTB = set_segB[i];
        tempo_ms(time);
    }

    PORTB = 0x00;

}

void count_display_mux(int time, int forward){
    //Se o usuário quer contar em order crescente: forward = 1.
    //Caso contrário, 0: Conta em ordem decrescente.

    unsigned char set_segF[10] = {0x7D,0x0C,0xB5,0x9D,0xCC,
                                 0xD9,0xF9,0x0D,0xFD,0xDD};
                                //{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
    unsigned char set_segB[10] = {0xDD,0xFD,0x0D,0xF9,0xD9,
                                  0xCC,0x9D,0xB5,0x0C,0x7D};
                                //{9, 8, 7, 6, 5, 4, 3, 2, 1, 0}
    int i,dez,uni,t;

    for(i = 0; i<100; i++){     //Repete 99 times.

        for(t = 0; t<time/10; t++){
            //A grosso modo, pega-se o tempo desejado e divide em 10, o que
            //será mostrado dentro desse loop.
            dez = i/10;
            uni = i%10;
            
            nivel_baixo(pin_c1);     //Display 1
            nivel_alto(pin_c2);

            if(forward) PORTB = set_segF[dez];
            else PORTB = set_segB[dez];
            tempo_ms(5);

            nivel_baixo(pin_c2);     //Display 2
            nivel_alto(pin_c1);

            if(forward) PORTB = set_segF[uni];
            else PORTB = set_segB[uni];
            tempo_ms(5);
        }
        
    }

    PORTB = 0x00;
}