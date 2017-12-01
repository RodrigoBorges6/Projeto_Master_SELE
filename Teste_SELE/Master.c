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
/*
 ISR (TIMER1_OVF_vect) {

 TCNT1 = 0xC2F7; 	// renicia o timer com 49911

 if(1 == flag_Vermelho_1)
 {
 cont_Vermelho_1++;
 }
 if(1 == flag_Vermelho_2)
 {
 cont_Vermelho_2++;
 }
 if(1 == flag_Verde_1)
 {
 cont_Verde_1++;
 }
 if(1 == flag_Verde_2)
 {
 cont_Verde_2++;
 }

 if(cont_tempo >= 180)
 {
 //escrever na eeprom
 }
 }
 */
int main(void) {
	uint8_t aux = 0; /*variavel dos ciclos for */
	uint8_t state = 0;
	uint8_t cont_SM = 0; /* contador incremental para percorrer os slaves */
	uint8_t check = 0;

	uint8_t lotacao_MAX = 6;
	uint8_t lotacao_atual = 0;
	uint8_t lotacao_atual_percentagem = 0;
	uint8_t n_slaves = 2; /* número de slaves */

	//uint8_t id_slave_sistema[2] = {0x01,0x02};
	uint8_t id_slave_alive[2] = { 0x01, 0x00 }; //{0x00,0x00}; ALTERAR ISTOOOOOOOOOO DEPOIS DE CHEKAR QUE FUNCIONA
	uint8_t valor_contador_slave[2] = { 0, 0 };

	/* Inicialização */

	init_io();
	init_RS485();
	/*init_timer_T1();
	 init_interrupt();*/

	/* Modo configuração */
/*

	if(!(PINB & (1 << Conf_buttom)))
	{
		LED_Vermelho_ON;
		LED_Amarelo_ON;
		while(1);

	}
*/
	/* Rotina de checkar os slaves
	 for( aux = 0 ; aux < n_slaves ; aux++)
	 {
	 if(0 == check_slave(id_slave_sistema[aux]))
	 {
	 id_slave_alive[aux] = id_slave_sistema[aux];
	 }
	 else
	 {
	 id_slave_alive[aux] = 0x00;
	 }
	 }
	 */
	/* Máquina de estados */
	while (1) {

		switch (state){

			case STATE_ADDR_SEND:

				MAX485_Sending;

				if (0x00 != id_slave_alive[cont_SM]) {
					check = send_Address(id_slave_alive[cont_SM]);
					state = STATE_CONT_RECEIVE;
				} else {
					if (cont_SM < (n_slaves - 1)) {
						cont_SM++;
					} else {
						cont_SM = 0;
					}
				}
				break;

			case STATE_CONT_RECEIVE:

				if(0 == check){
					MAX485_Receiving;
					valor_contador_slave[cont_SM] = RS485_receiveByte();
					state = STATE_CALC_SEND;
				}
				break;

			case STATE_CALC_SEND:

				lotacao_atual = 0;

				for (aux = 0; aux < (n_slaves - 1); aux++) {
					lotacao_atual = lotacao_atual + valor_contador_slave[aux];
				}

				lotacao_atual_percentagem = lotacao_atual * 100 / lotacao_MAX;

				_delay_us(30);
				MAX485_Sending;
				if (lotacao_atual >= lotacao_MAX) {
					//flag_Verde = 0;
					//flag_Vermelho = 1;
					check = send_Lotacao(1); /* Lotação superior à lotação MAX */
					/*semaforo vermelho */
					LED_Vermelho_ON;
					LED_Amarelo_OFF;
					LED_Verde_OFF;

				} else if ((100 > lotacao_atual_percentagem) && (lotacao_atual_percentagem >= 81)) {
					check = send_Lotacao(0);
					//flag_Verde = 1;
					//flag_Vermelho = 0;
					/* semaforo amarelo */
					LED_Vermelho_OFF;
					LED_Amarelo_ON;
					LED_Verde_OFF;
				} else {
					check = send_Lotacao(0);/* verificar o check */
					//flag_Verde = 1;
					//flag_Vermelho = 0;
					/* semaforo verde */
					LED_Vermelho_OFF;
					LED_Amarelo_OFF;
					LED_Verde_ON;
				}

				if (cont_SM < (n_slaves - 1)) {
					cont_SM++;
				} else {
					cont_SM = 0;
				}
				state = STATE_ADDR_SEND;
				_delay_us(200);
				break;

			default:

				state = STATE_ADDR_SEND;
				break;
		}
	}


/*
	while (1) {
		if (0 == state) {
			MAX485_Sending;

			if (0x00 != id_slave_alive[cont_SM]) {
				check = send_Address(id_slave_alive[cont_SM]);
				state = 1;
			} else {
				if (cont_SM < n_slaves) {
					cont_SM++;
				} else {
					cont_SM = 0;
				}
			}

		} else if ((0 == check) && (1 == state)) {

			MAX485_Receiving;
			valor_contador_slave[id_slave_alive[cont_SM]] = RS485_receiveByte();
			state = 2;
		} else if (2 == state) {

			lotacao_atual = 0;

			for (aux = 0; aux < n_slaves; aux++) {
				lotacao_atual = lotacao_atual + valor_contador_slave[aux];
			}

			lotacao_atual_percentagem = lotacao_atual * 100 / lotacao_MAX;

			MAX485_Sending;

			 _delay_us(5);
			if (lotacao_atual >= lotacao_MAX) {
				//flag_Verde = 0;
				//flag_Vermelho = 1;
				check = send_Lotacao(1);  Lotação superior à lotação MAX
				semaforo vermelho
				LED_Vermelho_ON
				;
				LED_Amarelo_OFF
				;
				LED_Verde_OFF
				;

			} else if (100 > lotacao_atual_percentagem
					&& lotacao_atual_percentagem >= 81) {
				check = send_Lotacao(0);
				//flag_Verde = 1;
				//flag_Vermelho = 0;
				 semaforo amarelo
				LED_Vermelho_OFF
				;
				LED_Amarelo_ON
				;
				LED_Verde_OFF
				;
			} else {
				check = send_Lotacao(0); verificar o check
				//flag_Verde = 1;
				//flag_Vermelho = 0;
				 semaforo verde
				LED_Vermelho_OFF
				;
				LED_Amarelo_OFF
				;
				LED_Verde_ON
				;
			}

			if (cont_SM < n_slaves) {
				cont_SM++;
			} else {
				cont_SM = 0;
			}
			state = 0;
		}

	}
*/

}




