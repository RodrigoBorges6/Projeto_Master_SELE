/*
 * Master.c
 *
 *  Created on: 24/11/2017
 *      Author: PedroRodrigues + RodrigoBorges
 */

#include "Master_functions.h"

int main(void) {

	uint8_t aux = 0; /*variavel auxiliar dos ciclos for */
	uint8_t state = 0; /* variavel da state machine de comunicaçao */
	uint8_t cont_SM = 0; /* contador incremental para percorrer os slaves, incrementado a cada ciclo da SM*/
	char byte = 0; /* variavel auxiliar para receber o byte enviado pelo slave */


	uint8_t lotacao_MAX = 6;
	char lotacao_atual = 0;
	uint8_t lotacao_atual_percentagem = 0;
	uint8_t n_slaves = 2;

	uint8_t id_slave_sistema[2] = { 0x01, 0x02 };
	uint8_t id_slave_alive[2] = { 0x99, 0x99 };
	char valor_contador_slave[2] = { 0, 0 };

	/* Inicialização */

	init_io();
	init_RS485();
	init_timer_T1();
	init_interrupt();

	/* Modo configuração */

	if(!(PINB & (1 << Conf_buttom))) /* Se botao pressionado */
	 {
		 configuration_mode();
	 }

	/* Rotina de checkar os slaves */

		for (aux = 0; aux < n_slaves; aux++) {

			if (0 == check_slave(id_slave_sistema[aux])) {

				id_slave_alive[aux] = id_slave_sistema[aux];

			} else {

				id_slave_alive[aux] = 0x00;

			}
			_delay_us(30);
		}


	/* Máquina de estados */
	while (1) {

		switch (state) {

		case STATE_ADDR_SEND:

			MAX485_Sending; /* Colocar pino de controlo a 1 -> MAX485 em sending mode */

			if (0x00 != id_slave_alive[cont_SM]) { /* Se o endereço for 0x00 quer dizer que o escravo correspondente não está operacional */

				send_Address(id_slave_alive[cont_SM]);/* Enviar uma trama com o endereço para o slave */

				state = STATE_CONT_RECEIVE;

			} else {

				if (cont_SM < (n_slaves - 1)) { /* Se ainda nao tivermos percorrido todos os slaves */

					cont_SM++;

				} else { /* Se ja tivermos percorridos todos os slaves, recomeçar do 1º */

					cont_SM = 0;
				}
			}
			break;

		case STATE_CONT_RECEIVE:

			MAX485_Receiving; /* Colocar pino de controlo a 0 -> MAX485 em receiving mode */

			byte = RS485_receiveByte(); /* Guarda na variavel byte o que receber do slave para depois comparar */

			if (watchdog_timeout == byte) {

				id_slave_alive[cont_SM] = 0x00; /* Considerar que o slave não está operacional */

				state = STATE_ADDR_SEND;
				break;

			} else {

				valor_contador_slave[cont_SM] = byte; /* Se o slave estiver operacional, atualizo o seu contador */

				state = STATE_CALC_SEND;
				break;

			}
		case STATE_CALC_SEND:

			lotacao_atual = 0; /* Reiniciar a lotacao_atual para nao tender para infinito */

			for (aux = 0; aux <= (n_slaves - 1); aux++) {

				lotacao_atual = lotacao_atual + valor_contador_slave[aux]; /* atualizar a lotacao */
			}

			lotacao_atual_percentagem = lotacao_atual * 100 / lotacao_MAX;

			_delay_us(30);

			MAX485_Sending; /* Colocar pino de controlo a 1 -> MAX485 em sending mode */

			if (lotacao_atual >= lotacao_MAX) { /* Lotação superior à lotação MAX */

				if ( watchdog_timeout == send_Lotacao(1)) { /*Slave ficou nao operacional */

					id_slave_alive[cont_SM] = 0x00; /* Eliminar o slave da lista de slaves operacionais */

					state = STATE_ADDR_SEND;
					break;

				} else {

					/* Atualizar o semaforo do parque */
					LED_Vermelho_ON;
					LED_Amarelo_OFF;
					LED_Verde_OFF;

					state = STATE_ADDR_SEND;

					if (cont_SM < (n_slaves - 1)) { /* Se ainda nao tivermos percorrido todos os slaves */

						cont_SM++;

					} else {

						cont_SM = 0;  /* Se ja tivermos percorridos todos os slaves, recomeçar do 1º */
					}

					_delay_us(200);
					break;
				}

			} else if ((100 > lotacao_atual_percentagem)
					&& (lotacao_atual_percentagem >= 81)) {

				if ( watchdog_timeout == send_Lotacao(0)) {   /*Slave ficou nao operacional */

					id_slave_alive[cont_SM] = 0x00;	/* Eliminar o slave da lista de slaves operacionais */
					state = STATE_ADDR_SEND;
					break;

				} else {

					/* Atualizar o semaforo do parque */
					LED_Vermelho_OFF;

					LED_Amarelo_ON;

					LED_Verde_OFF;

					state = STATE_ADDR_SEND;

					if (cont_SM < (n_slaves - 1)) { /* Se ainda nao tivermos percorrido todos os slaves */

						cont_SM++;

					} else {

						cont_SM = 0;  /* Se ja tivermos percorridos todos os slaves, recomeçar do 1º */
					}


					_delay_us(200);
					break;
				}

			} else {

				if ( watchdog_timeout == send_Lotacao(0)) {		/*Slave ficou nao operacional */

					id_slave_alive[cont_SM] = 0x00; /* Eliminar o slave da lista de slaves operacionais */

					state = STATE_ADDR_SEND;
					break;

				} else {
					/* Atualizar o semaforo do parque */
					LED_Vermelho_OFF;

					LED_Amarelo_OFF;

					LED_Verde_ON;

					state = STATE_ADDR_SEND;

					if (cont_SM < (n_slaves - 1)) { /* Se ainda nao tivermos percorrido todos os slaves */

						cont_SM++;

					} else {

						cont_SM = 0;  /* Se ja tivermos percorridos todos os slaves, recomeçar do 1º */
					}

					_delay_us(200);
					break;
				}
			}


		default:

			state = STATE_ADDR_SEND;
			break;

		}
	}
}

