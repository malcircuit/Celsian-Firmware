/*
 * leds.c
 *
 *  Created on: Jul 31, 2016
 *      Author: matt
 */

#include "leds.h"

static const uint8_t leds_list[LEDS_NUMBER] = {G_LED_PIN, R_LED_PIN};

void leds_init()
{
	nrf_gpio_cfg_output(R_LED_PIN);
	nrf_gpio_cfg_output(G_LED_PIN);

	nrf_gpio_pin_set(R_LED_PIN);
	nrf_gpio_pin_set(G_LED_PIN);
}

void led_toggle(unsigned led)
{
	nrf_gpio_pin_toggle(leds_list[led & 1]);
}

void led_clear(unsigned led)
{
	nrf_gpio_pin_set(leds_list[led & 1]);
}

void led_set(unsigned led)
{
	nrf_gpio_pin_clear(leds_list[led & 1]);
}
