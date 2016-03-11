//Utiliza interrupcao serial para receber comandos enviados via bluetooth ou zigbee
#include "SanUSB1X.h" //

short int msg_rcvd = 0;
int tamanho = 0;
char ser_input[50];
//unsigned char input;

void interrupt interrupcao();
void display_alphabet(int tempo, unsigned char letter);
int is_displayable(unsigned char letter);
unsigned char to_lower(unsigned char letter);
unsigned char to_upper(unsigned char letter);


void main(){

    clock_int_4MHz();
    habilita_interrupcao(recep_serial);
    taxa_serial(9600);
    int i;

    //Setting all B pins to be outputs, excepting pin b1
    TRISB = 0b00000010;
    PORTB = 0x00;
    
    while(1){

        //com interrupcao evitar no while, pois pode dar conflito com a gravacao
        if (entrada_pin_e3 == 0){
            Reset();
        }

        if(msg_rcvd == 1){
            msg_rcvd = 0;
            sendrw((char *)"Imprimindo no display...\r\n" );

            for(i = 0; i < tamanho; i++){
                //sendrw((char *) "Olha: \r\n");
                display_alphabet(500, ser_input[i]);
            }
            sendrw((char *)"Pronto.\r\n" );
            tamanho = 0;
        }
    }
}

void interrupt interrupcao()
{
 if (serial_interrompeu)  {
     serial_interrompeu=0;

     unsigned char input = le_serial();
     
     if(input != '.'){
        ser_input[tamanho] = le_serial();
        tamanho++;

     }else{
//        inverte_saida(pin_b7);
//        tempo_ms(500);
//        inverte_saida(pin_b7);
//        tempo_ms(500);
        msg_rcvd = 1;

     }
     
   }
}

void display_alphabet(int tempo, unsigned char letter){
    unsigned char alphabet[] = {0xED, 0xF8, 0x71, 0xBC, 0xF1,   //A, b, C, d, E,
                                0xE1, 0xF9, 0xEC, 0x0C, 0x3C,   //F, G, H, I, J,
                                0x70, 0x6D, 0x7D, 0xE5, 0xCD,   //L, n, O, p, q,
                                0xA0, 0xD9, 0xF0, 0x7C, 0xDC};  //r, S, t, U, y

//    int i;  //Increments over the alphabet
    int displayable = is_displayable(letter);

    if(displayable != -1){
        PORTB = alphabet[displayable];
        tempo_ms(tempo);
        PORTB = 0x00;
        tempo_ms(tempo/3);

    }else if (displayable == -1){
        sendrw((char *) "Undefined character. Sorry...\r\n");
    }

//    for(i = 0; i < 20; i++){    //Prints all characters of the alphabet
//        PORTB = alphabet[i];
//        tempo_ms(tempo);
//        PORTB = 0x00;
//        tempo_ms(tempo/2);
//    }
}

int is_displayable(unsigned char letter){
    //Available and acceptable characters:
    unsigned char alphabet[] = {'A', 'b', 'C', 'd', 'E',
                                'F', 'G', 'H', 'I', 'J',
                                'L', 'n', 'O', 'p', 'q',
                                'r', 'S', 't', 'U', 'y'};

    int i;
    for(i = 0; i < 20; i++){
        if(to_lower(letter) == alphabet[i] || to_upper(letter) == alphabet[i]){
            return i;
        }
    }
    
    return -1;
}

unsigned char to_lower(unsigned char letter){
    return (letter >= 97 && letter <= 122)?letter - 32:letter;
}

unsigned char to_upper(unsigned char letter){
    return (letter >= 65 && letter <= 90)?letter + 32:letter;
}