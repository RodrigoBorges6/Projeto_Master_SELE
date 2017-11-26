/*
 * Master_functions.c

 *
 *  Created on: 26/11/2017
 *      Author: pedrorodrigues
 */

#include "Master_functions.h"

void init_io(void){

	DDRD |= (1 << Led_controlo_MAX485);
	//DDRD = DDRD | 0b00000100; //Definir pino como saída PD2
}

void init_RS485()
{
	/* Set baud rate */
	UBRR0H = (unsigned char)(baudgen >> 8); //higher part of baudrate value
	UBRR0L = (unsigned char) baudgen;		//lower part of baudrate value

	/* Just in case ;) */
	UCSR0A = 0; //registo de flags pagina 192

	/* receiver and transmitter enable pagina 193 */
	UCSR0B = (1 << TXEN0)|(1 << RXEN0);

	/* Set frame format pagina 194: 8data, 1stop bit sem paridade*/
	UCSR0C = (3 << UCSZ00);//(1<<USBS0)seria para 2 stop bits

    // direction
	DDRD |= (1 << controlo_MAX485);//Definir pino como saída PD2
	_delay_ms(500); // ???
}

void RS485_sendingMode()
{
	MAX485_Sending;
	LED_MAX485_ON;
}

// RS485
void RS485_receivingMode()
{
	MAX485_Receiving;
	LED_MAX485_OFF;
}

void RS485_sendByte(unsigned char temp)
{
	///* Wait for empty transmit buffer */
	while ( !(UCSR0A & (1 << UDRE0)) ) ;
	/* Put data into buffer, sends the data */
	UDR0 = temp;
}

unsigned char RS485_receiveByte( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1 << RXC0)) ) ;
	/* Get and return received data from buffer */
	return UDR0;
}

int send_Want2talk(uint8_t n_slave){
	int i;
	uint8_t* trama;
	trama = (uint8_t *) malloc(2);

	trama[0] = n_slave;
	trama[1] = 0x03;

	for(i = 0 ; i <= 2; i++){
		RS485_sendByte(trama[i]);
	}
	return 0;
}

int read_Response(uint8_t n_slave){
	int i;
	uint8_t* trama_R;
	trama_R = (uint8_t *) malloc(2);

	for(i = 0; i <= 2 ; i++){
		trama_R[i] = RS485_receiveByte();
	}

	if(n_slave == trama_R[0]){
		if(0x00 == trama_R[1]){
			//Não quer falar
			printf("Escravo não quer falar \n");
			return 0;
		}
		else if(0x01 == trama_R[1]){
			//Quer falar
			printf("Escravo quer falar \n");
			return 0;
		}
		else{
			return -2;
		}
	}
	else{
		return -1;
	}

}



