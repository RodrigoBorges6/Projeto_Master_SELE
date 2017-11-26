/*
 * Master.c
 *
 *  Created on: 24/11/2017
 *      Author: PedroRodrigues + RodrigoBorges
 */

/*
 * Programa parad começar a testar o arduino +
 * MAX485 - transceiver para RS-485
*/

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "serial_port.h"
#include <stdbool.h>
//#include <math.h>
#include <string.h>
//#include <avr/eeprom.h>
//#include "timer_tools.h"
//#include "printf_tools.h"

#define F_CPU 16000000UL
#define	baud 57600  // baud rate
// Caution: erroneous result if F_CPU is not correctly defined
#define baudgen ((F_CPU/(16*baud))-1)  //baud divider

#define controlo_MAX485 PD2 //Quando está a 1, funciona como sender. Quando está a zero funciona como receiver.

#define MAX485_Sending PORTD |= (1 << controlo_MAX485) //Coloca 1 na saída PD2 para pôr o transceiver em modo Sender
#define MAX485_Receiving PORTD = PORTD & ~(1<<controlo_MAX485); //Coloca 1 na saída PD2 para pôr o transceiver em modo Receiver
#define Led_controlo_MAX485 PD3
#define LED_MAX485_ON PORTD |= (1 << Led_controlo_MAX485)
#define LED_MAX485_OFF PORTD = PORTD & ~(Led_controlo_MAX485)


//void init_io(void);
void init_RS485();
void RS485_sendingMode();
void RS485_receivingMode();
void RS485_sendByte(unsigned char temp);
unsigned char RS485_receiveByte( void );
int send_Want2talk(uint8_t n_slave);
int read_Response(uint8_t n_slave);

//-----------------------------------------------------------------------------------
int main(void){

	int check_status;
	//init_io();
	init_RS485();
	RS485_sendingMode();

	if(send_Want2talk(1) == 0){
		RS485_receivingMode();
		check_status = read_Response(1);
		if(check_status == 0){
			printf("Comunicação com escravo 1 concluída \n");
		}
		else if(check_status == -1){
			printf("Resposta de slave errado \n");
		}
		else if(check_status == -2){
			printf("Segundo valor da trama_R não coerente \n");
		}
		else{
			printf("Erro não conhecido \n");
		}
	}
	else{
		printf("Erro ao enviar a mensagem de Want2talk \n");
	}


}


/*
void init_io(void){

	DDRD |= (1 << controlo_MAX485);//Definir pino como saída PD2
	//DDRD = DDRD | 0b00000100; //Definir pino como saída PD2
}
*/
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

	if(trama_R[0] == n_slave){
		if(trama_R[1] == 0x00){
			//Não quer falar
			printf("Escravo não quer falar \n");
			return 0;
		}
		else if(trama_R[1] == 0x01){
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

