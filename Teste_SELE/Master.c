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

	int check_status;
	init_io();
	init_RS485();
	RS485_sendingMode();
	if(send_Want2talk(1) == 0){
		RS485_receivingMode();
		check_status = read_Response(1);
		if(check_status == 0){
			printf("Comunicação com escravo 1 concluída \n");
		}
		else if(check_status == -1){
			printf("Resposta de slave errado \n");
		}
		else if(check_status == -2){
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



