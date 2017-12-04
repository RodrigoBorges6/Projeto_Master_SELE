/*
 * Master_functions.c

 *
 *  Created on: 26/11/2017
 *      Author: pedrorodrigues
 */

#include "RS485.h"


volatile uint16_t watchdog = 0; /* contador do watchdog */
volatile uint8_t watchdog_flag = 0; /* flag que indica se o watchdog ativou */

void init_RS485(void) {

	/* Set baud rate */
	UBRR0H = (uint8_t) (baudgen >> 8); /*higher part of baudrate value */
	UBRR0L = (uint8_t) baudgen; /*lower part of baudrate value */

	/* receiver and transmitter enable pagina 193 */
	UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << UCSZ02);

	/* Set frame format pagina 194: 9data, 1stop bit sem paridade*/
	UCSR0C = (7 << UCSZ00)/*(1<<USBS0)seria para 2 stop bits */
	| (0 << UPM00) /* no parity */
	| (0 << USBS0) /* 1 stop bit */
	| (0 << UMSEL00) | (0 << UMSEL01);/* comunicacao assincrona */

	DDRB |= (1 << controlo_MAX485);/* Definir pino de controlo como saída PB2 */
}

void init_timer_T1(void) {

	/*este timer conta mili segundos, escala = 1ms */

	TIMSK1 |= (1 << OCIE1A); /* interrupçao quando atinge OCR1A */

	TCCR1B = 0;		/*parar TC1 */
	TCCR1A = 0;		/*modo normal */

	TCCR1B |= (1 << CS10); /* sem pre divisao */

	OCR1A = 16000; /* valor onde se dá a interrupt */

}

void init_interrupt(void) {

	sei(); /* Ativar interrupts */

}

ISR (TIMER1_COMPA_vect) { /* interrupt a cada 1ms */

	TCNT1 = 0; /* reiniciar o timer */
	watchdog++; /* incrementar o contador */

	if (watchdog >= 1000) { /* Se ja contou 1000 tickets ou seja 1 segundo */

		watchdog_flag = 1; /* ativar watchdog */

		return;

	} else {

		return;

	}
}

void reset_watchdog(void) {

	TCNT1 = 0; /* Reiniciar timer */
	watchdog = 0; /* Reiniciar contador */
	watchdog_flag = 0; /* Reiniciar flag do watchdog */

	return;
}

void RS485_sendByte(uint8_t temp) {

	while ((UCSR0A & (1 << UDRE0)) == 0); /* Wait for empty transmit buffer */

	UDR0 = temp; /* Put data into buffer, sends the data */

	UCSR0A |= (1 << TXC0);

	while ((UCSR0A & (1 << TXC0)) == 0);/* espera até ter enviado o byte */
}

char RS485_receiveByte(void) {

	reset_watchdog();	/* Reiniciar watchdog para o usar */

	while ((UCSR0A & (1 << RXC0)) == 0) {	/* Wait for data to be received */

		if (1 == watchdog_flag) {	/* Se o watchdog tiver ativado, considerar que o slave nao esta operacional */

			return watchdog_timeout;

		}
		/* Else não necessário */
	}

	return UDR0;/* Get and return received data from buffer */

}

void send_Address(uint8_t n_slave) {

	UCSR0B = UCSR0B | (1 << TXB80); /* Ativar o 9º, ou sejaTrama de endereços */

	RS485_sendByte(n_slave); /* Enviar o endereço do slave */

	return;
}