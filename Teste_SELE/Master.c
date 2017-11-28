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

int main(void)
{

	uint8_t state = 0;
	uint8_t check = 0;
	uint8_t lotacao_MAX = 5;
	uint8_t lotacao_atual = 0;
	uint8_t lotacao_atual_percentagem = 0;
	uint8_t id_slave = 0x01;
	uint8_t n_slaves = 2; /* número de slaves */
	uint8_t aux = 0;
	uint8_t valor_contador_slave[2] = {0x00,0x00};


	/* Inicialização */
	init_io();
	init_RS485();
	MAX485_Sending;

	/* Máquina de estados */

	while(1)
	{

		if(0 == state)
		{

			check = send_Address(id_slave);
			state = 1;

		}
		else if((0 == check) && (1 == state))
		{

			MAX485_Receiving;
			valor_contador_slave[id_slave] = RS485_receiveByte();
			state = 2;
		}
		else if(2 == state)
		{

			lotacao_atual = 0;

			for(aux = 0; aux < n_slaves; aux++)
			{
				lotacao_atual = lotacao_atual + valor_contador_slave[aux];
			}

			lotacao_atual_percentagem = lotacao_atual * 100 / lotacao_MAX;

			MAX485_Sending;

			/* _delay_us(5); */
			if(lotacao_atual >= lotacao_MAX)
			{
				check = send_Lotacao(1); /* Lotação superior à lotação MAX */
				/*semaforo vermelho */
				LED_Vermelho_ON;
				LED_Amarelo_OFF;
				LED_Verde_OFF;
			}
			else if( 100 > lotacao_atual_percentagem && lotacao_atual_percentagem >= 81)
			{
				check = send_Lotacao(0);
				/* semaforo amarelo */
				LED_Vermelho_OFF;
				LED_Amarelo_ON;
				LED_Verde_OFF;
			}
			else
			{
				check = send_Lotacao(0);
				/* semaforo verde */
				LED_Vermelho_OFF;
				LED_Amarelo_OFF;
				LED_Verde_ON;
			}

			if(id_slave < n_slaves)
			{
				id_slave++;
			}
			else
			{
				id_slave = 0x01;
			}
			state = 0;
			return 0;
		}

	}

}

