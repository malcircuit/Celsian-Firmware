/*
 * sensors.c
 *
 *  Created on: Jul 31, 2016
 *      Author: matt
 */

#include "sensors.h"

void sensors_init()
{
	twi_init();
	sht20_init();
	veml_init();
	mpl_init(TEMP_AND_PRES);
}

void sensors_low_power()
{
	veml_low_power();
	mpl_low_power();
	sht20_low_power();
}
