/*
 * MPL3115A2.h
 *
 *  Created on: Jul 31, 2016
 *      Author: matt
 */

#ifndef SENSORS_MPL3115A2_H_
#define SENSORS_MPL3115A2_H_

#include "twi_util.h"
#include <math.h>
#include <nrf_drv_gpiote.h>
#include <util.h>

#define PRES_INT1_PIN 		12
#define PRES_INT2_PIN		13

#define PRES_INT_POLARITY NRF_GPIOTE_POLARITY_HITOLO

#define MPL_ADDRESS			0x60

#define MPL_STATUS_REG			0x00
#define MPL_OUT_P_REG			0x01
#define MPL_OUT_T_REG			0x04
#define MPL_DR_STATUS_REG		0x06
#define MPL_OUT_P_DELTA_REG		0x07
#define MPL_OUT_T_DELTA_REG		0x0A
#define MPL_WHO_AM_I_REG		0x0C
#define MPL_F_STATUS_REG		0x0D
#define MPL_F_DATA_REG			0x0E
#define MPL_F_SETUP_REG			0x0F
#define MPL_TIME_DLY_REG		0x10
#define MPL_SYSMOD_REG			0x11
#define MPL_INT_SOURCE_REG		0x12
#define MPL_PT_DATA_CFG_REG		0x13
#define MPL_BAR_IN_REG			0x14
#define MPL_P_TGT_REG			0x16
#define MPL_T_TGT_REG			0x18
#define MPL_P_WND_REG			0x19
#define MPL_T_WND_REG			0x1B
#define MPL_P_MIN_REG			0x1C
#define MPL_T_MIN_REG			0x1F
#define MPL_P_MAX_REG			0x21
#define MPL_T_MAX_REG			0x24
#define MPL_CTRL_REG1			0x26
#define MPL_CTRL_REG2			0x27
#define MPL_CTRL_REG3			0x28
#define MPL_CTRL_REG4			0x29
#define MPL_CTRL_REG5			0x2A
#define MPL_OFF_P_REG			0x2B
#define MPL_OFF_T_REG			0x2C
#define MPL_OFF_H_REG			0x2D

// DR_STATUS bits
#define PTOW 		7
#define POW			6
#define TOW			5
#define PTDR		3
#define PDR			2
#define TDR			1

// F_STATUS bits
#define MPL_F_OVF		7
#define MPL_F_WMRK_FLAG	6
#define MPL_F_CNT_MASK 	0x3F

// F_SETUP bits
#define MPL_F_MODE		6
#define MPL_F_WMRK_MASK	0x7F

#ifdef SRC_DRDY
#undef	SRC_DRDY
#endif
// INT_SOURCE bits
#define SRC_DRDY	7
#define SRC_FIFO	6
#define SRC_PW		5
#define SRC_TW		4
#define SRC_PTH		3
#define SRC_TTH		2
#define SRC_PCHG	1
#define SRC_TCHG	0

// PT_DATA_CFG bits
#define DREM		2
#define PDEFE		1
#define TDEFE		0

#ifdef RST
#undef RST
#endif

// CTRL_REG1 bits
#define ALT			7
#define RAW			6
#define OS2			5
#define OS1			4
#define OS0			3
#define RST			2
#define OST			1
#define SBYB		0

// CTRL_REG2 bits
#define LOAD_OUTPUT	5
#define ALARM_SEL	4
#define ST_MASK		0x0F

// CTRL_REG3 bits
#define IPOL1		5
#define PP_OD1		4
#define IPOL2		1
#define PP_OD2		0

#ifdef INT_EN_DRDY
#undef INT_EN_DRDY
#endif

// MPL_CTRL_REG4 bits
#define INT_EN_DRDY		7
#define INT_EN_FIFO		6
#define INT_EN_PW		5
#define INT_EN_TW		4
#define INT_EN_PTH		3
#define INT_EN_TTH		2
#define INT_EN_PCHG		1
#define INT_EN_TCHG		0

#ifdef INT_CFG_DRDY
#undef INT_CFG_DRDY
#endif

// MPL_CTRL_REG5 bits
#define INT_CFG_DRDY	7
#define INT_CFG_FIFO	6
#define INT_CFG_PW		5
#define INT_CFG_TW		4
#define INT_CFG_PTH		3
#define INT_CFG_TTH		2
#define INT_CFG_PCHG	1
#define INT_CFG_TCHG	0

typedef enum {ALTITUDE_MODE, PRESSURE_MODE} mpl_sys_mode_t;
typedef enum {PRES_INT1, PRES_INT2} mpl_int_pin_t;
typedef enum {PRESSURE, TEMP, TEMP_AND_PRES} mpl_ost_type_t;

#define OST_PRESSURE	1
#define OST_TEMP		2
#define OST_MASK		3

#define	DATA_READY_INT 	0x80
#define	FIFO_INT		0x40
#define	PRES_WINDOW_INT	0x20
#define	TEMP_WINDOW_INT	0x10
#define	PRES_THRESH_INT	0x08
#define	TEMP_THRESH_INT	0x04
#define	PRES_CHANGE_INT	0x02
#define	TEMP_CHANGE_INT	0x01

void mpl_init(mpl_ost_type_t ost_cfg);
void mpl_ost_cfg(mpl_ost_type_t ost_cfg);
bool mpl_start_oneshot();
void mpl_oneshot_block(double *pressure, double *temp);
double mpl_get_pressure();
double mpl_get_temp();
double mpl_get_pressure_delta();
double mpl_get_temp_delta();
void mpl_set_temp_target(int8_t target_degrees_c, uint8_t degrees_c_window);
void mpl_set_pres_target(uint32_t pascals, uint32_t pascals_window);
double mpl_get_min_pres();
double mpl_get_max_pres();
double mpl_get_min_temp();
double mpl_get_max_temp();
void mpl_reset_min_pres();
void mpl_reset_max_pres();
void mpl_reset_min_temp();
void mpl_reset_max_temp();
uint8_t mpl_get_device_id();
void mpl_low_power();

#endif /* SENSORS_MPL3115A2_H_ */
