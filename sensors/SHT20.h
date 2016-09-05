/*
 * sht20.h
 *
 *  Created on: Jul 31, 2016
 *      Author: matt
 */

#ifndef SENSORS_SHT20_H_
#define SENSORS_SHT20_H_

#include "twi_util.h"
#include <math.h>

#define SHT20_ADDRESS 		0x40

#define TEMP_HOLD 			0xE3
#define RH_HOLD				0xE5
#define TEMP_NO_HOLD 		0xF3
#define RH_NO_HOLD			0xF5
#define READ_USER_REG 		0xE6
#define WRITE_USER_REG		0xE7

#define SOFT_RESET_COMMAND	0xFE

typedef enum {RH_12_T_14, RH_8_T12, RH_10_T_13, RH_11_T_11} sht20_res_t;
#define RES_RH_12_T_14_BIT	0x00
#define RES_RH_8_T_12_BIT	0x01
#define RES_RH_10_T_13_BIT	0x80
#define RES_RH_11_T_11_BIT	0x81
#define RES_MASK			0x7E

#define EN_CHIP_HEATER		0x04
#define DIS_OTP_RELOAD		0x02
#define LOW_BATT_BIT_POS	6

#define VALUE_STATUS_MASK 	0xFFFC	// Used to eliminate the status bits dangling on the LSB of temp and RH values

uint16_t sht20_get_raw_temp();
uint16_t sht20_get_raw_rh();
double sht20_get_temp();
double sht20_get_rh();

void sht20_soft_reset();
void sht20_change_res(sht20_res_t res);
uint8_t sht20_is_low_battery();
void sht20_init();
void sht20_low_power();

#endif /* SENSORS_SHT20_H_ */
