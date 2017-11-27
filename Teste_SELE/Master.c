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
	uint8_t id_slave = 1;
	uint8_t n_slaves = 1; //número de slaves
	uint8_t aux;
	uint8_t valor_contador_slave[2] = {0,0};


	//Inicialização
	init_io();
	init_RS485();
	RS485_sendingMode();

	//Máquina de estados

	while(1){

		if(0 == state){
			//check = send_Address(id_slave);
			//state = 1;

			while (1)
				{
					while( ( UCSR0A & ( 1 << UDRE0 ) ) == 0 ){} // Wait for ready
					UDR0 = 0x61;	                // Send HEX code for ASCII a
				}

		}
		else if((0 == check) && (1 == state)){

			RS485_receivingMode();
			valor_contador_slave[id_slave] = RS485_receiveByte();
			state = 2;
			LED_MAX485_ON;
		}
		else if(2 == state){

			lotacao_atual = 0;

			for(aux = 1; aux <= n_slaves; aux++){
				lotacao_atual = lotacao_atual + valor_contador_slave[aux];

				//printf("Lotação atual: %ud\n", &lotacao_atual);
			}

			RS485_sendingMode();

			if(lotacao_atual >= lotacao_MAX){
				check = send_Lotacao(1); //Lotação superior à lotação MAX
			}
			else {
				check = send_Lotacao(0);
			}

			state = 0;
		}

	}

}

