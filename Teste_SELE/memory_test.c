/*
 *		memory_test.c
 *
 *      Author: pedrorodrigues
 */


#include "memory_test.h"

#include "RS485.h"

#define SRAM_ERROR 0

uint16_t EEMEM hash_flash, assinatura;

static uint8_t buffer[SEC_SIZE] __attribute__ ((section (".sram_buffer")));

uint8_t flash_test(void){

	uint16_t hash = 0;
	uint16_t i = 0;
	uint16_t n_words = 256 * 64;

	for(i = 0; i < n_words; i++){

		hash ^= pgm_read_word_near(i);
	}

	if(SIGNATURE_FLASH != eeprom_read_word(&assinatura)){ /* Quando o programa estiver pronto, alterar SIGNATURE_FLASH  */

	 eeprom_update_word(&hash_flash, hash);
	 eeprom_update_word(&assinatura, SIGNATURE_FLASH);
	}

	if(hash == eeprom_read_word(&hash_flash)){

		return 0;
	}
	else{

		return 1;
	}

	return 0;
}

uint8_t sram_test(){

	static uint8_t current_section = 0;

	for(current_section = 0; current_section < NSECS; current_section++) {

		if (1 == (marchC_minus( (uint8_t *) INTERNAL_SRAM_START + current_section * SEC_SIZE, buffer, SEC_SIZE))){

			return 1;

		}
	}

	return 0;
}

uint8_t marchC_minus(register volatile uint8_t * p_sram, register volatile uint8_t * p_buffer, register uint16_t size) {


	/*
	 * p_sram -> pointer to the start of the sram memory
	 * p_buffer -> pointer to the start of the buffer
	 */

	/* Todas as variaveis sao registers para nao serem escritas na RAM para nao interferir no teste */
	register uint16_t i = 0;
	register uint8_t erro = 0;

	/* Save content of the section: copy to buffer unless we test the buffer */
	if (p_buffer != p_sram){

		for (uint16_t i = 0; i < size; i++){

			*(p_buffer + i) = *(p_sram + i);

		}
	}



	/*
	 * Implemetação do MARCH C-
	 */

	/* Write ZEROS UP */

	for (i = 0; i < size; i++) {

		*(p_sram + i) = ZEROS;

		/* induzir erro */
	#if SRAM_ERROR

		if((p_sram + i) == (uint8_t *) 0x0800){

			*(p_sram+i) = 0xff;

		}
	#endif

	}

	/* read ZEROS, write ONES UP */

	for (i = 0; i < size; i++) {

		if (ZEROS != *(p_sram + i)){

			erro = 1;

		}

		*(p_sram + i) = ONES;

	}

	/* read ONES, write ZEROS UP */

	for (i = 0; i < size; i++) {

		if (ONES != *(p_sram + i)){

			erro = 1;

		}

		*(p_sram + i) = ZEROS;

	}

	/* read ZEROS, write ONES DOWN */
	for (i = size; i > 0; i--) {

		if (ZEROS != *(p_sram + i - 1)){

			erro = 1;

		}

		*(p_sram + i - 1) = ONES;

	}

	/*  read ONES, write ZEROS DOWN */
	for (i = size; i > 0; i--) {

		if (ONES != *(p_sram + i - 1)){

			erro = 1;

		}

		*(p_sram + i - 1) = ZEROS;

	}

	/* read ZEROS UP */
	for (i = 0; i < size; i++) {

		if (ZEROS != *(p_sram + i)){

			erro = 1;

		}

	}

	/* Restore content of the section: copy from buffer, unless buffer is tested */
	if (p_buffer != p_sram){

		for (i = 0; i < size; i++){

			*(p_sram + i) = *(p_buffer + i);

		}

	}

	if(erro == 0) {

		return 0;

	}

	else{

		return 1;

	}

	return 0;
}
