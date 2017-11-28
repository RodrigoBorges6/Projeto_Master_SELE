/*
 * Master_functions.c

 *
 *  Created on: 26/11/2017
 *      Author: pedrorodrigues
 */

#include "Master_functions.h"

void init_io(void)
{

	/* DDRD |= (1 << Led_controlo_MAX485); */
	DDRD |= (1 << LED_Vermelho);
	DDRD |= (1 << LED_Amarelo);
	DDRD |= (1 << LED_Verde);
	/*DDRD = DDRD | 0b00000100;*/  /*Definir pino como saída PD2 */
}

void init_RS485(void)
{
	/* Set baud rate */
	UBRR0H = (uint8_t)(baudgen >> 8); /*higher part of baudrate value */
	UBRR0L = (uint8_t) baudgen;		/*lower part of baudrate value */

	/* Just in case ;) */
	/* UCSR0A = 0; */ /* registo de flags pagina 192 */

	/* receiver and transmitter enable pagina 193 */
	UCSR0B = (1 << TXEN0)|(1 << RXEN0) | (1 << UCSZ02);

	/* Set frame format pagina 194: 9data, 1stop bit sem paridade*/
	UCSR0C = (3 << UCSZ00)/*(1<<USBS0)seria para 2 stop bits */
			| (0 << UPM00) /* no parity */
			| (0 << USBS0) /* 1 stop bit */
			| (0 << UMSEL00)
			| (0 << UMSEL01);/* comunicacao assincrona */
    /* direction */
	DDRB |= (1 << controlo_MAX485);/* Definir pino como saída PB2 */
}

void RS485_sendByte(uint8_t temp)
{
	/* Wait for empty transmit buffer */
	while ( (UCSR0A & (1 << UDRE0)) == 0 ) ;
	/* Put data into buffer, sends the data */
	UDR0 = temp;
	while ( (UCSR0A & (1 << TXC0)) == 0 ) ; /* espera até ter enviado o byte */
}

uint8_t RS485_receiveByte( void )
{
	/* Wait for data to be received */
	while ( (UCSR0A & (1 << RXC0)) == 0 );
	/* Get and return received data from buffer */
	return UDR0;
}

uint8_t send_Address(uint8_t n_slave)
{

	UCSR0B = UCSR0B | (1 << TXB80); /* Trama de endereços */

	RS485_sendByte(n_slave);

	return 0;
}

uint8_t send_Lotacao(uint8_t semaforo){

	UCSR0B = UCSR0B & ~(1 << TXB80); /* Trama de dados */

	if(1 == semaforo){
		RS485_sendByte(0xFF); /* Parque ocupado - semaforo vermelho */
		return 0;
	}
	else if(0 == semaforo){
		RS485_sendByte(0xAA); /* Parque livre - semaforo verde */
		return 0;
	}
	else {
		return 1;
	}

}



