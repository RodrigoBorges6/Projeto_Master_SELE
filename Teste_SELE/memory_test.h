/*
 *		memory_test.h
 *
 *      Author: pedrorodrigues
 */


#ifndef MEMORY_TEST_H_
#define MEMORY_TEST_H_


#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>


#define ONES 0xFF
#define ZEROS 0x00

#define SIGNATURE_FLASH 12344

/*******************************************************************************************************************************
 * Testes de memória ClassB
 * Adaptação da biblioteca da AVR
 ***************************************************************************************************************************/

#define INTERNAL_SRAM_SIZE 2048
#define INTERNAL_SRAM_START 0x0100
#define NSECS 8 /* Podia ser 2, 4, 8, 16 etc */
#define SEC_SIZE (INTERNAL_SRAM_SIZE / NSECS) /*internal The size of each segment in bytes */

/*Testar a memória RAM*/
uint8_t sram_test(void);

/*Executar o código que realiza o MARCH C Minus test*/
uint8_t marchC_minus(register volatile uint8_t * p_sram, register volatile uint8_t * p_buffer, register uint16_t size);

/*Teste à memória flash*/
uint8_t flash_test(void);

#endif /* MEMORY_TEST_H_ */
