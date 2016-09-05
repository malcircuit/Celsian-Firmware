/*
 * twi_util.h
 *
 *  Created on: Jul 31, 2016
 *      Author: matt
 */

#ifndef SENSORS_TWI_UTIL_H_
#define SENSORS_TWI_UTIL_H_

#include <nrf52.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <app_error.h>
#include <app_util_platform.h>
#include <app_twi.h>
#include "nrf_drv_config.h"

#define MAX_QUEUE_LENGTH 4

void twi_init();
uint8_t twi_read_status(uint8_t address);
void twi_write_command(uint8_t address, uint8_t command);
uint8_t twi_read_reg_byte(uint8_t address, uint8_t reg);
void twi_read_reg_bytes(uint8_t address, uint8_t reg, uint8_t *data, uint8_t length);
void twi_read_bytes(uint8_t address, uint8_t *data, uint8_t length);
uint16_t twi_read_word(uint8_t address, uint8_t reg);
void twi_read_word_crc(uint8_t address, uint8_t reg, uint16_t *word, uint8_t *checksum);
void twi_write_byte(uint8_t address, uint8_t reg, uint8_t data);
//void twi_write_bytes(uint8_t address, uint8_t reg, uint8_t *data, uint8_t length);
void twi_write_word(uint8_t address, uint8_t reg, uint8_t *word);

#endif /* SENSORS_TWI_UTIL_H_ */
