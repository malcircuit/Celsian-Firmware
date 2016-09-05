/*
 * sensors.h
 *
 *  Created on: Jul 31, 2016
 *      Author: matt
 */

#ifndef SENSORS_SENSORS_H_
#define SENSORS_SENSORS_H_

#include "SHT20.h"
#include "FXOS8700CQ.h"
#include "MPL3115A2.h"
#include "VEML6075.h"
#include "twi_util.h"

void sensors_init();
void sensors_low_power();

#endif /* SENSORS_SENSORS_H_ */
