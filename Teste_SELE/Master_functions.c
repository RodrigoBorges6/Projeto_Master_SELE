/*
 * Master_functions.c

 *
 *  Created on: 26/11/2017
 *      Author: pedrorodrigues
 */

#include "Master_functions.h"

void init_io(void) {

	/* DDRD |= (1 << Led_controlo_MAX485); */
	DDRB |= (1 << LED_Vermelho);
	DDRB |= (1 << LED_Amarelo);
	DDRB |= (1 << LED_Verde);
	DDRB &= ~(1 << Conf_buttom); // declarar como uma entrada
	PORTB |= (1 << Conf_buttom); //pull up
}

void init_RS485(void) {
	/* Set baud rate */
	UBRR0H = (uint8_t) (baudgen >> 8); /*higher part of baudrate value */
	UBRR0L = (uint8_t) baudgen; /*lower part of baudrate value */

	/* Just in case ;) */
	/* UCSR0A = 0; *//* registo de flags pagina 192 */

	/* receiver and transmitter enable pagina 193 */
	UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << UCSZ02);

	/* Set frame format pagina 194: 9data, 1stop bit sem paridade*/
	UCSR0C = (7 << UCSZ00)/*(1<<USBS0)seria para 2 stop bits */
			| (0 << UPM00) /* no parity */
			| (0 << USBS0) /* 1 stop bit */
			| (0 << UMSEL00) | (0 << UMSEL01);/* comunicacao assincrona */
	/* direction */
	DDRB |= (1 << controlo_MAX485);/* Definir pino como saída PB2 */
}

void RS485_sendByte(uint8_t temp) {
	/* Wait for empty transmit buffer */
	while ((UCSR0A & (1 << UDRE0)) == 0)
		;
	/* Put data into buffer, sends the data */
	UDR0 = temp;
	UCSR0A |= 1 << TXC0;
	while ((UCSR0A & (1 << TXC0)) == 0)
		; /* espera até ter enviado o byte */
}

uint8_t RS485_receiveByte(void) {
	/* Wait for data to be received */
	while ((UCSR0A & (1 << RXC0)) == 0)
		;
	/* Get and return received data from buffer */
	return UDR0;
}

uint8_t send_Address(uint8_t n_slave) {

	UCSR0B = UCSR0B | (1 << TXB80); /* Trama de endereços */

	RS485_sendByte(0x01);

	return 0;
}

uint8_t send_Lotacao(uint8_t semaforo) {

	UCSR0B = UCSR0B & ~(1 << TXB80); /* Trama de dados */

	if (1 == semaforo) {
		RS485_sendByte(0xFF);/* Parque ocupado - semaforo vermelho */
		MAX485_Receiving
		;
		if (0xFF == RS485_receiveByte()) {
			return 0;
		} else {
			return -1;
		}
	} else if (0 == semaforo) {
		RS485_sendByte(0xAA); /* Parque livre - semaforo verde */
		MAX485_Receiving
		;
		if (0xAA == RS485_receiveByte()) {
			return 0;
		} else {
			return -1;
		}
	} else {
		return 1;
	}

}

uint8_t check_slave(uint8_t n_slave) {
	if (0 == send_Address(n_slave)) {
		if (0 == RS485_receiveByte()) {
			return 0;
		} else {
			return 1;
		}
	} else {
		return 1;
	}
}
/*
 void init_timer_T1(void) {

 //este timer conta segundos, escala = 1s

 TCCR1B = 0;		//parar TC1
 TCCR1A = 0;		//modo normal
 TCNT1 = 0xC2F7;	//valor para iniciar o timer (65536 - (1 - 16MHz / 1024)) = 4991

 TIMSK1 |= (1 << TOIE1);	//ativar a interupção por overflow

 TCCR1B |= (1 << CS12) | (1 << CS10);		// timer 1 com predivisão de 1024
 }

 void init_interrupt(void) {
 //definir interrupções
 sei();

 }
 */

