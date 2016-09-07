/*
 * ble.h
 *
 *  Created on: Aug 22, 2016
 *      Author: matt
 */

#ifndef BLE_UTIL_H_
#define BLE_UTIL_H_

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <nordic_common.h>
#include <nrf.h>
#include <app_error.h>
#include <app_timer.h>
#include <ble.h>
#include <ble_hci.h>
#include <ble_srv_common.h>
#include <ble_advdata.h>
#include <ble_advertising.h>
#include <ble_conn_params.h>
#include <softdevice_handler.h>
#include "pstorage_platform.h"
#include <pstorage.h>
#include <nrf_gpio.h>
#include <ble_hci.h>
#include <ble_advdata.h>
#include <ble_advertising.h>

#define IS_SRVC_CHANGED_CHARACT_PRESENT  0                                          /**< Include or not the service_changed characteristic. if not enabled, the server's database cannot be changed for the lifetime of the device*/

#define CENTRAL_LINK_COUNT               0                                          /**< Number of central links used by the application. When changing this number remember to adjust the RAM settings*/
#define PERIPHERAL_LINK_COUNT            1                                          /**< Number of peripheral links used by the application. When changing this number remember to adjust the RAM settings*/

#define APP_ADV_INTERVAL                 300                                        /**< The advertising interval (in units of 0.625 ms. This value corresponds to 25 ms). */
#define APP_ADV_TIMEOUT_IN_SECONDS       10                                         /**< The advertising timeout in units of seconds. */

#define APP_TIMER_PRESCALER              0                                          /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_OP_QUEUE_SIZE          4                                          /**< Size of timer operation queues. */

#define MIN_CONN_INTERVAL                MSEC_TO_UNITS(100, UNIT_1_25_MS)           /**< Minimum acceptable connection interval (0.1 seconds). */
#define MAX_CONN_INTERVAL                MSEC_TO_UNITS(200, UNIT_1_25_MS)           /**< Maximum acceptable connection interval (0.2 second). */
#define SLAVE_LATENCY                    0                                          /**< Slave latency. */
#define CONN_SUP_TIMEOUT                 MSEC_TO_UNITS(4000, UNIT_10_MS)            /**< Connection supervisory timeout (4 seconds). */

#define FIRST_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER) /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY    APP_TIMER_TICKS(30000, APP_TIMER_PRESCALER)/**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT     3                                          /**< Number of attempts before giving up the connection parameter negotiation. */

#define DEAD_BEEF                        0xDEADBEEF                                 /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

/*#define NRF_CLOCK_LFCLKSRC      {.source        = NRF_CLOCK_LF_SRC_XTAL,            \
//                                 .rc_ctiv       = 0,                                \
//                                 .rc_temp_ctiv  = 0,                                \
                                 .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM}

//#define NRF_CLOCK_LFCLKSRC		{	.source = NRF_CLOCK_LF_SRC_RC,	\
//									.rc_ctiv = 4,					\
//									.rc_temp_ctiv = 1,				\
//									.xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_250_PPM	\
//								} */

#define DEVICE_NAME                    	"Celsian"                          		/**< Name of device. Will be included in the advertising data. */
#define MANUFACTURER_NAME              	"Matt Sutter"                      		/**< Manufacturer. Will be passed to Device Information Service. */

// Unique to Celsian	 				2D040000-779E-4EFA-9E1B-351935C016DC
#define CELSIAN_BASE_UUID 				{{0xDC, 0x16, 0xC0, 0x35, 0x19, 0x35, 0x1B, 0x9E, 0xFA, 0x4E, 0x9E, 0x77, 0x00, 0x00, 0x04, 0x2D}}

// Celsian BLE GATT Service UUID
#define CELSIAN_SERVICE_UUID			0x0001
#define CELSIAN_SERVICE_UUID_TYPE		BLE_UUID_TYPE_VENDOR_BEGIN

// Celsian BLE GATT Characteristic UUIDs
#define CELSIAN_MPL_TEMP_CHAR_UUID		0x0002
#define CELSIAN_SHT_TEMP_CHAR_UUID		0x0003
#define CELSIAN_RH_CHAR_UUID			0x0004
#define CELSIAN_PRES_CHAR_UUID			0x0005
#define CELSIAN_UVA_CHAR_UUID			0x0006
#define CELSIAN_UVB_CHAR_UUID			0x0007
#define CELSIAN_UVD_CHAR_UUID			0x0008
#define CELSIAN_UVCOMP1_CHAR_UUID		0x0009
#define CELSIAN_UVCOMP2_CHAR_UUID		0x000A

#define UNITLESS			0x2700
#define UNITS_DEGREES_C		0x272F
#define UNITS_PASCALS		0x2724
#define UNITS_PERCENT		0x27AD

typedef union {
	uint8_t data[8];
	double 	value;
} ble_double_t;

typedef struct
{
	uint8_t *p_mpl_temp;
	uint8_t *p_sht_temp;
	uint8_t *p_rh;
	uint8_t *p_pres;
	uint8_t *p_uva;
	uint8_t *p_uvb;
	uint8_t *p_uvd;
	uint8_t *p_uvcomp1;
	uint8_t *p_uvcomp2;

} ble_char_value_handles_t;

typedef void (*ble_callback_t)();

typedef struct
{
	ble_callback_t on_adv_idle;
	ble_callback_t on_adv_start;
	ble_callback_t on_connect;
	ble_callback_t on_disconnect;
} ble_callbacks_t;

void ble_init(ble_callbacks_t *event_callbacks, ble_char_value_handles_t values);
void ble_start_adv();
void ble_disconnect();

#endif /* BLE_UTIL_H_ */
