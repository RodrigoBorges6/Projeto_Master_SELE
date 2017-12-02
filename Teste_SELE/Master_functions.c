/*
 * Master_functions.c

 *
 *  Created on: 26/11/2017
 *      Author: pedrorodrigues
 */

#include "Master_functions.h"


volatile uint16_t watchdog = 0; /* contador do watchdog */
volatile uint8_t watchdog_flag = 0; /* flag que indica se o watchdog ativou */


uint8_t EEMEM num_of_slaves, address_of_slaves[3]; /* Variaveis da EEPROM */


void init_io(void) {

	/* Declarar LEDs como entradas */
	DDRB |= (1 << LED_Vermelho);
	DDRB |= (1 << LED_Amarelo);
	DDRB |= (1 << LED_Verde);

	DDRB &= ~(1 << Conf_buttom); /* declarar botao como uma entrada */
	PORTB |= (1 << Conf_buttom); /*pull up */
}

void init_RS485(void) {

	/* Set baud rate */
	UBRR0H = (uint8_t) (baudgen >> 8); /*higher part of baudrate value */
	UBRR0L = (uint8_t) baudgen; /*lower part of baudrate value */

	/* receiver and transmitter enable pagina 193 */
	UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << UCSZ02);

	/* Set frame format pagina 194: 9data, 1stop bit sem paridade*/
	UCSR0C = (7 << UCSZ00)/*(1<<USBS0)seria para 2 stop bits */
	| (0 << UPM00) /* no parity */
	| (0 << USBS0) /* 1 stop bit */
	| (0 << UMSEL00) | (0 << UMSEL01);/* comunicacao assincrona */

	DDRB |= (1 << controlo_MAX485);/* Definir pino de controlo como saída PB2 */
}

void init_timer_T1(void) {

	/*este timer conta mili segundos, escala = 1ms */

	TIMSK1 |= (1 << OCIE1A); /* interrupçao quando atinge OCR1A */

	TCCR1B = 0;		/*parar TC1 */
	TCCR1A = 0;		/*modo normal */

	TCCR1B |= (1 << CS10); /* sem pre divisao */

	OCR1A = 16000; /* valor onde se dá a interrupt */

}

void init_interrupt(void) {

	sei(); /* Ativar interrupts */

}

void configuration_mode(void) {

	char str[10], *ptr;
	uint8_t mode = 0, aux_num_slaves = 0;
	uint8_t aux_slave_address = 0;


	LED_Vermelho_ON
	;
	LED_Amarelo_ON
	;
	LED_Verde_ON
	;

	write_string("\r\nMODO DE PROGRAMACAO\r\n");
	write_string("Escolher modo de operação:\r\n"
			"1 - Modo de configuração de slaves\r\n"
			"2 - Configuração atual\r\n"
			"3 - Informações Master\r\n");

	do {
		write_string("\r\n -> Modo: ");

		while (!(read_string(str)))
			;
		mode = strtol(str, &ptr, 10);

		if ((mode != 1) && (mode != 2) && (mode != 3)) {
			write_string("Modo de configuração inválido!\r\n");
		}
	} while ((mode != 1) && (mode != 2) && (mode != 3));

	switch (mode) {

	case 1: /*Modo de configuração de slaves*/

		write_string(
				"\r\nNúmero de slaves pretendidos (mínimo de 1 e máximo de 3): ");

		do {
			while (!(read_string(str)))
				;
			aux_num_slaves = strtol(str, &ptr, 10);

			if ((aux_num_slaves != 1) && (aux_num_slaves != 2) && (aux_num_slaves != 3)) {
				write_string("\r\nNúmero de slaves inválido! ");
			}
		} while ((aux_num_slaves != 1) && (aux_num_slaves != 2) && (aux_num_slaves != 3));

		//eeprom_update_byte(&num_of_slaves, aux_num_slaves);

		switch (aux_num_slaves) {

		case 1:

			write_string("\r\nEndereço do slave (valor entre 1 e 256): ");

			do {
				while (!(read_string(str)))
					;
				aux_slave_address = strtol(str, &ptr, 10);

				if (!((aux_slave_address >= 1) && (aux_slave_address <= 256))) {
					write_string("\r\nEndereço inválido! ");
				}

			} while (!((aux_slave_address >= 1) && (aux_slave_address <= 256)));

			//eeprom_update_byte(&address_of_slaves[0], aux_slave_address);
			//eeprom_update_byte(&address_of_slaves[1], 0x00);
			//eeprom_update_byte(&address_of_slaves[2], 0x00);
			write_string("\r\n TESTE 1\r\n");
			break;

		case 2:

			write_string(
					"\r\nEndereço do primeiro slave (valor entre 1 e 256): ");

			do {
				while (!(read_string(str)))
					;
				aux_slave_address = strtol(str, &ptr, 10);

				if (!((aux_slave_address >= 1) && (aux_slave_address <= 256))) {
					write_string("\r\nEndereço inválido! ");
				}

			} while (!((aux_slave_address >= 1) && (aux_slave_address <= 256)));

			//eeprom_update_byte(&address_of_slaves[0], aux_slave_address);

			write_string(
					"\r\nEndereço do segundo slave (valor entre 1 e 256): ");

			do {
				while (!(read_string(str)))
					;
				aux_slave_address = strtol(str, &ptr, 10);

				if (!((aux_slave_address >= 1) && (aux_slave_address <= 256))) {
					write_string("\r\nEndereço inválido! ");
				}

			} while (!((aux_slave_address >= 1) && (aux_slave_address <= 256)));

			//eeprom_update_byte(&address_of_slaves[1], aux_slave_address);
			//eeprom_update_byte(&address_of_slaves[2], 0x00);
			write_string("\r\n TESTE 2\r\n");
			break;

		case 3:

			write_string(
					"\r\nEndereço do primeiro slave (valor entre 1 e 256): ");

			do {
				while (!(read_string(str)))
					;
				aux_slave_address = strtol(str, &ptr, 10);

				if (!((aux_slave_address >= 1) && (aux_slave_address <= 256))) {
					write_string("\r\nEndereço inválido! ");
				}

			} while (!((aux_slave_address >= 1) && (aux_slave_address <= 256)));

			//eeprom_update_byte(&address_of_slaves[0], aux_slave_address);

			write_string(
					"\r\nEndereço do segundo slave (valor entre 1 e 256): ");

			do {
				while (!(read_string(str)))
					;
				aux_slave_address = strtol(str, &ptr, 10);

				if (!((aux_slave_address >= 1) && (aux_slave_address <= 256))) {
					write_string("\r\nEndereço inválido! ");
				}

			} while (!((aux_slave_address >= 1) && (aux_slave_address <= 256)));

			//eeprom_update_byte(&address_of_slaves[1], aux_slave_address);

			write_string(
					"\r\nEndereço do terceiro slave (valor entre 1 e 256): ");

			do {
				while (!(read_string(str)))
					;
				aux_slave_address = strtol(str, &ptr, 10);

				if (!((aux_slave_address >= 1) && (aux_slave_address <= 256))) {
					write_string("\r\nEndereço inválido! ");
				}

			} while (!((aux_slave_address >= 1) && (aux_slave_address <= 256)));

			//eeprom_update_byte(&address_of_slaves[2], aux_slave_address);
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
}

ISR (TIMER1_COMPA_vect) { /* interrupt a cada 1ms */

	TCNT1 = 0; /* reiniciar o timer */
	watchdog++; /* incrementar o contador */

	if (watchdog >= 1000) { /* Se ja contou 1000 tickets ou seja 1 segundo */

		watchdog_flag = 1; /* ativar watchdog */

		return;

	} else {

		return;

	}
}

void reset_watchdog(void) {

	TCNT1 = 0; /* Reiniciar timer */
	watchdog = 0; /* Reiniciar contador */
	watchdog_flag = 0; /* Reiniciar flag do watchdog */

	return;
}

void RS485_sendByte(uint8_t temp) {

	while ((UCSR0A & (1 << UDRE0)) == 0); /* Wait for empty transmit buffer */

	UDR0 = temp; /* Put data into buffer, sends the data */

	UCSR0A |= (1 << TXC0);

	while ((UCSR0A & (1 << TXC0)) == 0);/* espera até ter enviado o byte */
}

char RS485_receiveByte(void) {

	reset_watchdog();	/* Reiniciar watchdog para o usar */

	while ((UCSR0A & (1 << RXC0)) == 0) {	/* Wait for data to be received */

		if (1 == watchdog_flag) {	/* Se o watchdog tiver ativado, considerar que o slave nao esta operacional */

			return watchdog_timeout;

		}
		/* Else não necessário */
	}

	return UDR0;/* Get and return received data from buffer */

}

uint8_t check_slave(uint8_t n_slave) {

	MAX485_Sending; /* Colocar pino de controlo a 1 -> MAX485 em sending mode */

	send_Address(n_slave); /* Enviar o address ao slave */

	MAX485_Receiving;	/* Colocar pino de controlo a 0 -> MAX485 em receiving mode */

	if (n_slave == RS485_receiveByte()) {	/* Se o slave retornou o seu endereço */

		return 0;

	} else {

		return watchdog_timeout;	/* Se nao seguir o protocolo, considerar o slave nao operacional */

	}

}

void send_Address(uint8_t n_slave) {

	UCSR0B = UCSR0B | (1 << TXB80); /* Ativar o 9º, ou sejaTrama de endereços */

	RS485_sendByte(n_slave); /* Enviar o endereço do slave */

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
}
