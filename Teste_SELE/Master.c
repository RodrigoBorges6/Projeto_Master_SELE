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
//----------------------------------------------------------------------------------
void init_io(void){

	DDRD |= (1 << controlo_MAX485);
	//DDRD = DDRD | 0b00000100; //Definir pino como saída PD2
}

int send_Want2talk(uint8_t n_slave){
	uint8_t* trama;
	trama = (uint8_t *) malloc(2);

	trama[0] = n_slave;
	trama[1] = 0x03;

	write_string(trama);
	return 0;
}

int read_Response(uint8_t n_slave){
	uint8_t* trama_R;
	trama_R = (uint8_t *) malloc(2);

	if(read_string(trama_R) == 1){ //acho que é 1 para estar tudo bem
		if(trama_R[0] == n_slave){
			//checkar se quer falar ou não
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
			else return -3;
		}
		else return -2;
	}
	else return -1;

}

//-----------------------------------------------------------------------------------
int main(void){

	init_io();
	int check_status;

	if(send_Want2talk(1) == 0){
		check_status = read_Response(1);
		if(check_status == 0){
			printf("Comunicação com escravo 1 concluída \n");
		}
		else if(check_status == -1){
			printf("Erro na leitura da porta série \n");
		}
		else if(check_status == -2){
			printf("Resposta de slave errado \n");
		}
		else if(check_status == -3){
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




