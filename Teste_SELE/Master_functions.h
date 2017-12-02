/*
 * Master_functions.h
 *
 *  Created on: 26/11/2017
 *      Author: pedrorodrigues
 */

#ifndef MASTER_FUNCTIONS_H_
#define MASTER_FUNCTIONS_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include "serial_port.h"

/*
 //#include <stdio.h>
 //#include <stdlib.h>
 //#include <avr/interrupt.h>
 //#include "serial_port.h"
 //#include <stdbool.h>
 //#include <math.h>
 //#include <string.h>
 //#include <avr/eeprom.h>
 //#include "timer_tools.h"
 //#include "printf_tools.h"
 */

#define F_CPU 16000000UL
#define	baud 57600  /* baud rate ; Caution: erroneous result if F_CPU is not correctly defined */
#define baudgen ((F_CPU/(16*baud))-1)  /*baud divider*/
#define controlo_MAX485 PB2 /*Quando está a 1, funciona como sender. Quando está a zero funciona como receiver.*/
#define MAX485_Sending PORTB |= (1 << controlo_MAX485) /*Coloca 1 na saída PD2 para pôr o transceiver em modo Sender */
#define MAX485_Receiving PORTB &= ~(1<<controlo_MAX485); /*Coloca 0 na saída PD2 para pôr o transceiver em modo Receiver */
#define LED_Vermelho PB3
#define LED_Vermelho_ON PORTB |= (1 << LED_Vermelho);
#define LED_Vermelho_OFF PORTB &= ~(1 << LED_Vermelho);
#define LED_Amarelo PB4
#define LED_Amarelo_ON PORTB |= (1 << LED_Amarelo);
#define LED_Amarelo_OFF PORTB &= ~(1 << LED_Amarelo);
#define LED_Verde PB5
#define LED_Verde_ON PORTB |= (1 << LED_Verde);
#define LED_Verde_OFF PORTB &= ~(1 << LED_Verde);
#define Conf_buttom PB1

#define STATE_ADDR_SEND 0
#define STATE_CONT_RECEIVE 1
#define STATE_CALC_SEND 2


#define watchdog_timeout 99
/*
 #define Led_controlo_MAX485 PD3
 #define LED_MAX485_ON PORTD |= (1 << Led_controlo_MAX485)
 #define LED_MAX485_OFF PORTD = PORTD & ~(1<<Led_controlo_MAX485)
 */
void init_io(void);
void init_RS485(void);
void RS485_sendByte(uint8_t temp);
char RS485_receiveByte(void);
void send_Address(uint8_t n_slave);
uint8_t send_Lotacao(uint8_t semaforo);
uint8_t check_slave(uint8_t n_slave);
void init_timer_T1(void);
void init_interrupt(void);
void reset_watchdog (void);
void configuration_mode(void);


#endif /* MASTER_FUNCTIONS_H_ */
