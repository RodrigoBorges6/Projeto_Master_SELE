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

uint8_t EEMEM num_of_slaves;
uint8_t EEMEM address_of_slaves[3];

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

	char str[10], *ptr;
	int mode = 0, num_slaves = 0;
	uint8_t slave_address;

	/* Inicialização */

	init_io();
	init_RS485();
	/*init_timer_T1();
	 init_interrupt();*/

	/* Modo configuração */

	if(!(PINB & (1 << Conf_buttom)))
	{
		LED_Vermelho_ON;
		LED_Amarelo_ON;
		LED_Verde_ON;

		write_string("\r\nMODO DE PROGRAMACAO\r\n");
		write_string("Escolher modo de operação:\r\n"
					"1 - Modo de configuração de slaves\r\n"
					"2 - Configuração atual\r\n"
					"3 - Informações Master\r\n");

		do {
				write_string("\r\n -> Modo: ");

				while (!(read_string(str)));
				mode = strtol(str, &ptr, 10);

				if((mode != 1) && (mode != 2) && (mode != 3)){
					write_string("Modo de configuração inválido!\r\n");
				}
			} while ((mode != 1) && (mode != 2) && (mode != 3));

		switch(mode){

			case 1: /*Modo de configuração de slaves*/

				write_string("\r\nNúmero de slaves pretendidos (mínimo de 1 e máximo de 3): ");

				do {
						while (!(read_string(str)));
						num_slaves = strtol(str, &ptr, 10);

						if((num_slaves != 1) && (num_slaves != 2) && (num_slaves != 3)){
							write_string("\r\nNúmero de slaves inválido! ");
						}
					} while ((num_slaves != 1) && (num_slaves != 2) && (num_slaves != 3));

				//eeprom_update_byte(&num_of_slaves, num_slaves);

				switch(num_slaves){

					case 1:

						write_string("\r\nEndereço do slave (valor entre 1 e 256): ");

						do {
								while (!(read_string(str)));
								slave_address = strtol(str, &ptr, 10);

								if(!((slave_address >= 1) && (slave_address <= 256))){
									write_string("\r\nEndereço inválido! ");
								}

							} while (!((slave_address >= 1) && (slave_address <= 256)));

						//eeprom_update_byte(&address_of_slaves[0], slave_address);
						//eeprom_update_byte(&address_of_slaves[1], 0x00);
						//eeprom_update_byte(&address_of_slaves[2], 0x00);
						write_string("\r\n TESTE 1\r\n");
						break;

					case 2:

						write_string("\r\nEndereço do primeiro slave (valor entre 1 e 256): ");

						do {
								while (!(read_string(str)));
								slave_address = strtol(str, &ptr, 10);

								if(!((slave_address >= 1) && (slave_address <= 256))){
									write_string("\r\nEndereço inválido! ");
								}

							} while (!((slave_address >= 1) && (slave_address <= 256)));

						//eeprom_update_byte(&address_of_slaves[0], slave_address);

						write_string("\r\nEndereço do segundo slave (valor entre 1 e 256): ");

						do {
								while (!(read_string(str)));
								slave_address = strtol(str, &ptr, 10);

								if(!((slave_address >= 1) && (slave_address <= 256))){
									write_string("\r\nEndereço inválido! ");
								}

							} while (!((slave_address >= 1) && (slave_address <= 256)));

						//eeprom_update_byte(&address_of_slaves[1], slave_address);
						//eeprom_update_byte(&address_of_slaves[2], 0x00);
						write_string("\r\n TESTE 2\r\n");
						break;

					case 3:

						write_string("\r\nEndereço do primeiro slave (valor entre 1 e 256): ");

						do {
								while (!(read_string(str)));
								slave_address = strtol(str, &ptr, 10);

								if(!((slave_address >= 1) && (slave_address <= 256))){
									write_string("\r\nEndereço inválido! ");
								}

							} while (!((slave_address >= 1) && (slave_address <= 256)));

						//eeprom_update_byte(&address_of_slaves[0], slave_address);

						write_string("\r\nEndereço do segundo slave (valor entre 1 e 256): ");

						do {
								while (!(read_string(str)));
								slave_address = strtol(str, &ptr, 10);

								if(!((slave_address >= 1) && (slave_address <= 256))){
									write_string("\r\nEndereço inválido! ");
								}

							} while (!((slave_address >= 1) && (slave_address <= 256)));

						//eeprom_update_byte(&address_of_slaves[1], slave_address);

						write_string("\r\nEndereço do terceiro slave (valor entre 1 e 256): ");

						do {
								while (!(read_string(str)));
								slave_address = strtol(str, &ptr, 10);

								if(!((slave_address >= 1) && (slave_address <= 256))){
									write_string("\r\nEndereço inválido! ");
								}

							} while (!((slave_address >= 1) && (slave_address <= 256)));

						//eeprom_update_byte(&address_of_slaves[2], slave_address);
						write_string("\r\n TESTE 3\r\n");
						break;

					default:

						write_string("\r\nERRO ERRO ERRO!!!!!!!\r\n");
						break;

				}
				break;

				case 2: /*Configuração atual*/

					write_string("\r\n******Configuração Atual******\r\n");
					write_string("Número de slaves: 3\r\n");
					write_string("Endereço do primeiro slave: \r\n");
					write_string("Endereço do segundo slave: \r\n");
					write_string("Endereço do terceiro slave: \r\n");
					break;

				case 3: /*Informações Master*/

					write_string("\r\n******Informações Master******\r\n");
					write_string("Serial Number: 123456789\r\n");
					write_string("Data de fabrico: 2 de Dezembro de 2017\r\n");
					write_string("Local de fabrico: DEEC @ FEUP\r\n");
					write_string("Developers: Pedro Rodrigues & Rodrigo Borges\r\n");
					break;

				default:
					write_string("\r\n ERRO ERRO ERRO ERRO \r\n");
					break;

		}
		while(1);

	}

	write_string("\r\n TESTE TESTE\r\n");

	write_string("MODO NORMAL\r\n");




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




