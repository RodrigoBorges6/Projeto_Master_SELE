/*
 * testar.c
 *
 *  Created on: 24/11/2017
 *      Author: rodrigoborges
 */

/*
 * Programa para começar a testar o arduino +
 * MAX485 - transceiver para RS-485
*/

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
//#include <util/delay.h>
//#include "serial_port.h"
//#include <math.h>
//#include <string.h>
//#include <avr/eeprom.h>
//#include "timer_tools.h"
//#include "printf_tools.h"

#define F_CPU 16000000UL
#define	baud 57600  // baud rate
// Caution: erroneous result if F_CPU is not correctly defined
#define baudgen ((F_CPU/(16*baud))-1)  //baud divider

#define controlo_MAX485 PD4 //Quando está a 1, funciona como sender. Quando está a zero funciona como receiver.

void init_io(void);

int main(void){

	init_io();

}

void init_io(void){

	DDRD |= (1 << controlo_MAX485); //definir pino PD2 como uma saída

}



