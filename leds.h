/*
 * leds.h
 *
 *  Created on: Jul 31, 2016
 *      Author: matt
 */

#ifndef LEDS_H_
#define LEDS_H_

#include <nrf_gpio.h>

#define LEDS_NUMBER 2
#define R_LED_PIN 7
#define G_LED_PIN 11

#define R_LED 0
#define G_LED 1

void leds_init();
void led_toggle(unsigned led);
void led_clear(unsigned led);
void led_set(unsigned led);

#endif /* LEDS_H_ */
