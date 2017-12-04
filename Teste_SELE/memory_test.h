/*
 *
 *  Created on: 03/12/2017
 *      Author: pedrorodrigues
 */


#ifndef MEMORY_TEST_H_
#define MEMORY_TEST_H_


#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>


#define ONES 0xFF
#define ZEROS 0x00
#define UP 1
#define DOWN 0
#define EITHER 2
#define SIGNATURE_FLASH 12345

uint8_t teste_FLASH(void);

uint8_t MarchC_minus(uint8_t* addr_base, uint16_t n);

void writeZero(uint8_t addr_direction, uint8_t* addr_base, uint16_t n);

uint8_t readZero(uint8_t addr_direction, uint8_t* addr_base, uint16_t n);

uint8_t readZero_writeOne(uint8_t addr_direction, uint8_t* addr_base, uint16_t n);

uint8_t readOne_writeZero(uint8_t addr_direction, uint8_t* addr_base, uint16_t n);


#endif /* MEMORY_TEST_H_ */
