/*
 * serial_port.h
 *
 *  Created on: 30/11/2015
 *      Author: rodrigoborges
 */

#ifndef SERIAL_PORT_H_
#define SERIAL_PORT_H_

#include <avr/interrupt.h>

#define F_CPU 16000000UL

#define	baud 57600  // baud rate
/* Caution: erroneous result if F_CPU is not correctly defined*/
#define baudgen ((F_CPU/(16*baud))-1)  /*baud divider*/

/*inicia a porta de serie do ATmega328P*/
void init_USART(void);

/*tem como argumento o caracter que queremos enviar atrazes do Tx*/
void print_char(char c);

/*Envia uma string pela porta de serie apontada por str*/
void write_string(char *str);

/*retorna um char que é lido atravez de Rx*/
char get_char(void);

/*retorna 1 quando acaba da ler uma string, 0 se ainda não acabou de ler*/
/*a fim da leitura e detetado quando é lido a caracter \n ou \r*/
int read_string(char *str);

#endif /* SERIAL_PORT_H_ */
