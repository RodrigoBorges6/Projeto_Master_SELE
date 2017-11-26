/*
 * Master_functions.h
 *
 *  Created on: 26/11/2017
 *      Author: pedrorodrigues
 */

#ifndef MASTER_FUNCTIONS_H_
#define MASTER_FUNCTIONS_H_


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
#define LED_MAX485_OFF PORTD = PORTD & ~(1<<Led_controlo_MAX485)
//----------------------------------------------------------------------------------

void init_io(void);
void init_RS485();
void RS485_sendingMode();
void RS485_receivingMode();
void RS485_sendByte(unsigned char temp);
unsigned char RS485_receiveByte( void );
int send_Want2talk(uint8_t n_slave);
int read_Response(uint8_t n_slave);

//-----------------------------------------------------------------------------------



#endif /* MASTER_FUNCTIONS_H_ */
