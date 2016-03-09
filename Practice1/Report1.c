//Library kindly made available by Sandro Juca to make it easier to work
//with uC via USB.
#include "SanUSB1.h"

//Must be here or inside the firmware.
#pragma interrupt interrupcao //Tem que estar aqui ou dentro do firmware.c

//To handle interruptions. As in this program we won't have interruption, no 
//method is necessary here.
void interrupcao() {}


void main(){
	//Clock by which PIC will have its time reference
	clock_int_4MHz();

	//Infinite loop, so PIC will always execute the instructions inside main.
	while(1)
	{
		//If the indicating bottom to override the current program has been pressed.
		if(!entrada_pin_e3){Reset();}//pressionar o botão para gravação

		//If not, toggle the output of pin b7
		inverte_saida(pin_b7);
		
		//Waits 1 sec in a loop
		tempo_ms(1000);
	}
}
