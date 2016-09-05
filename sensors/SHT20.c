/*
 * SHT20.c
 *
 *  Created on: Jul 31, 2016
 *      Author: matt
 */

#include "SHT20.h"

void sht20_init()
{

}

uint16_t sht20_get_raw_temp()
{
	uint16_t temp;
	uint8_t crc;
	twi_read_word_crc(SHT20_ADDRESS, TEMP_HOLD, &temp, &crc);

	return temp & VALUE_STATUS_MASK;
}

double sht20_get_temp()
{
	return (double)sht20_get_raw_temp()/0x10000*175.72-46.85;
}

uint16_t sht20_get_raw_rh()
{
	uint16_t rh;
	uint8_t crc;
	twi_read_word_crc(SHT20_ADDRESS, RH_HOLD, &rh, &crc);

	return rh & VALUE_STATUS_MASK;
}

double sht20_get_rh()
{
	return (double)sht20_get_raw_rh()/0x10000*125 - 6;
}

// TODO: Verify that this works
void sht20_soft_reset()
{
	twi_write_command(SHT20_ADDRESS, SOFT_RESET_COMMAND);
}

// TODO: Verify that this works
void sht20_change_res(sht20_res_t res)
{
	uint8_t user_reg = twi_read_reg_byte(SHT20_ADDRESS, READ_USER_REG);

	user_reg &= RES_MASK;

	switch (res)
	{
	case RH_8_T12:
		user_reg |= RES_RH_8_T_12_BIT;
		break;
	case RH_10_T_13:
		user_reg |= RES_RH_10_T_13_BIT;
		break;
	case RH_11_T_11:
		user_reg |= RES_RH_11_T_11_BIT;
		break;
	default:
		break;
	}

	twi_write_byte(SHT20_ADDRESS, WRITE_USER_REG, user_reg);
}

// TODO: Verify that this works
uint8_t sht20_is_low_battery()
{
	return (twi_read_reg_byte(SHT20_ADDRESS, READ_USER_REG) >> LOW_BATT_BIT_POS) & 1;
}

void sht20_low_power()
{

}
