/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/**
 *
 *
 * The NFCPINS.PROTECT bit in the UICR register must be set to 0 to
 * allow the MISO line to be read correctly.
 *
 */
#include <nrf.h>
#include "util.h"
#include "sensors.h"
#include "debug.h"
#include "leds.h"
#include "flash.h"
#include "ble_util.h"
#include <nrf_delay.h>

#define WAKE_FROM_GPIO_EVENT 	0x00010000
#define CLEAR_RESETREAS 		0xFFFFFFFF

volatile bool pulse_detected = false, is_adv = false, is_connected = false;

void check_for_wake_from_gpio();
void init_interrupts();

void on_pulse(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
	pulse_detected = true;
}

void on_ble_idle()
{
	is_adv = false;
	led_clear(G_LED);
	sensors_low_power();

	debug_printf("BLE is idle, shutting down\n");
    APP_ERROR_CHECK(sd_power_system_off());
}

void on_ble_adv_start()
{
	is_adv = true;
	debug_printf("BLE advertising started\n");
}

void on_ble_connect()
{
	is_adv = false;
	is_connected = true;
	led_set(G_LED);
	debug_printf("BLE connected\n");
}

void on_ble_disconnect()
{
	is_connected = false;
	ble_start_adv();
	debug_printf("BLE disconnected\n");
}

/**
 * @brief Function for application main entry.
 */
int main(void)
{
	double sht20_temp = 0, rh = 0, pressure = 0, mpl_temp = 0, uv_index;
	uint16_t uva = 0, uvb = 0, uvd = 0, uvcomp1 = 0, uvcomp2 = 0;

	ble_char_value_handles_t values = {
			.p_sht_temp 	= (uint8_t *)&sht20_temp,
			.p_mpl_temp 	= (uint8_t *)&mpl_temp,
			.p_rh 			= (uint8_t *)&rh,
			.p_pres 		= (uint8_t *)&pressure,
			.p_uva 			= (uint8_t *)&uva,
			.p_uvb 			= (uint8_t *)&uvb,
			.p_uvd 			= (uint8_t *)&uvd,
			.p_uvcomp1 		= (uint8_t *)&uvcomp1,
			.p_uvcomp2 		= (uint8_t *)&uvcomp2,
	};

	ble_callbacks_t callbacks = {
			.on_adv_idle = on_ble_idle,
			.on_adv_start = on_ble_adv_start,
			.on_connect = on_ble_connect,
			.on_disconnect = on_ble_disconnect
	};

	ble_init(&callbacks, values);
	debug_init();
	sensors_init();
	flash_init();
	leds_init();
	fxo_init();
	init_interrupts();

	debug_printf("\nMPL ID: 0x%02X\n", mpl_get_device_id());
	debug_printf("FXO ID: 0x%02X\n", fxo_get_device_id());
	debug_printf("VEML ID: 0x%04X\n", veml_get_device_id());

	flash_print_id();

	check_for_wake_from_gpio();

	while (true)
	{
		if (pulse_detected && !is_adv && !is_connected)
		{
			debug_printf("Pulse Detected\n");
			is_adv = true;
			ble_start_adv();
			pulse_detected = false;
		}

		if (is_adv)
		{
			led_toggle(G_LED);

			nrf_delay_ms(500);
		}

		if (is_connected)
		{
			sht20_temp = sht20_get_temp();
			rh = sht20_get_rh();
			debug_printf("SHT T: %d F\t RH: %d %%\n",(int)(sht20_temp*1.8 + 32), (int)rh);

			mpl_oneshot_block(&pressure, &mpl_temp);
			debug_printf("MPL T: %d F\t P: %d mb\n", (int)(mpl_temp*1.8 +32), (int)(pressure/100));

			uv_index = veml_get_uv_index(&uva, &uvb, &uvd, &uvcomp1, &uvcomp2);
			debug_printf("UVI: %d\n", (int)uv_index);

			nrf_delay_ms(500);
		}
	}
}

void check_for_wake_from_gpio()
{
	uint32_t reset_reason = 0;

	APP_ERROR_CHECK(sd_power_reset_reason_get(&reset_reason));

	if ((reset_reason & WAKE_FROM_GPIO_EVENT) == WAKE_FROM_GPIO_EVENT)		// Reset caused by GPIO interrupt
	{
		pulse_detected = true;
		APP_ERROR_CHECK(sd_power_reset_reason_clr(WAKE_FROM_GPIO_EVENT));
	}
	else if (reset_reason == 0)												// Power-on-Reset
	{
		pulse_detected = true;
	}
	else
	{
		APP_ERROR_CHECK(sd_power_reset_reason_clr(CLEAR_RESETREAS));
	    APP_ERROR_CHECK(sd_power_system_off());
	}
}

void init_interrupts()
{
	const nrf_drv_gpiote_in_config_t motion_int_cfg = {
			.sense = MOTION_INT_POLARITY,
			.pull = NRF_GPIO_PIN_NOPULL,
			.is_watcher = false,
			.hi_accuracy = false
	};

	if (!nrf_drv_gpiote_is_init())
	{
		APP_ERROR_CHECK(nrf_drv_gpiote_init());
	}

	APP_ERROR_CHECK(nrf_drv_gpiote_in_init(MOTION_INT1_PIN, &motion_int_cfg, on_pulse));
	nrf_drv_gpiote_in_event_enable(MOTION_INT1_PIN, true);
}

/** @} */
