#include "SanUSB1X.h"

//#pragma interrupt interrupcao
void interrupt interrupcao(){}

short int A, B, C;              //Entradas
short int a, b, c, d, e, f, g;  //Saídas

void decod(short int A, short int B, short int C);

void main (void)
{
    clock_int_4MHz();

    while (1)
    {
        decod(0,0,0);   //d
        tempo_ms(1000);

        decod(0,0,1);   //A
        tempo_ms(1000);

        decod(0,1,0);   //n
        tempo_ms(1000);

        decod(0,1,1);   //I
        tempo_ms(1000);

        decod(1,0,0);   //E
        tempo_ms(1000);

        decod(1,0,1);   //L
        tempo_ms(1000);
    }
}

void decod(short int A, short int B, short int C){

    a = !A&!B&C | !A&B&!C | A&!B&!C;
    saida_pino(pin_b0, a);
    
    b = !A;
    saida_pino(pin_b2, b);
    
    c = !A;
    saida_pino(pin_b3, c);
    
    d = !B&!C | A&!B;
    saida_pino(pin_b4, d);
    
    e = !B | !A&!C;
    saida_pino(pin_b5, e);
    
    f = !B&C | A&!B | !A&B&!C;
    saida_pino(pin_b6, f);
    
    g = !A&!B | !B&!C;
    saida_pino(pin_b7, g);
}