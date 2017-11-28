/*
 * Master.c
 *
 *  Created on: 24/11/2017
 *      Author: PedroRodrigues + RodrigoBorges
 */

/*
 * Programa parad começar a testar o arduino +
 * MAX485 - transceiver para RS-485
*/


#include "Master_functions.h"

int main(void){

	uint8_t state = 0;
	uint8_t check = 0;
	uint8_t lotacao_MAX = 5;
	uint8_t lotacao_atual = 0;
	uint8_t id_slave = 0x01;
	uint8_t n_slaves = 1; //número de slaves
	uint8_t aux;
	uint8_t valor_contador_slave[2] = {0x00,0x00};


	//Inicialização
	init_io();
	init_RS485();
	RS485_sendingMode();

	//Máquina de estados

	while(1){

		if(0 == state){

			check = send_Address(id_slave);
			state = 1;

		}
		else if((0 == check) && (1 == state)){

			RS485_receivingMode();
			valor_contador_slave[id_slave] = RS485_receiveByte();
			state = 2;
		}
		else if(2 == state){

			lotacao_atual = 0;

			for(aux = 1; aux <= n_slaves; aux++){
				lotacao_atual = lotacao_atual + valor_contador_slave[aux];
			}

			RS485_sendingMode();
			//_delay_us(5);
			if(lotacao_atual >= lotacao_MAX){
				check = send_Lotacao(1); //Lotação superior à lotação MAX
			}
			else {
				check = send_Lotacao(0);
			}
			LED_MAX485_ON;
			state = 0;
			return 0;
		}

	}

}

