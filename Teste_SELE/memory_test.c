/* Implementation of MARCH C- */

#include "memory_test.h"

#include "RS485.h"


uint16_t EEMEM hash_flash, assinatura;


uint8_t teste_FLASH(void){

	uint16_t hash = 0;
	uint16_t i = 0;
	uint16_t n_words = 256 * 64;

	for(i = 0; i < n_words; i++){

		hash ^= pgm_read_word_near(i);
	}

	if(eeprom_read_word(&assinatura) != SIGNATURE_FLASH){ /* Qd o programa estiver pronto, alterar SIGNATURE_FLASH  */

	 eeprom_update_word(&hash_flash, hash);
	 eeprom_update_word(&assinatura, SIGNATURE_FLASH);
	}

	if(hash == eeprom_read_word(&hash_flash)){

		return 0;
	}
	else{

		return 1;
	}
}

uint8_t MarchC_minus(uint8_t* addr_base, uint16_t n) {

	writeZero(EITHER, addr_base, n);

	if(0 == readZero_writeOne(UP, addr_base, n)){

		if(0 == readOne_writeZero(UP, addr_base, n)){

			if(0 == readZero_writeOne(UP, addr_base, n)){

				if(0 == readOne_writeZero(UP, addr_base, n)){

					readZero(EITHER, addr_base, n);
					return 0;
				}

				else{

					return 1;
				}
			}
			else{

				return 1;
			}
		}
		else{

			return 1;
		}
	}
	else
	{

		return 1;
	}
}

void writeZero(uint8_t addr_direction, uint8_t* addr_base, uint16_t n) {

	int16_t cell = 0;

	if (UP == addr_direction) {

		for (cell = 0; cell <= n - 1; cell++) {

			addr_base[cell] = ZEROS;
		}

	}

	else if (DOWN == addr_direction) {

		for (cell = n - 1; cell >= 0; cell--) {

			addr_base[cell] = ZEROS;
		}
	}

	else {

		for (cell = 0; cell <= n - 1; cell++) {

			addr_base[cell] = ZEROS;
		}

	}
}

uint8_t readZero(uint8_t addr_direction, uint8_t* addr_base, uint16_t n) {

	int16_t cell = 0;

		if (UP == addr_direction) {

			for (cell = 0; cell <= n - 1; cell++) {

				if (ZEROS != addr_base[cell]) {

					return 1;

				}

			}

			return 0;

		}

		else if (DOWN == addr_direction) {

			for (cell = n - 1; cell >= 0; cell--) {

				if (ZEROS != addr_base[cell]) {

					return 1;

				}
			}

			return 0;
		}

		else {

			for (cell = 0; cell <= n - 1; cell++) {

				if (ZEROS != addr_base[cell]) {

					return 1;

				}
			}

			return 0;
		}

}

uint8_t readZero_writeOne(uint8_t addr_direction, uint8_t* addr_base, uint16_t n) {

	int16_t cell = 0;

	if (UP == addr_direction) {

		for (cell = 0; cell <= n - 1; cell++) {

			if (ZEROS == addr_base[cell]) {

				addr_base[cell] = ONES;

			}

			else {

				return 1;
			}
		}

		return 0;

	}

	else if (DOWN == addr_direction) {

		for (cell = n - 1; cell >= 0; cell--) {

			if (ZEROS == addr_base[cell]) {

				addr_base[cell] = ONES;

			}

			else {

				return 1;
			}
		}

		return 0;
	}

	else {

		for (cell = 0; cell <= n - 1; cell++) {

			if (ZEROS == addr_base[cell]) {

				addr_base[cell] = ONES;

			}

			else {

				return 1;
			}
		}

		return 0;
	}

}

uint8_t readOne_writeZero(uint8_t addr_direction, uint8_t* addr_base, uint16_t n) {

	int16_t cell = 0;

	if (UP == addr_direction) {

		for (cell = 0; cell <= n - 1; cell++) {

			if (ONES == addr_base[cell]) {

				addr_base[cell] = ZEROS;

			}

			else {

				return 1;

			}
		}

		return 0;

	}

	else if (DOWN == addr_direction) {

		for (cell = n - 1; cell >= 0; cell--) {

			if (ONES == addr_base[cell]) {

				addr_base[cell] = ZEROS;

			}

			else {

				return 1;
			}
		}

		return 0;
	}

	else {

		for (cell = 0; cell <= n - 1; cell++) {

			if (ONES == addr_base[cell]) {

				addr_base[cell] = ZEROS;

			}

			else {

				return 1;
			}
		}

		return 0;
	}
}
