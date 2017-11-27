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
	UBRR0H = (uint8_t)(baudgen >> 8); //higher part of baudrate value
	UBRR0L = (uint8_t) baudgen;		//lower part of baudrate value

	/* Just in case ;) */
	//UCSR0A = 0; //registo de flags pagina 192

	/* receiver and transmitter enable pagina 193 */
	UCSR0B = (1 << TXEN0)|(1 << RXEN0);

	/* Set frame format pagina 194: 9data, 1stop bit sem paridade*/
	UCSR0C = (7 << UCSZ00);//(1<<USBS0)seria para 2 stop bits

    // direction
	DDRD |= (1 << controlo_MAX485);//Definir pino como saída PD2
	//_delay_ms(500); // ???
}

// RS485
void RS485_sendingMode()
{
	MAX485_Sending;
	//LED_MAX485_ON;
}

void RS485_receivingMode()
{
	MAX485_Receiving;
	//LED_MAX485_OFF;
}

void RS485_sendByte(uint8_t temp)
{
	///* Wait for empty transmit buffer */
	while ( (UCSR0A & (1 << UDRE0)) == 0 ) ;
	/* Put data into buffer, sends the data */
	UDR0 = 0x01;
}

uint8_t RS485_receiveByte( void )
{
	/* Wait for data to be received */
	while ( (UCSR0A & (1 << RXC0)) == 0 );
	/* Get and return received data from buffer */
	return UDR0;
}

uint8_t send_Address(uint8_t n_slave){

	UCSR0B = UCSR0B | (1 << TXB80); //Trama de endereços

	RS485_sendByte(n_slave);

	return 0;
}

uint8_t send_Lotacao(uint8_t semaforo){

	UCSR0B = (0 << TXB80); //Trama de dados

	if(1 == semaforo){
		RS485_sendByte(0xFF); //Parque ocupado - semaforo vermelho
		return 0;
	}
	else if(0 == semaforo){
		RS485_sendByte(0xAA); //Parque livre - semaforo verde
		return 0;
	}
	else {
		return 1;
	}

}



