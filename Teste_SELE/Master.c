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

/* TImers para os LEDS dos slaves (os 4) + modo manutençao com botao + reset que envia por printlns a info dos timers (nao esquecer de meter o RS485 em modo recepçao)
 + diodo RX + ciclo de teste dos slaves */

#include "Master_functions.h"

int main(void) {

	uint8_t aux = 0; /*variavel dos ciclos for */
	uint8_t state = 0;
	uint8_t cont_SM = 0; /* contador incremental para percorrer os slaves */

	uint8_t lotacao_MAX = 6;
	char lotacao_atual = 0;
	uint8_t lotacao_atual_percentagem = 0;
	uint8_t n_slaves = 2; /* número de slaves */

	uint8_t id_slave_sistema[2] = { 0x01, 0x02 };
	uint8_t id_slave_alive[2] = { 0x99, 0x99 };
	char valor_contador_slave[2] = { 0, 0 };
	char byte = 0;

	/* Inicialização */

	init_io();
	init_RS485();
	init_timer_T1();
	init_interrupt();

	/* Modo configuração */

	 if(!(PINB & (1 << Conf_buttom)))
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
		//if((id_slave_alive[0] == 0x01 )&& (id_slave_alive[1] == 0x02)) LED_Verde_ON;
		_delay_us(30);
	}

	//return 0;
	/* Máquina de estados */
	while (1) {

		switch (state) {

		case STATE_ADDR_SEND:
			TCNT1 = 0xC2F7;
			MAX485_Sending; /* Colocar pino de controlo a 1 -> MAX485 em sending mode */

			if (0x00 != id_slave_alive[cont_SM]) {

				send_Address(id_slave_alive[cont_SM]);

				state = STATE_CONT_RECEIVE;

			} else {

				//LED_Amarelo_ON;

				if (cont_SM < (n_slaves - 1)) {

					cont_SM++;

				} else {

					cont_SM = 0;
				}
			}
			break;

		case STATE_CONT_RECEIVE:

			MAX485_Receiving
			/* Colocar pino de controlo a 0 -> MAX485 em receiving mode */
			;

			byte = RS485_receiveByte();

			if (watchdog_timeout == byte) {

				id_slave_alive[cont_SM] = 0x00;

				state = STATE_ADDR_SEND;
				break;

			} else {

				valor_contador_slave[cont_SM] = byte;

				state = STATE_CALC_SEND;
				break;

			}
		case STATE_CALC_SEND:

			lotacao_atual = 0;

			for (aux = 0; aux <= (n_slaves - 1); aux++) {

				lotacao_atual = lotacao_atual + valor_contador_slave[aux];
			}

			lotacao_atual_percentagem = lotacao_atual * 100 / lotacao_MAX;

			_delay_us(30);

			MAX485_Sending;

			if (lotacao_atual >= lotacao_MAX) { /* Lotação superior à lotação MAX */

				if ( watchdog_timeout == send_Lotacao(1)) {

					id_slave_alive[cont_SM] = 0x00;

					state = STATE_ADDR_SEND;
					break;

				} else {

					LED_Vermelho_ON
					;
					LED_Amarelo_OFF
					;
					LED_Verde_OFF
					;

					state = STATE_ADDR_SEND;

					if (cont_SM < (n_slaves - 1)) {

						cont_SM++;

					} else {

						cont_SM = 0;
					}

					_delay_us(200);
					break;
				}

			} else if ((100 > lotacao_atual_percentagem)
					&& (lotacao_atual_percentagem >= 81)) {

				if ( watchdog_timeout == send_Lotacao(0)) {

					id_slave_alive[cont_SM] = 0x00;
					state = STATE_ADDR_SEND;
					break;

				} else {

					LED_Vermelho_OFF
					;
					LED_Amarelo_ON
					;
					LED_Verde_OFF
					;

					state = STATE_ADDR_SEND;

					if (cont_SM < (n_slaves - 1)) {

						cont_SM++;

					} else {

						cont_SM = 0;
					}

					_delay_us(200);
					break;
				}

			} else {

				if ( watchdog_timeout == send_Lotacao(0)) {

					id_slave_alive[cont_SM] = 0x00;
					LED_Vermelho_ON;
					state = STATE_ADDR_SEND;
					break;

				} else {
					LED_Vermelho_OFF
					;
					LED_Amarelo_OFF
					;
					LED_Verde_ON
					;

					state = STATE_ADDR_SEND;

					if (cont_SM < (n_slaves - 1)) {

						cont_SM++;

					} else {

						cont_SM = 0;
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

