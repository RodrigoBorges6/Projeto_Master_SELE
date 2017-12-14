/*
 * Master.c
 *
 *  Created on: 24/11/2017
 *      Author: PedroRodrigues + RodrigoBorges
 */

#include "RS485.h"
#include "memory_test.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/eeprom.h>


#define LED_Vermelho PB3
#define LED_Vermelho_OFF PORTB |= (1 << LED_Vermelho);
#define LED_Vermelho_ON PORTB &= ~(1 << LED_Vermelho);
#define LED_Amarelo PB4
#define LED_Amarelo_OFF PORTB |= (1 << LED_Amarelo);
#define LED_Amarelo_ON PORTB &= ~(1 << LED_Amarelo);
#define LED_Verde PB5
#define LED_Verde_OFF PORTB |= (1 << LED_Verde);
#define LED_Verde_ON PORTB &= ~(1 << LED_Verde);


#define STATE_ADDR_SEND 0
#define STATE_CONT_RECEIVE 1
#define STATE_CALC_SEND 2


uint8_t EEMEM max_capacity, num_of_slaves, address_of_slaves[3]; /* Variaveis da EEPROM */


/* Declarar funcoes */

/* Init às io, RS485, timers, interrupts */
void init (void);

/* Configura as entradas / saidas */
void init_io(void);

/* Configuration mode */
void configuration_mode(void);

/*Impressão dos endereços dos escravos no modo de configuração*/
void print_address(uint8_t num_slaves);

/*Permite a impressão de valores sem usar printf*/
void print_value(uint8_t value);

/* Memory test */
void memory_test ();

/* Atualizar variaveis com a EEPROM */
void update_via_EEPROM (uint8_t *lotacao_MAX, uint8_t *n_slaves, uint8_t *id_slave_sistema, uint8_t *id_slave_alive);

/* Rotina para checkar os slaves */
void check_routine(uint8_t n_slaves, uint8_t *id_slave_sistema, uint8_t *id_slave_alive, uint8_t *cont_slaves_desligados);

/* Check de 1 slave */
uint8_t check_slave(uint8_t n_slave);

/* Maquina de estados */
void state_machine (uint8_t locacao_MAX, uint8_t n_slaves, uint8_t cont_slaves_desligados, uint8_t id_slave_sistema[2], uint8_t id_slave_alive[2]);

/* Enviar lotacao ao slave */
uint8_t send_Lotacao(uint8_t semaforo);


/* Main */

int main(void) {


	uint8_t aux = 0; /*variavel auxiliar dos ciclos for */

	uint8_t lotacao_MAX = 6;   /* Default para a variável */
	uint8_t n_slaves = 3;	/* Default para a variável */
	uint8_t cont_slaves_desligados = 0; /* contador do número de slaves not alive */

	uint8_t id_slave_sistema[255]; /* Iniciar a variável */
	uint8_t id_slave_alive[255]; /* Iniciar a variável */

	for(aux = 0; aux < 256 ; aux++){

		id_slave_sistema[aux] = 0;
		id_slave_alive[aux] = 0;
	}


	/* Inicialização */

	init();

	/* Modo configuração */

	if(0 == (PINB & (1 << Conf_buttom))){ /* Se botão estiver pressionado */

		configuration_mode();

	}

	/* Teste memória */

	memory_test();


	/* Atualizar variaveis com a EEPROM */

	update_via_EEPROM(&lotacao_MAX, &n_slaves, id_slave_sistema, id_slave_alive);


	/* Rotina de checkar os slaves */

	check_routine(n_slaves,id_slave_sistema, id_slave_alive, &cont_slaves_desligados);

	/* State Machine */

	state_machine (lotacao_MAX, n_slaves, cont_slaves_desligados, id_slave_sistema, id_slave_alive);


	/* end main */

	return 0;

}


/* Funcoes */

void init (void){

	init_io();
	init_RS485();
	init_timer_T1();
	init_interrupt();

	return;
}

void init_io(void) {

	/* Declarar LEDs como saídas */

	DDRB |= (1 << LED_Vermelho);
	DDRB |= (1 << LED_Amarelo);
	DDRB |= (1 << LED_Verde);

	DDRB &= ~(1 << Conf_buttom); /* declarar botao como uma entrada */
	PORTB |= (1 << Conf_buttom); /* pull up */

	return;
}

void memory_test (){

	/* Desativar as interrupçoes */
	cli();

	/* Testar RAM */

	if( 1 == sram_test()){

		LED_Vermelho_ON;
		while(1){
			;
		}
	}

	/* Testar FLASH */

	if( 1 == flash_test()){

		LED_Amarelo_ON;
		while(1){
			;
		}
	}

	/* Reativa as interrupçoes */

	sei();

	return;

}

void configuration_mode(void) {

	char str[10], *ptr;
	uint8_t mode = 0, aux_num_slaves = 0;
	uint8_t aux_slave_address = 0, aux_max_capacity = 0;

	LED_Vermelho_ON;
	LED_Amarelo_ON;
	LED_Verde_ON;

	while(1){

		write_string("\r\nMODO DE PROGRAMACAO\r\n");
		write_string("Escolher modo de operacao:\r\n"
				"1 - Modo de configuracao de slaves\r\n"
				"2 - Modo de configuracao da lotacao do parque\r\n"
				"3 - Configuracao atual\r\n"
				"4 - Informacoes Master\r\n"
				"5 - Sair do modo de configuracao\r\n");

		do {
			write_string("\r\n -> Modo: ");

			while (0 == (read_string(str))){
				;
			}
			mode = strtol(str, &ptr, 10);

			if ((1 != mode) && (2 != mode) && (3 != mode) && (4 != mode) && (5 != mode)) {
				write_string("Modo de configuracao invalido!\r\n");
			}
		} while ((1 != mode) && (2 != mode) && (3 != mode) && (4 != mode) && (5 != mode));

		switch (mode) {

		case 1: /*Modo de configuracao de slaves*/

			write_string(
					"\r\nNumero de slaves pretendidos (minimo de 1 e maximo de 3): ");

			do {
				while (0 == (read_string(str))){
					;
				}

				aux_num_slaves = strtol(str, &ptr, 10);

				if ((1 != aux_num_slaves) && (2 != aux_num_slaves) && (3 != aux_num_slaves)) {
					write_string("\r\nNumero de slaves invalido! ");
				}
			} while ((1 != aux_num_slaves) && (2 != aux_num_slaves) && (3 != aux_num_slaves));

			eeprom_update_byte(&num_of_slaves, aux_num_slaves);

			switch (aux_num_slaves) {

			case 1:

				write_string("\r\nEndereco do slave (valor entre 1 e 256): ");

				do {
					while (0 == (read_string(str))){
						;
					}

					aux_slave_address = strtol(str, &ptr, 10);

					if (0 == ((1 <= aux_slave_address) && (256 >= aux_slave_address))) {
						write_string("\r\nEndereco invalido! ");
					}

				} while (0 == ((1 <= aux_slave_address) && (256 >= aux_slave_address)));

				eeprom_update_byte(&address_of_slaves[0], aux_slave_address);
				eeprom_update_byte(&address_of_slaves[1], 0x00);
				eeprom_update_byte(&address_of_slaves[2], 0x00);
				write_string("\r\nConfiguracao do slave concluida com sucesso!!\r\n");
				break;

			case 2:

				write_string("\r\nEndereco do primeiro slave (valor entre 1 e 256): ");

				do {
					while (0 == (read_string(str))){
						;
					}

					aux_slave_address = strtol(str, &ptr, 10);

					if (0 == ((1 <= aux_slave_address) && (256 >= aux_slave_address))) {
						write_string("\r\nEndereco invalido! ");
					}

				} while (0 == ((1 <= aux_slave_address) && (256 >= aux_slave_address)));

				eeprom_update_byte(&address_of_slaves[0], aux_slave_address);

				write_string("\r\nEndereco do segundo slave (valor entre 1 e 256): ");

				do {
					while (0 == (read_string(str))){
						;
					}

					aux_slave_address = strtol(str, &ptr, 10);

					if (0 == ((1 <= aux_slave_address) && (256 >= aux_slave_address))) {
						write_string("\r\nEndereco invalido! ");
					}

				} while (0 == ((1 <= aux_slave_address) && (256 >= aux_slave_address)));

				eeprom_update_byte(&address_of_slaves[1], aux_slave_address);
				eeprom_update_byte(&address_of_slaves[2], 0x00);
				write_string("\r\nConfiguracao dos 2 slaves concluida com sucesso!!\r\n");
				break;

			case 3:

				write_string("\r\nEndereco do primeiro slave (valor entre 1 e 256): ");

				do {
					while (0 == (read_string(str))){
						;
					}

					aux_slave_address = strtol(str, &ptr, 10);

					if (0 == ((1 <= aux_slave_address) && (256 >= aux_slave_address))) {
						write_string("\r\nEndereco invalido! ");
					}

				} while (0 == ((1 <= aux_slave_address) && (256 >= aux_slave_address)));

				eeprom_update_byte(&address_of_slaves[0], aux_slave_address);

				write_string("\r\nEndereco do segundo slave (valor entre 1 e 256): ");

				do {
					while (0 == (read_string(str))){
						;
					}

					aux_slave_address = strtol(str, &ptr, 10);

					if (0 == ((1 <= aux_slave_address) && (256 >= aux_slave_address))) {
						write_string("\r\nEndereco invalido! ");
					}

				} while (0 == ((1 <= aux_slave_address) && (256 >= aux_slave_address)));

				eeprom_update_byte(&address_of_slaves[1], aux_slave_address);

				write_string("\r\nEndereco do terceiro slave (valor entre 1 e 256): ");

				do {
					while (0 == (read_string(str))){
						;
					}

					aux_slave_address = strtol(str, &ptr, 10);

					if (0 == ((1 <= aux_slave_address) && (256 >= aux_slave_address))) {
						write_string("\r\nEndereco invalido! ");
					}

				} while (0 == ((1 <= aux_slave_address) && (256 >= aux_slave_address)));

				eeprom_update_byte(&address_of_slaves[2], aux_slave_address);
				write_string("\r\nConfiguracao dos 3 slaves concluida com sucesso!!\r\n");
				break;

			default:

				write_string("\r\nERRO ERRO ERRO!!!!!!!\r\n");
				break;

			}
			break;

			case 2: /*Modo de configuracao da lotacao do parque*/

				write_string("\r\n******Configuracao da Lotacao Maxima******\r\n");

				write_string("Valor da lotacao maxima pretendida (minimo de 0 e maximo de 200): ");

				do {
					while (0 == (read_string(str))){
						;
					}

					aux_max_capacity = strtol(str, &ptr, 10);

					if (0 == ((0 <= aux_max_capacity) && (200 >= aux_max_capacity))) {
						write_string("\r\nCapacidade invalida! ");
					}

				} while (0 == ((0 <= aux_max_capacity) && (200 >= aux_max_capacity)));

				eeprom_update_byte(&max_capacity, aux_max_capacity);

				write_string("\r\n*****Fim da configuracao de lotacao maxima***** \r\n");

				break;

			case 3: /*Configuracao atual*/

				write_string("\r\n**********Configuracao Atual**********\r\n\r\n");

				aux_max_capacity = eeprom_read_byte(&max_capacity);

				write_string("Lotacao maxima atual: ");

				print_value(aux_max_capacity);
				write_string("\r\n");

				aux_num_slaves = eeprom_read_byte(&num_of_slaves);

				write_string("Numero de slaves: ");

				print_value(aux_num_slaves);
				write_string("\r\n");

				print_address(aux_num_slaves);

				write_string("\r\n******Fim da Configuracao Atual******\r\n");
				break;

			case 4: /*Informações Master*/

				write_string("\r\n******************Informacoes Master******************\r\n");
				write_string("\r\n*                   Serial Number: 123456789                 *\r\n");
				write_string("\r\n*        Data de fabrico: 15 de Dezembro de 2017     *\r\n");
				write_string("\r\n*              Local de fabrico: DEEC @ FEUP            *\r\n");
				write_string("\r\n* Developers: Pedro Rodrigues & Rodrigo Borges *\r\n");
				write_string("\r\n**********************************************************\r\n");
				break;

			case 5:

				write_string("\r\n ************** A sair do modo de configuracao**************\r\n");
				return;
				break;

			default:
				write_string("\r\n ERRO ERRO ERRO ERRO \r\n");
				break;

		}
	}
	return;

}

void print_address(uint8_t aux_num_slaves){

	uint8_t i=0, aux_slave_address = 0;

	for(i = 0; i < aux_num_slaves; i++){

		write_string("Endereco do slave numero \"");
		print_value(i+1);
		write_string("\" : ");
		aux_slave_address = eeprom_read_byte(&address_of_slaves[i]);
		print_value(aux_slave_address);
		write_string("\r\n");

	}

	return;
}

void print_value(uint8_t value){

	uint8_t aux = 0;

	if(9 >= value){

		print_char(value + 48);
		return;

	}
	else if(9 < value && 99 >= value){

		aux = value / 10;
		print_char(aux + 48);
		aux = value % 10;
		print_char(aux + 48);
		return;

	}
	else if(99 < value){

		aux = value / 100;
		print_char(aux + 48);
		aux = (value % 100) / 10;
		print_char(aux + 48);
		aux = value % 10;
		print_char(aux + 48);
		return;

	}
	else {
		;
	}

	return;
}

void update_via_EEPROM (uint8_t *lotacao_MAX, uint8_t *n_slaves, uint8_t *id_slave_sistema, uint8_t *id_slave_alive){

	/* Atualizar variaveis com a EEPROM */

	uint8_t aux = 0; /*variavel auxiliar dos ciclos for */

	*n_slaves = eeprom_read_byte(&num_of_slaves);
	*lotacao_MAX = eeprom_read_byte(&max_capacity);

	for(aux = 0; aux < *n_slaves; aux++){

		id_slave_sistema[aux] = eeprom_read_byte(&address_of_slaves[aux]);

	}

	return;
}

void check_routine(uint8_t n_slaves, uint8_t *id_slave_sistema, uint8_t *id_slave_alive, uint8_t *cont_slaves_desligados){


	/*variavel auxiliar dos ciclos for */
	uint8_t aux = 0;

	/* Rotina de checkar os slaves */

	for (aux = 0; aux < n_slaves; aux++) {

		if (0 == check_slave(id_slave_sistema[aux])) {

			id_slave_alive[aux] = id_slave_sistema[aux];

		}
		else {

			id_slave_alive[aux] = 0x00;
			*cont_slaves_desligados = *cont_slaves_desligados + 1;
		}

		_delay_us(30);
	}

	return;
}

uint8_t check_slave(uint8_t n_slave) {

	/* Colocar pino de controlo a 1 -> MAX485 em sending mode */
	MAX485_Sending;

	/* Enviar o address ao slave */
	send_Address(n_slave);

	/* Colocar pino de controlo a 0 -> MAX485 em receiving mode */
	MAX485_Receiving;

	/* Se o slave retornou o seu endereço */
	if ( RS485_receiveByte() == n_slave){

		return 0;

	} else {

		return watchdog_timeout;	/* Se nao seguir o protocolo, considerar o slave nao operacional */

	}

	return 0;
}

void state_machine (uint8_t lotacao_MAX, uint8_t n_slaves, uint8_t cont_slaves_desligados, uint8_t *id_slave_sistema, uint8_t *id_slave_alive){

	uint8_t state = 0; /* variavel da state machine de comunicaçao */
	uint8_t aux = 0; /*variavel auxiliar dos ciclos for */
	uint8_t cont_SM = 0; /* contador incremental para percorrer os slaves, incrementado a cada ciclo da SM*/
	int8_t byte = 0; /* variavel auxiliar para receber o byte enviado pelo slave */

	int8_t lotacao_atual = 0;
	uint8_t lotacao_atual_percentagem = 0;
	int8_t valor_contador_slave[2] = { 0, 0};

	/* Máquina de estados */

	while (1) {

		switch (state) {

		case STATE_ADDR_SEND:

			MAX485_Sending; /* Colocar pino de controlo a 1 -> MAX485 em sending mode */

			if (0x00 != id_slave_alive[cont_SM]) { /* Se o endereço for 0x00 quer dizer que o escravo correspondente não está operacional */

				send_Address(id_slave_alive[cont_SM]);/* Enviar uma trama com o endereço para o slave */

				state = STATE_CONT_RECEIVE;

			} else {

				if (n_slaves == cont_slaves_desligados ) {	/* Se todos os slaves estiver nao operacionais */

					LED_Vermelho_ON;
					LED_Amarelo_ON;
					LED_Verde_OFF;
					break;

				} else {

					if (cont_SM < (n_slaves - 1)) { /* Se ainda nao tivermos percorrido todos os slaves */

						cont_SM++;

					} else { /* Se ja tivermos percorridos todos os slaves, recomeçar do 1º */

						cont_SM = 0;
					}
				}
			}
			break;

		case STATE_CONT_RECEIVE:

			MAX485_Receiving; /* Colocar pino de controlo a 0 -> MAX485 em receiving mode */

			byte = RS485_receiveByte(); /* Guarda na variavel byte o que receber do slave para depois comparar */

			if (watchdog_timeout == byte) {

				id_slave_alive[cont_SM] = 0x00; /* Considerar que o slave não está operacional */

				cont_slaves_desligados++; /* Incrementar o numero de slaves nao operacionais */

				state = STATE_ADDR_SEND;
				break;

			} else {

				valor_contador_slave[cont_SM] = byte; /* Se o slave estiver operacional, atualizo o seu contador */

				state = STATE_CALC_SEND;
				break;

			}

			break;

		case STATE_CALC_SEND:

			lotacao_atual = 0; /* Reiniciar a lotacao_atual para nao tender para infinito */

			for (aux = 0; aux <= (n_slaves - 1); aux++) {

				lotacao_atual = lotacao_atual + valor_contador_slave[aux]; /* atualizar a lotacao */
			}

			if(0 > lotacao_atual){

				lotacao_atual = 0;

			}

			lotacao_atual_percentagem = lotacao_atual * 100 / lotacao_MAX;

			_delay_us(30);

			MAX485_Sending; /* Colocar pino de controlo a 1 -> MAX485 em sending mode */

			if (lotacao_atual >= lotacao_MAX) { /* Lotação superior à lotação MAX */

				if (send_Lotacao(1) == watchdog_timeout) { /*Slave ficou nao operacional */

					id_slave_alive[cont_SM] = 0x00; /* Eliminar o slave da lista de slaves operacionais */

					cont_slaves_desligados++; /* Incrementar o numero de slaves nao operacionais */

					state = STATE_ADDR_SEND;
					break;

				}
				else {

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

			} else if ((100 > lotacao_atual_percentagem) && (81 <= lotacao_atual_percentagem)) {

				if (send_Lotacao(0) == watchdog_timeout) {   /*Slave ficou nao operacional */

					id_slave_alive[cont_SM] = 0x00;	/* Eliminar o slave da lista de slaves operacionais */

					cont_slaves_desligados++; /* Incrementar o numero de slaves nao operacionais */

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

				if (send_Lotacao(0) == watchdog_timeout) {		/*Slave ficou nao operacional */

					id_slave_alive[cont_SM] = 0x00; /* Eliminar o slave da lista de slaves operacionais */

					cont_slaves_desligados++; /* Incrementar o numero de slaves nao operacionais */

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

			break;


		default:

			state = STATE_ADDR_SEND;
			break;

		}
	}

	return;
}

uint8_t send_Lotacao(uint8_t semaforo) {

	UCSR0B = UCSR0B & ~(1 << TXB80); /* Desligar o 9º, ou seja Trama de dados */

	if (1 == semaforo) { /* Parque lotado */

		RS485_sendByte(0xFF);/* Parque ocupado - semaforo vermelho */

		MAX485_Receiving;	/* Colocar pino de controlo a 0 -> MAX485 em receiving mode */

		if (0xFF == RS485_receiveByte()) {	/* Esperar pela recepçao e comparar o que o slave enviou com o suposto */

			return 0;

		} else {

			return watchdog_timeout;	/* Se nao seguir o protocolo, considerar o slave nao operacional */

		}

	} else {

		RS485_sendByte(0xAA); /* Parque livre - semaforo verde */

		MAX485_Receiving;	/* Colocar pino de controlo a 0 -> MAX485 em receiving mode */

		if (0xAA == RS485_receiveByte()) {	/* Esperar pela recepçao e comparar o que o slave enviou com o suposto */

			return 0;

		} else {

			return watchdog_timeout;	/* Se nao seguir o protocolo, considerar o slave nao operacional */

		}

	}

	return 0;
}
