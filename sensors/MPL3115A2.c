/*
 * MPL3115A2.c
 *
 * 	Defaulting to Barometer mode
 *
 *  Created on: Jul 31, 2016
 *      Author: matt
 */

#include "MPL3115A2.h"

static inline double Q_to_float(int32_t value, uint8_t n)
{
	return ((double) value)/(1 << n);
}

void mpl_init(mpl_ost_type_t ost_cfg)
{
	mpl_ost_cfg(ost_cfg);
}

void mpl_ost_cfg(mpl_ost_type_t ost_cfg)
{
	// Enable data ready triggers
	uint8_t pt_data_cfg = 0;

	if (ost_cfg == PRESSURE)
	{
		pt_data_cfg |= ((1 << PDEFE) | (1 << DREM));
	}
	else if (ost_cfg == TEMP)
	{
		pt_data_cfg |= ((1 << TDEFE) | (1 << DREM));
	}
	else
	{
		pt_data_cfg |= ((1 << PDEFE) | (1 << TDEFE)  | (1 << DREM));
	}

	twi_write_byte(MPL_ADDRESS, MPL_PT_DATA_CFG_REG, pt_data_cfg);
}

bool mpl_start_oneshot()
{
	uint8_t ctrl_reg1 = twi_read_reg_byte(MPL_ADDRESS, MPL_CTRL_REG1);	// Read OST

	if ((ctrl_reg1 >> OST) ^ 1)		// If the OST bit is cleared, we're good to go
	{
		ctrl_reg1 |= (1 << OST);	// Set OST

		twi_write_byte(MPL_ADDRESS, MPL_CTRL_REG1, ctrl_reg1);
		return true;
	}

	return false;
}

void mpl_oneshot_block(double *pressure, double *temp)
{
	uint8_t dr_status = twi_read_reg_byte(MPL_ADDRESS, MPL_DR_STATUS_REG);

	if (dr_status > 0)	// If old data in registers
	{
		mpl_get_pressure();
		mpl_get_temp();
	}

	uint8_t ctrl_reg1 = twi_read_reg_byte(MPL_ADDRESS, MPL_CTRL_REG1);	// Read OST

	if ((ctrl_reg1 >> OST) ^ 1)		// If the OST bit is cleared, we're good to go
	{
		ctrl_reg1 |= (1 << OST);	// Set OST

		twi_write_byte(MPL_ADDRESS, MPL_CTRL_REG1, ctrl_reg1);

		do
		{
			dr_status = twi_read_reg_byte(MPL_ADDRESS, MPL_DR_STATUS_REG);
		}
		while(!(dr_status & (1 << PTDR)));	// Wait for data to arrive

		*pressure = mpl_get_pressure();
		*temp = mpl_get_temp();
	}
}

static inline double get_pres_reg(uint8_t reg)
{
	uint8_t data[3] = {0};
	twi_read_reg_bytes(MPL_ADDRESS, reg, data, 3);

	int32_t pressure = ((uint32_t)data[0] << 16) | ((uint32_t) data[1] << 8) | data[2];

	return Q_to_float(pressure >> 4, 2);
}

static inline double get_temp_reg(uint8_t reg)
{
	uint8_t data[2] = {0};
	twi_read_reg_bytes(MPL_ADDRESS, reg, data, 2);

	int16_t temp = ((uint16_t) data[0] << 8) | data[1];

	return Q_to_float(temp >> 4, 4);
}

double mpl_get_pressure()
{
	return get_pres_reg(MPL_OUT_P_REG);
}

double mpl_get_temp()
{
	return get_temp_reg(MPL_OUT_T_REG);
}

double mpl_get_pressure_delta()
{
	return get_pres_reg(MPL_OUT_P_DELTA_REG);
}

double mpl_get_temp_delta()
{
	return get_temp_reg(MPL_OUT_T_DELTA_REG);
}

/**
 * Set the target temperature and temperature window
 * (in degrees C) to trigger an interrupt.  Range is +/- 128 C.
 */
void mpl_set_temp_target(int8_t target_degrees_c, uint8_t degrees_c_window)
{
	twi_write_byte(MPL_ADDRESS, MPL_T_TGT_REG, (uint8_t)target_degrees_c);
	twi_write_byte(MPL_ADDRESS, MPL_T_WND_REG, degrees_c_window);
}

/**
 * Set the target pressure and pressure range (in Pa)
 * to trigger an interrupt.  Max for both is 2^17 Pa.
 */
void mpl_set_pres_target(uint32_t pascals, uint32_t pascals_window)
{
	uint8_t data[2] = {(pascals/2 >> 8) & 0xFF, (pascals/2) & 0xFF};
	twi_write_word(MPL_ADDRESS, MPL_P_TGT_REG, data);

	data[1] = (pascals_window/2 >> 8) & 0xFF;
	data[0] = (pascals_window/2) & 0xFF;
	twi_write_word(MPL_ADDRESS, MPL_P_WND_REG, data);
}

double mpl_get_min_pres()
{
	return get_pres_reg(MPL_P_MIN_REG);
}

double mpl_get_max_pres()
{
	return get_pres_reg(MPL_P_MAX_REG);
}

double mpl_get_min_temp()
{
	return get_temp_reg(MPL_T_MIN_REG);
}

double mpl_get_max_temp()
{
	return get_temp_reg(MPL_T_MAX_REG);
}

//void mpl_reset_min_pres()
//{
//	uint8_t data[3] = {0};
//	twi_write_bytes(MPL_ADDRESS, MPL_P_MIN_REG, data, 3);
//}

//void mpl_reset_max_pres()
//{
//	uint8_t data[3] = {0};
//	twi_write_bytes(MPL_ADDRESS, MPL_P_MAX_REG, data, 3);
//}

void mpl_reset_min_temp()
{
	uint8_t data[2] = {0};
	twi_write_word(MPL_ADDRESS, MPL_T_MIN_REG, data);
}

void mpl_reset_max_temp()
{
	uint8_t data[2] = {0};
	twi_write_word(MPL_ADDRESS, MPL_T_MAX_REG, data);
}

uint8_t mpl_get_device_id()
{
	return twi_read_reg_byte(MPL_ADDRESS, MPL_WHO_AM_I_REG);
}

void mpl_low_power()
{

}
