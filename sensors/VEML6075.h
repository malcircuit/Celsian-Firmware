/*
 * VEML6075.h
 *
 *  Created on: Jul 31, 2016
 *      Author: matt
 */

#ifndef SENSORS_VEML6075_H_
#define SENSORS_VEML6075_H_

#include "twi_util.h"
#include <math.h>
#include <util.h>

#define VEML_ADDRESS			0x10

#define VEML_UV_CONF_REG		0x00
#define VEML_UVA_DATA_REG		0x07
#define VEML_DUMMY_REG			0x08
#define VEML_UVB_DATA_REG		0x09
#define VEML_UVCOMP1_DATA_REG	0x0A
#define VEML_UVCOMP2_DATA_REG	0x0B
#define VEML_ID_REG				0x0C

// UV index calculation constants
#define VEML_A_COEF 	3.33 	// The default value for the UVA VIS coefficient
#define VEML_B_COEF 	2.5		// The default value for the UVA IR coefficient
#define VEML_C_COEF  	3.66	// The default value for the UVB VIS coefficient
#define VEML_D_COEF 	2.75	// The default value for the UVB IR coefficient
#define VEML_UVA_RESP	0.0011
#define VEML_UVB_RESP	0.00125

#define UV_IT2		6
#define UV_IT1		5
#define UV_IT0		4
#define HD			3
#define UV_TRIG		2
#define UV_AF		1
#define SD			0

void veml_init();
uint16_t veml_get_device_id();
double veml_get_uv_index(uint16_t *p_uva, uint16_t *p_uvb, uint16_t *p_uvd, uint16_t *p_uvcomp1, uint16_t *p_uvcomp2);
void veml_low_power();

#endif /* SENSORS_VEML6075_H_ */
