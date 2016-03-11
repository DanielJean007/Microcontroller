#include "SanUSB1X.h"

//#pragma interrupt interrupcao
void interrupt interrupcao(){}

short int A, B, C, D, E;        //Entradas
short int a, b, c, d, e, f, g;  //Saídas

void decod(short int A, short int B, short int C, short int D, short int E);
void display_alphabet(int tempo);

void main (void)
{
    int tempo = 1000;

    clock_int_4MHz();

     //Setting all B pins to be outputs, excepting pin b1
    TRISB = 0b00000010;
    PORTB = 0x00;

    while (1)
    {
        if (entrada_pin_e3==0){
            Reset();
        }
        display_alphabet(tempo);
    }
}

void decod(short int A, short int B, short int C, short int D, short int E){

    a = !A&!C&!D&E | !A&B&!C&!D | !A&!D&B&E |!A&!B&!D&!E&C |!A&!E&B&C&D| A&!B&!C&!D&!E;
    saida_pino(pin_b0, a);

    b = !A&!B&!C&!D | !A&!C&D&E | !B&!C&D&E | !A&!E&B&D | !A&!D&B&C&E;
    saida_pino(pin_b2, b);

    c = !B&!C&!E | !A&!C&E | !B&!C&D | !A&!E&D | !A&!E&B&C;
    saida_pino(pin_b3, c);

    d = !A&!B&C | !B&!C&A | !A&!B&!D&!E | !A&!D&!E&C | !A&!C&B&E;
    saida_pino(pin_b4, d);

    e = !A&!D | !A&!B&!E | !A&!C&!E | !A&C&E | !B&!C&!D&E | !B&!C&!E&D;
    saida_pino(pin_b5, e);

    f = !A&!D&E | !A&!B&!D&C | !A&!E&C&D | !A&!C&!E&B | !B&!C&!D&A | !B&!C&A&E;
    saida_pino(pin_b6, f);

    g = !A&!E  | !B&!C&!D | !A&C&D | !A&!D&B | !B&!C&A&E;
    saida_pino(pin_b7, g);
}

void display_alphabet(int tempo){
    unsigned char alphabet[] = {0xED, 0xF8, 0x71, 0xBC, 0xF1,
                                0xE1, 0xF9, 0xEC, 0x0C, 0x3C,
                                0x70, 0x6D, 0x7D, 0xE5, 0xCD,
                                0xA0, 0xD9, 0xF0, 0x7C, 0xDC};
    //a, b, c, d, e, f, g, h, i, j, l, n, o, p, q, r, s, t, u, y

    int i;
    for(i = 0; i < 20; i++){
        PORTB = alphabet[i];
        tempo_ms(tempo);
    }
}