/*
 * FXOS8700CQ.h
 *
 *  Created on: Jul 31, 2016
 *      Author: matt
 */

#ifndef SENSORS_FXOS8700CQ_H_
#define SENSORS_FXOS8700CQ_H_

#include "twi_util.h"
#include <util.h>
#include <nrf_drv_gpiote.h>
#include <nrf_delay.h>

#define FXO_ADDRESS					0x1E

#define FXO_STATUS_REG 				0x00

/* DR_STATUS bits */
#define ZYXOW	7	// Asserted when X, Y, or Z data is overwritten
#define ZOW		6	// Asserted when Z data is overwritten
#define YOW		5	// Asserted when Y data is overwritten
#define XOW		4	// Asserted when X data is overwritten
#define ZYXDR	3	// Asserted when X, Y, or Z data is ready
#define ZDR		2	// Asserted when Z data is ready
#define YDR		1	// Asserted when Y data is ready
#define XDR		0	// Asserted when X data is ready

/* F_STATUS bits */
#define F_OVF		7	// Asserted when the FIFO overflows
#define F_WKRK_FLAG	6	// Asserted when the number of samples in the FIFO hits F_CNT
#define F_CNT5		5	// FIFO sample count watermark limit
#define F_CNT4		4
#define F_CNT3		3
#define F_CNT2		2
#define F_CNT1		1
#define F_CNT0		0

#define FXO_OUT_X_REG 				0x01
#define FXO_OUT_Y_REG 				0x03
#define FXO_OUT_Z_REG 				0x05

#define FXO_F_SETUP_REG				0x09

/* F_SETUP bits */
#define F_MODE1			7	// FIFO buffer operating mode
#define F_MODE0			6
#define F_WMRK5			5	// FIFO sample count watermark
#define F_WMRK4			4
#define F_WMRK3			3
#define F_WMRK2			2
#define F_WMRK1			1
#define F_WMRK0			0

#define F_MODE_FIFO_DIABLED			0
#define F_MODE_RING_BUFFER			1
#define F_MODE_STOP_ON_OVF			2
#define F_MODE_FIFO_TRIGGER_MODE	3

#define FXO_TRIG_CFG_REG			0x0A

/* TRIG_CFG bits */
#define TRIG_TRANS		6	// Transient interrupt FIFO trigger
#define TRIG_LNDPRT		5	// Orientation interrupt FIFO trigger
#define TRIG_PULSE		4	// Pulse interrupt FIFO trigger
#define TRIG_FFMT		3	// Freefall/motion interrupt FIFO trigger
#define TRIG_A_VECM		2	// Accel vector-magnitude FIFO trigger

#define FXO_SYSMOD_REG				0x0B

/* SYSMOD bits */
#define FGERR		7	// FIFO gate error
#define FGT4		6	// ODR time units since fgerr was asserted
#define FGT3		5
#define FGT2		4
#define FGT1		3
#define FGT0		2
#define SYSMOD1		1	// System mode
#define SYSMOD0		1

#define SYSMOD_STANDBY	0
#define SYSMOD_WAKE		1
#define SYSMOD_SLEEP	2

#define FXO_INT_SOURCE_REG			0x0C

/* INT_SOURCE bits */
#define SRC_ASLP		7	// Interrupt came from Sleep/Wake event
#define SRC_FIFO		6	// Interrupt came from FIFO event
#define SRC_TRANS		5	// Interrupt came from Accel Transient event
#define SRC_LNDPRT		4	// Interrupt came from Orientation event
#define SRC_PULSE		3	// Interrupt came from Accel Pulse event
#define SRC_FFMT		2	// Interrupt came from Freefall/Motion event
#define SRC_A_VECM		1	// Interrupt came from Accel vector-mag event
#define SRC_DRDY		0	// Interrupt came from Data Ready event

#define FXO_WHO_AM_I_REG			0x0D
#define FXO_XYZ_DATA_CFG_REG		0x0E

/* XYZ_DATA_CFG bits */
#define HPF_OUT		4	// Enable accel output data high-pass filter
#define FS1			1	// Accelerometer full scale range
#define FS0			0

#define FS_2G		0	// +/- 2g range
#define FS_4G		1	// +/- 4g range
#define FS_8G		2	// +/- 8g range
#define FS_RESERVED	3	// Do not use

#define FXO_HP_FILTER_CUTOFF_REG	0x0F

/* HP_FILTER_CUTOFF bits */
#define PULSE_HPF_BYP	5	// Bypass accel output high-pass filter for pulse processing
#define PULSE_LPF_EN	4	// Enable accel output low-pass filter for pulse processing
#define SEL1			1	// HPF cutoff frequency selection (see datasheet Table 64, pg 57)
#define SEL0			0

#define FXO_PL_STATUS_REG			0x10

/* PL_STATUS bits */
#define NEWLP		7	// Orientation status change flag
#define LO			6	// Z-tilt angle lockout flag
#define LAPO1		2	// Landscape/portrait orienation
#define LAPO0		1
#define BAFRO		0	// Back or front orientation

#define PORTRAIT_UP			0	// y = ~ –1 g, x = ~0 g
#define PORTRAIT_DOWN		1	// y = ~ +1 g, x = ~0 g
#define LANDSCAPE_RIGHT		2	// y = ~0 g, x = ~ +1 g
#define LANDSCAPE_LEFT		3	// y = ~0 g, x = ~ -1 g

#define BACK_FACING			1
#define FRONT_FACING		0

#define FXO_PL_CFG_REG				0x11

/* PL_CFG_REG */
#define DBCNTM		7	// Debounce counter mode
#define PL_EN		6	// Orientation detection enabled

#define	FXO_PL_COUNT_REG			0x12
#define FXO_PL_BF_ZCOMP_REG			0x13

/* PL_BF_ZCOMP_REG */
#define BKFR1		7	// Back/front trip angle threshold
#define BKFR0		6
#define ZLOCK2		2	// Z-lock angle threshold
#define ZLOCK1		1
#define ZLOCK0		0

#define ZLOCK_14_DEGREES	0
#define ZLOCK_17_DEGREES	1
#define ZLOCK_21_DEGREES	2
#define ZLOCK_24_DEGREES	3
#define ZLOCK_28_DEGREES	4	// Default
#define ZLOCK_32_DEGREES	5
#define ZLOCK_36_DEGREES	6
#define ZLOCK_40_DEGREES	7

// Back-to-front threshold
#define BACK_FRONT_80_DEGREES	0
#define BACK_FRONT_75_DEGREES	1
#define BACK_FRONT_70_DEGREES	2	// Default
#define BACK_FRONT_65_DEGREES	3

// Front-to-back threshold
#define FRONT_BACK_100_DEGREES	0
#define FRONT_BACK_105_DEGREES	1
#define FRONT_BACK_110_DEGREES	2	// Default
#define FRONT_BACK_115_DEGREES	3

#define FXO_PL_THS_REG_REG			0x14

/* PL_THS_REG bits */
#define PL_THS4		7	// Portrait to landscape trip threshold
#define PL_THS3		6
#define PL_THS2		5
#define PL_THS1		4
#define PL_THS0		3
#define HYS2		2 	// Portrait to landscape trip threshold hysteresis
#define HYS1		1
#define HYS0		0

#define PL_THS_15_DEGREES	0x07
#define PL_THS_20_DEGREES	0x09
#define PL_THS_30_DEGREES	0x0C
#define PL_THS_35_DEGREES	0x0D
#define PL_THS_40_DEGREES	0x0F
#define PL_THS_45_DEGREES	0x10
#define PL_THS_55_DEGREES	0x13
#define PL_THS_60_DEGREES	0x14
#define PL_THS_70_DEGREES	0x17
#define PL_THS_75_DEGREES	0x19

#define HYS_0_DEGREES		0
#define HYS_4_DEGREES		1
#define HYS_7_DEGREES		2
#define HYS_11_DEGREES		3
#define HYS_14_DEGREES		4
#define HYS_17_DEGREES		5
#define HYS_21_DEGREES		6
#define HYS_24_DEGREES		7

#define FXO_A_FFMT_CFG_REG			0x15

/**
 * A_FFMT_CFG bits
 *
 * Threshold behavior depends on whether the Freefall or Motion
 * trigger is enabled. A Freefall event is the logical AND combination
 * of LOW-g X, Y, and Z BELOW threshold event triggers. A Motion event
 * is the logical OR combination of HIGH-g X, Y, and Z ABOVE threshold
 * event triggers.
 */
#define A_FFMT_ELE		7	// Enable FFMT event flag latch
#define A_FFMT_OAE		6	// Enable Motion flag (default is Freefall)
#define A_FFMT_ZEFE		5	// Enable flag on accel above/below Z threshold
#define A_FFMT_YEFE		4	// Enable flag on accel above/below Y threshold
#define A_FFMT_XEFE		3	// Enable flag on accel above/below X threshold

#define FXO_A_FFMT_SRC_REG			0x16

/* A_FFMT_SRC bits */
#define A_FFMT_EA		7	// One or more FFMT flags have been asserted
#define A_FFMT_ZHE		5	// Z-high event flag
#define A_FFMT_ZHP		4	// Z-high event polarity flag
#define A_FFMT_YHE		3	// Y-high event flag
#define A_FFMT_YHP		2	// Y-high event polarity flag
#define A_FFMT_XHE		1	// X-high event flag
#define A_FFMT_XHP		0	// X-high event polarity flag

#define FXO_A_FFMT_THS_REG			0x17

/* A_FFMT_THS bits */
#define A_FFMT_DBCNTM		7	// FFMT even debounce counter clear mode
#define THS6				6	// General Freefall/Motion detection threshold (63 mg/LSB)
#define THS5				5
#define THS4				4
#define THS3				3
#define THS2				2
#define THS1				1
#define THS0				0

#define	FXO_A_FFMT_COUNT_REG		0x18

#define FXO_TRANSIENT_CFG_REG		0x1D

/* TRANSIENT_CFG bits */
#define TRAN_ELE		4	// Enable Transient Accel event latch
#define TRAN_ZEFE		3	// Enable Z-axis transient event flag
#define TRAN_YEFE		2	// Enable Y-axis transient event flag
#define TRAN_XEFE		1	// Enable X-axis transient event flag
#define TRAN_HPF_BYP	0	// Enable accel data high-pass filter bypass

#define FXO_TRANSIENT_SRC_REG		0x1E

/* A_FFMT_SRC bits */
#define TRAN_EA			6	// Transient event active flag
#define TRAN_ZEF		5	// Z-axis transient event flag
#define TRAN_ZPOL		4	// Z-axis transient polarity flag
#define TRAN_YEF		3	// Y-axis transient event flag
#define TRAN_YPOL		2	// Y-axis transient event polarity flag
#define TRAN_XEF		1	// X-axis transient event flag
#define TRAN_XPOl		0	// X-axis transient event polarity flag

#define FXO_TRANSIENT_THS_REG		0x1F

/* TRANSIENT_THS bits */
#define TR_DBCNTM		7	// Accel Transient event debounce counter clear mode
#define TR_THS6			6	// General Transient threshold (+/- 8g 63 mg/LSB, max +/- 4g)
#define TR_THS5			5
#define TR_THS4			4
#define TR_THS3			3
#define TR_THS2			2
#define TR_THS1			1
#define TR_THS0 		0

#define FXO_TRANSIENT_COUNT_REG		0x20

#define FXO_PULSE_CFG_REG			0x21

/* PULSE_CFG bits */
#define PLS_DPA			7	// Enable double-tap abort
#define PLS_ELE			6	// Enable Pulse event latch
#define PLS_ZDPEFE		5	// Enable double-pulse event flag on Z-axis
#define PLS_ZSPEFE		4	// Enable single-pulse event flag on Z-axis
#define PLS_YDPEFE		3	// Enable double-pulse event flag on Y-axis
#define PLS_YSPEFE		2	// Enable single-pulse event flag on Y-axis
#define PLS_XDPEFE		1	// Enable double-pulse event flag on X-axis
#define PLS_XSPEFE		0	// Enable single-pulse event flag on X-axis

#define FXO_PULSE_SRC_REG			0x22

/* PULSE_SRC bits */
#define PLS_SRC_EA		7	// Pulse event active flag
#define PLS_SRC_AXZ		6	// Z-axis pulse event flag
#define PLS_SRC_AXY		5	// Y-axis pulse event flag
#define PLS_SRC_AXX		4	// X-axis pulse event flag
#define PLS_SRC_DPE		3	// Double pulse on first event flag
#define PLS_SRC_POLZ	2	// Z-axis pulse event polarity
#define PLS_SRC_POLY	1	// Y-axis pulse event polarity
#define PLS_SRC_POLX	0	// X-axis pulse event polarity

#define FXO_PULSE_THSX_REG			0x23
#define FXO_PULSE_THSY_REG			0x24
#define FXO_PULSE_THSZ_REG			0x25
#define FXO_PULSE_TMLT_REG			0x26
#define FXO_PULSE_LTCY_REG			0x27
#define FXO_PULSE_WIND_REG			0x28

#define FXO_ASLP_COUNT_REG			0x29

#define FXO_CTRL_REG1				0x2A

/* CTRL_REG1 bits */
#define ASLP_RATE1		7	// Auto-wake sample frequency
#define ASLP_RATE0		6
#define DR2				5	// Output data rate (ODR)
#define DR1				4
#define DR0				3
#define LNOISE			2	// Reduced rate or full-scale
#define F_READ			1	// Fast read mode
#define ACTIVE			0	// Active mode

#define ASLP_RATE_50HZ		0	// Sleep ODR 50 Hz
#define ASLP_RATE_12_5_HZ	1	// Sleep ODR 12.5 Hz
#define ASLP_RATE_6_25_HZ	2	// Sleep ODR 6.25 Hz
#define ASLP_RATE_1_56_HZ	3	// Sleep ODR 1.56 Hz

#define DR_SINGLE_800_HZ	0	// Single sensor mode ODR 800 Hz
#define DR_SINGLE_400_HZ	1	// Single sensor mode ODR 400 Hz
#define DR_SINGLE_200_HZ	2	// Single sensor mode ODR 200 Hz
#define DR_SINGLE_100_HZ	3	// Single sensor mode ODR 100 Hz
#define DR_SINGLE_50_HZ		4	// Single sensor mode ODR 50 Hz
#define DR_SINGLE_12_5_HZ	5	// Single sensor mode ODR 12.5 Hz
#define DR_SINGLE_6_25_HZ	6	// Single sensor mode ODR 6.25 Hz
#define DR_SINGLE_1_56_HZ	7	// Single sensor mode ODR 1.56 Hz

#define DR_HYBRID_400_HZ	0	// Dual sensor mode ODR 400 Hz
#define DR_HYBRID_200_HZ	1	// Dual sensor mode ODR 200 Hz
#define DR_HYBRID_100_HZ	2	// Dual sensor mode ODR 100 Hz
#define DR_HYBRID_50_HZ		3	// Dual sensor mode ODR 50 Hz
#define DR_HYBRID_25_HZ		4	// Dual sensor mode ODR 25 Hz
#define DR_HYBRID_6_25_HZ	5	// Dual sensor mode ODR 6.25 Hz
#define DR_HYBRID_3_125_HZ	6	// Dual sensor mode ODR 3.125 Hz
#define DR_HYBRID_0_7813_HZ	5	// Dual sensor mode ODR 0.7813 Hz

#define FXO_CTRL_REG2				0x2B

/* CTRL_REG2 bits */
#define ST			7	// Enable accelerometer self-test
#define RST			6	// Software reset
#define SMODS1		4	// Accelerometer sleep mode oversample ratio (OSR) mode
#define SMODS0		3
#define SLPE		2	// Enable Auto-sleep
#define MODS1		1	// Accelerometer wake mode oversample ratio (OSR) mode
#define MODS0		0

// For OSR vs. OSR mode, check FXOS8700CQ datasheet Table 39 (pg 47)
#define OSR_NORMAL				0
#define OSR_LOW_NOISE_LOW_POWER	1
#define OSR_HIGH_RES			2
#define OSR_LOW_POWER			3

#define FXO_CTRL_REG3				0x2C

/* CTRL_REG3 bits */
#define FIFO_GATE			7	// Enable FIFO sleep-to-wake/wake-to-sleep gate
#define WAKE_TRAN			6	// Enable transient system wake interrupt
#define WAKE_LNDPRT			5	// Enable orientation system wake interrupt
#define WAKE_PULSE			4	// Enable pulse system wake interrupt
#define WAKE_FFMT			3	// Enable freefall/motion system wake interrupt
#define WAKE_A_VECM			2	// Enable accel vector-mag system wake interrupt
#define IPOL				1	// Interrupt output polarity (0 is active low)
#define PPOD				0	// Interrupt output open drain enable

#define FXO_CTRL_REG4				0x2D

/* CRTL_REG4 bits */
#define	INT_EN_ASLP			7	// Enable Auto-sleep/wake interrupt
#define	INT_EN_FIFO			6	// Enable FIFO interrupt
#define	INT_EN_TRANS		5	// Enable Accel Transient interrupt
#define	INT_EN_LNDPRT		4	// Enable Orientation interrupt
#define	INT_EN_PULSE		3	// Enable Accel Pulse interrupt
#define	INT_EN_FFMT			2	// Enable Freefall/Motion interrupt
#define	INT_EN_A_VECM		1	// Enable Accel vector-mag interrupt
#define	INT_EN_DRDY			0	// Enable Data Ready interrupt

#define FXO_CTRL_REG5				0x2E

/**
 *  CRTL_REG5 bits
 *
 *  INT1 = 1, INT2 = 0
 */
#define	INT_CFG_ASLP		7	// Auto-sleep/wake interrupt pin select
#define	INT_CFG_FIFO		6	// FIFO interrupt pin select
#define	INT_CFG_TRANS		5	// Accel transient interrupt pin select
#define	INT_CFG_LNDPRT		4	// Orientation interrupt pin select
#define	INT_CFG_PULSE		3	// Accel pulse interrupt pin select
#define	INT_CFG_FFMT		2	// Freefall/motion interrupt pin select
#define	INT_CFG_A_VECM		1	// Accel vector-mag interrupt pin select
#define	INT_CFG_DRDY		0	// Data ready interrupt pin select

#define FXO_OFF_X_REG				0x2F
#define FXO_OFF_Y_REG				0x30
#define FXO_OFF_Z_REG				0x31

#define FXO_M_DR_STATUS_REG			0x32

#define FXO_M_OUT_X_REG				0x33
#define FXO_M_OUT_Y_REG				0x35
#define FXO_M_OUT_Z_REG				0x37

#define FXO_CMP_X_REG				0x39
#define FXO_CMP_Y_REG				0x3B
#define FXO_CMP_Z_REG				0x3D

#define FXO_M_OFF_X_REG				0x3F
#define FXO_M_OFF_Y_REG				0x41
#define FXO_M_OFF_Z_REG				0x43

#define FXO_MAX_X_REG				0x45
#define FXO_MAX_Y_REG				0x47
#define FXO_MAX_Z_REG				0x49

#define FXO_MIN_X_REG				0x4B
#define FXO_MIN_Y_REG				0x4D
#define FXO_MIN_Z_REG				0x4F

#define FXO_TEMP_REG				0x50

#define FXO_M_THS_CFG_REG			0x52

/* M_THS_CFG bits */
#define M_THS_ELE			6	// Enable magnetic thresholf event latch
#define M_THS_OAE			5	// Magnetic threshold event logic
								// 0 = logical OR of BELOW threshold flags
								// 1 = logical AND of ABOVE threshold flags
#define M_THS_ZEFE			4	// Enable Z-axis magnetic field above/below threshold flag
#define M_THS_YEFE			3	// Enable Y-axis magnetic field above/below threshold flag
#define M_THS_XEFE			3	// Enable X-axis magnetic field above/below threshold flag
#define M_THS_WAKE_EN		3	// Enable wake on magnetic threshold event
#define M_THS_INT_EN		3	// Enable magnetic threshold interrupt
#define M_THS_INT_CFG		3	// 0 = INT2 pin, 1 = INT1 pin

#define FXO_M_THS_SRC_REG			0x53

/* M_THS_SRC bits */
#define M_THS_EA		7	// One or more magnetic threshold flags have been asserted
#define M_THS_ZHE		5	// Z-high event flag
#define M_THS_ZHP		4	// Z-high event polarity flag
#define M_THS_YHE		3	// Y-high event flag
#define M_THS_YHP		2	// Y-high event polarity flag
#define M_THS_XHE		1	// X-high event flag
#define M_THS_XHP		0	// X-high event polarity flag

#define FXO_M_THS_X_REG				0x54

/* M_THS_X bits */
#define M_THS_DBCNTM		15	// Magnetic thresholf event debounce counter mode
#define M_THS_X14			14	// Magnetic field threshold
#define M_THS_X13			13	//
#define M_THS_X12			12	//
#define M_THS_X11			11	//
#define M_THS_X10			10	//
#define M_THS_X9			9	//
#define M_THS_X8			8	//
#define M_THS_X7			7	//
#define M_THS_X6			6	//
#define M_THS_X5			5	//
#define M_THS_X4			4	//
#define M_THS_X3			3	//
#define M_THS_X2			2	//
#define M_THS_X1			1	//
#define M_THS_X0			0	//

#define FXO_M_THS_Y_REG				0x56
#define FXO_M_THS_Z_REG				0x58
#define FXO_M_THS_COUNT_REG			0x5A

#define FXO_M_CTRL_REG1				0x5B

/* M_CTRL_REG1 bits */
#define M_ACAL				7	// Enable hard-iron auto-calibration
#define M_RST				6	// Enable one-shot magnetic reset (degauss)
#define M_OST				5	// Trigger one-shot magnetic measurement
#define M_OS2				4	// Magnetic oversample ratio (OSR)
#define M_OS1				3	//
#define M_OS0				2	//
#define M_HMS1				1	// Hybrid mode setting
#define M_HMS0				0	//

#define M_HMS_ACCEL_ONLY	0	// Only Accerometer active
#define M_HMS_MAG_ONLY		1	// Only Magnetometer active
#define M_HMS_HYBRID_MODE	3	// Both sensors active

#define FXO_M_CTRL_REG2				0x5C

/* M_CTRL_REG2 bits */
#define HYB_AUTOINC_MODE	5	// Hybrid mode auto-increment setting
#define M_MAXMIN_DIS		4	// Disable magnetic max/min detection function
#define M_MAXMIN_DIS_THS	3	// Disable magnetic max/min detection function upon magnetic threshold event
#define M_MAXMIN_RST		2	// Reset the magnetic max/min registers
#define M_RST_CNT1			1	// Magnetic reset (degauss) frequency
#define M_RST_CNT0			0	//

#define M_RST_ODR			0	// Resets at start of ODR cycle
#define M_RST_16_ODR		1	// Resets every 16 ODR cycles
#define M_RST_512_ODR		2	// Resets every 512 ODR cycles
#define M_RST_DISABLE		3	// Disables automatic magnetic reset

#define FXO_M_CTRL_REG3				0x5D

/* M_CTRL_REG3 bits */
#define M_RAW				7	// Enable magnetic raw data mode
#define M_ASLP_OS2			6	// Magnetic OSR in auto-sleep mode
#define M_ASLP_OS1			5	//
#define M_ASLP_OS0			4	//
#define M_THS_XYZ_UPDATE	3	// Which axis references to update on magnetic threshold events
								// 0 = All axis references are updated
								// 1 = Only the reference for axis that trigger the event is updated

#define FXO_M_INT_SRC_REG			0x5E

/* M_INT_SRC bits */
#define SRC_M_THS			2	// Magnetic threshold event flag
#define SRC_M_VECM			1	// Magnetic vector-mag interrupt flag
#define SRC_M_DRDY			0	// Magnetic Data Ready flag

#define FXO_A_VECM_CFG_REG			0x5F

/* A_VECM_CFG bits */
#define A_VECM_ELE			6	// Enable vector-magnitude event latch
#define A_VECM_INITM		5	// Enable user specified initial vector-magnitude reference
#define A_VECM_UPDM			4	// Always use user specified vector-magnitude value as reference
#define A_VECM_EN			3	// Enable Accelerometer vector-magnitude function

#define FXO_A_VECM_THS_REG			0x60

/* A_VECM_THS bits */
#define A_VECM_DBCNTM		15	// Vector-magnitude event debounce counter mode
#define A_VECM_THS12		12	// Vector-magnitude threshold
#define A_VECM_THS11		11	//
#define A_VECM_THS10		10	//
#define A_VECM_THS9			9	//
#define A_VECM_THS8			8	//
#define A_VECM_THS7			7	//
#define A_VECM_THS6			6	//
#define A_VECM_THS5			5	//
#define A_VECM_THS4			4	//
#define A_VECM_THS3			3	//
#define A_VECM_THS2			2	//
#define A_VECM_THS1			2	//
#define A_VECM_THS0			0	//

#define FXO_A_VECM_CNT_REG			0x62
#define FXO_A_VECM_INITX_REG		0x63
#define FXO_A_VECM_INITY_REG		0x65
#define FXO_A_VECM_INITZ_REG		0x67

#define FXO_M_VECM_CFG_REG			0x69

/* M_VECM_CFG bits */
#define M_VECM_ELE			6	// Enable magnetic vector-magnitude event latch
#define M_VECM_INITM		5	// Enable user specified initial vector-magnitude reference
#define M_VECM_UPDM			4	// Always use user specified vector-magnitude value as reference
#define M_VECM_EN			3	// Enable Magnetic vector-magnitude function
#define M_VECM_WAKE_EN		3	// Enable wake on magnetic vector-mag event
#define M_VECM_INT_EN		3	// Enable magnetic vector_mag interrupt
#define M_VECM_INT_CFG		3	// 0 = INT2 pin, 1 = INT1 pin

#define FXO_M_VECM_THS_REG			0x6A

/* M_VECM_THS bits */
#define M_VECM_DBCNTM		15	// Magnetic vector-magnitude event debounce counter mode
#define M_VECM_THS14		14	// Magnetic vector-magnitude threshold
#define M_VECM_THS13		13	//
#define M_VECM_THS12		12	//
#define M_VECM_THS11		11	//
#define M_VECM_THS10		10	//
#define M_VECM_THS9			9	//
#define M_VECM_THS8			8	//
#define M_VECM_THS7			7	//
#define M_VECM_THS6			6	//
#define M_VECM_THS5			5	//
#define M_VECM_THS4			4	//
#define M_VECM_THS3			3	//
#define M_VECM_THS2			2	//
#define M_VECM_THS1			2	//
#define M_VECM_THS0			0	//

#define FXO_M_VECM_CNT_REG			0x6C
#define FXO_M_VECM_INITX_REG		0x6D
#define FXO_M_VECM_INITY_REG		0x6F
#define FXO_M_VECM_INITZ_REG		0x71

#define FXO_A_FFMT_THS_X_REG		0x73

/* A_FFMT_THS_X bits */
#define A_FFMT_THS_XYZ_EN	15	// Enables ordinate specific Freefall/Motion threshold values
#define A_FFMT_THS_X12		14	// Freefall/Motion X axis threshold
#define A_FFMT_THS_X11		13	//
#define A_FFMT_THS_X10		12	//
#define A_FFMT_THS_X9		11	//
#define A_FFMT_THS_X8		10	//
#define A_FFMT_THS_X7		9	//
#define A_FFMT_THS_X6		8	//
#define A_FFMT_THS_X5		7	//
#define A_FFMT_THS_X4		6	//
#define A_FFMT_THS_X3		5	//
#define A_FFMT_THS_X2		4	//
#define A_FFMT_THS_X1		3	//
#define A_FFMT_THS_X0		2	//

#define FXO_A_FFMT_THS_Y_REG		0x75

/* A_FFMT_THS_Y bits */
#define A_FFMT_TRANS_THS_EN	15	// Enables things
#define A_FFMT_THS_Y12		14	// Freefall/Motion Y axis threshold
#define A_FFMT_THS_Y11		13	//
#define A_FFMT_THS_Y10		12	//
#define A_FFMT_THS_Y9		11	//
#define A_FFMT_THS_Y8		10	//
#define A_FFMT_THS_Y7		9	//
#define A_FFMT_THS_Y6		8	//
#define A_FFMT_THS_Y5		7	//
#define A_FFMT_THS_Y4		6	//
#define A_FFMT_THS_Y3		5	//
#define A_FFMT_THS_Y2		4	//
#define A_FFMT_THS_Y1		3	//
#define A_FFMT_THS_Y0		2	//

#define FXO_A_FFMT_THS_Z_REG		0x77

/* A_FFMT_THS_Z bits */
#define A_FFMT_TRANS_THS_EN	15	// Enables things
#define A_FFMT_THS_Z12		14	// Freefall/Motion Z axis threshold
#define A_FFMT_THS_Z11		13	//
#define A_FFMT_THS_Z10		12	//
#define A_FFMT_THS_Z9		11	//
#define A_FFMT_THS_Z8		10	//
#define A_FFMT_THS_Z7		9	//
#define A_FFMT_THS_Z6		8	//
#define A_FFMT_THS_Z5		7	//
#define A_FFMT_THS_Z4		6	//
#define A_FFMT_THS_Z3		5	//
#define A_FFMT_THS_Z2		4	//
#define A_FFMT_THS_Z1		3	//
#define A_FFMT_THS_Z0		2	//

#define ACCEL_AXIS_MASK		0x3FFF
#define FFMT_THS_AXIS_MASK	0x1FFF
#define PULSE_THS_AXIS_MASK	0x7F

#define MOTION_INT1_PIN 	6
#define MOTION_INT2_PIN		8

#define MOTION_INT_POLARITY NRF_GPIOTE_POLARITY_HITOLO

uint8_t fxo_get_device_id();
void fxo_init();
void fxo_get_accel(int16_t *x, int16_t *y, int16_t *z);
void fxo_soft_reset();


#endif /* SENSORS_FXOS8700CQ_H_ */
