#include "SanUSB1X.h"

//#pragma interrupt interrupcao
//void interrupt interrupcao(){}

int input;

void interrupt interrupcao(){
    if(ext1_interrompeu){
        ext1_interrompeu = 0;

        tempo_ms(10);
        while(ext1_interrompeu){}
        input += 25;
    }

}
void main(){

    //Página 115 da apostila
    input = 0;

    clock_int_4MHz();
    habilita_interrupcao(ext1);//pin_b0
    TRISB = 0b00000010;

    while(1){

        if(!entrada_pin_e3){//pressionar o botão para gravação
            Reset();
        }

        if(input > 100) input = 0;
        SetaPWM2(10000, input);
    }
}