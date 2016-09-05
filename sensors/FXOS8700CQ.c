/*
 * FXOS8700CQ.c
 *
 *  Created on: Jul 31, 2016
 *      Author: matt
 */

#include "FXOS8700CQ.h"

uint8_t fxo_get_device_id()
{
	return twi_read_reg_byte(FXO_ADDRESS, FXO_WHO_AM_I_REG);
}

void fxo_init()
{
	fxo_soft_reset();
	uint8_t ctrl_reg1 = bts(DR1) | bts(DR0) | bts(LNOISE) | bts(ACTIVE);	// 100 Hz ODR, Low Noise, Active
	uint8_t ctrl_reg2 = bts(MODS1) | bts(MODS0);							// Low Power mode
	uint8_t xyz_data_cfg = bts(FS0);										// +/- 4 g range
//	uint8_t hp_filter_cutoff = bts

	uint8_t pulse_cfg = bts(PLS_DPA) | bts(PLS_ZDPEFE);						// Enable Z-Axis Double Pulse flag
	uint8_t pulse_thsz = 48;												// Z-Axis pulse threshold = ~1 g
	uint8_t pulse_tmlt = 80;												// Pulse timelimit = ~0.25 seconds
	uint8_t pulse_ltcy = 4;													// Pulse Latency = ~0.5 seconds
	uint8_t pulse_wind = 100;												// Pulse Window = ~1 seconds

	uint8_t ctrl_reg4 =	bts(INT_EN_PULSE);
	uint8_t ctrl_reg5 =	bts(INT_CFG_PULSE);

	twi_write_byte(FXO_ADDRESS, FXO_XYZ_DATA_CFG_REG, xyz_data_cfg);
	twi_write_byte(FXO_ADDRESS, FXO_PULSE_CFG_REG, pulse_cfg);
	twi_write_byte(FXO_ADDRESS, FXO_PULSE_THSZ_REG, pulse_thsz);
	twi_write_byte(FXO_ADDRESS, FXO_PULSE_TMLT_REG, pulse_tmlt);
	twi_write_byte(FXO_ADDRESS, FXO_PULSE_LTCY_REG, pulse_ltcy);
	twi_write_byte(FXO_ADDRESS, FXO_PULSE_WIND_REG, pulse_wind);
	twi_write_byte(FXO_ADDRESS, FXO_CTRL_REG2, ctrl_reg2);
	twi_write_byte(FXO_ADDRESS, FXO_CTRL_REG4, ctrl_reg4);
	twi_write_byte(FXO_ADDRESS, FXO_CTRL_REG5, ctrl_reg5);

	twi_write_byte(FXO_ADDRESS, FXO_CTRL_REG1, ctrl_reg1);				// Start measuring
}

void fxo_get_accel(int16_t *x, int16_t *y, int16_t *z)
{
	uint8_t data[6] = {0};
	twi_read_reg_bytes(FXO_ADDRESS, FXO_OUT_X_REG, data, 6);

	*x = (int16_t)(((uint16_t) data[0] << 8) | data[1]) >> 2;
	*y = (int16_t)(((uint16_t) data[2] << 8) | data[3]) >> 2;
	*z = (int16_t)(((uint16_t) data[4] << 8) | data[5]) >> 2;
}

void fxo_soft_reset()
{
	twi_write_byte(FXO_ADDRESS, FXO_CTRL_REG1, 0x00);			// Set to Standby for config
	twi_write_byte(FXO_ADDRESS, FXO_CTRL_REG2, bts(RST));		// Trigger reset

	nrf_delay_ms(1);											// Datasheet's recommendation
}
