/*
 * RS485.h
 *
 *      Author: pedrorodrigues
 */

#ifndef RS485_H_
#define RS485_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "serial_port.h"


#define F_CPU 16000000UL
#define	baud 57600  /* baud rate ; Caution: erroneous result if F_CPU is not correctly defined */
#define baudgen ((F_CPU/(16*baud))-1)  /*baud divider*/

#define controlo_MAX485 PB2 /*Quando está a 1, funciona como sender. Quando está a zero funciona como receiver.*/
#define MAX485_Sending PORTB |= (1 << controlo_MAX485) /*Coloca 1 na saída PD2 para pôr o transceiver em modo Sender */
#define MAX485_Receiving PORTB &= ~(1<<controlo_MAX485); /*Coloca 0 na saída PD2 para pôr o transceiver em modo Receiver */

#define Conf_buttom PB1


#define watchdog_timeout 99	/* valor peseudo-aleatorio, apenas para identificar timeout */


/* Configura o RS485 e o seu pino de controlo */
void init_RS485(void);

/* Configura o Timer 1
 * para funcionar sem pre-divisao em CTC mode
 * de forma a contar de um em um milisegundo
 */
void init_timer_T1(void);

/* Ativa as interrupts */
void init_interrupt(void);

/* Reset do watchdog */
void reset_watchdog (void);

/* Envia um byte pelo RS485 */
void RS485_sendByte(uint8_t temp);

/* Recebe um byte pelo RS485 */
char RS485_receiveByte(void);

/* Enviar address ao slave */
void send_Address(uint8_t n_slave);



#endif /* RS485_H_ */
