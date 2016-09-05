/*
 * VEML6075.c
 *
 *  Created on: Jul 31, 2016
 *      Author: matt
 */

#include "VEML6075.h"

void veml_init()
{
	uint8_t data[2] = {bts(UV_IT0) | bts(HD), 0};

	twi_write_word(VEML_ADDRESS, VEML_UV_CONF_REG, data);
}

uint16_t veml_get_device_id()
{
	uint8_t data[2] = {0};

	twi_read_reg_bytes(VEML_ADDRESS, VEML_ID_REG, data, 2);

	return ((uint16_t)data[1] << 8) | data[0];
}

#include <debug.h>
double veml_get_uv_index(uint16_t *p_uva, uint16_t *p_uvb, uint16_t *p_uvd, uint16_t *p_uvcomp1, uint16_t *p_uvcomp2)
{
	uint16_t uva, uvb, uvd, uvcomp1, uvcomp2;

	double uvacomp, uvbcomp;

	uint8_t data[2] = {0};

	twi_read_reg_bytes(VEML_ADDRESS, VEML_UVA_DATA_REG, data, 2);
	uva = ((uint16_t)data[1] << 8) | data[0];

	twi_read_reg_bytes(VEML_ADDRESS, VEML_DUMMY_REG, data, 2);
	uvd = ((uint16_t)data[1] << 8) | data[0];

	twi_read_reg_bytes(VEML_ADDRESS, VEML_UVB_DATA_REG, data, 2);
	uvb = ((uint16_t)data[1] << 8) | data[0];

	twi_read_reg_bytes(VEML_ADDRESS, VEML_UVCOMP1_DATA_REG, data, 2);
	uvcomp1 = ((uint16_t)data[1] << 8) | data[0];

	twi_read_reg_bytes(VEML_ADDRESS, VEML_UVCOMP2_DATA_REG, data, 2);
	uvcomp2 = ((uint16_t)data[1] << 8) | data[0];

	uvacomp = (uva - uvd) - (double)(VEML_A_COEF) * (uvcomp1 - uvd) - (double)(VEML_B_COEF) * (uvcomp2 - uvd);
	uvbcomp = (uvb - uvd) - (double)(VEML_C_COEF) * (uvcomp1 - uvd) - (double)(VEML_D_COEF) * (uvcomp2 - uvd);

	debug_printf("UVA: %04X\tUVB: %04X\tUVD: %04X\n", uva, uvb, uvd);
	debug_printf("UVcomp1: %04X\tUVcomp2: %04X\n", uvcomp1, uvcomp2);
	debug_printf("UVAcomp: %d\tUVBcomp: %d\n", (int)uvacomp, (int)uvbcomp);

	*p_uva = uva;
	*p_uvb = uvb;
	*p_uvd = uvd;
	*p_uvcomp1 = uvcomp1;
	*p_uvcomp2 = uvcomp2;

	return ( (uvbcomp * VEML_UVB_RESP) + (uvacomp * VEML_UVA_RESP) ) / 2.0;
}

void veml_low_power()
{
	uint8_t data[2] = {bts(SD), 0};		// Shutdown

	twi_write_word(VEML_ADDRESS, VEML_UV_CONF_REG, data);
}

