/*
 * debug.h
 *
 *  Created on: Jul 31, 2016
 *      Author: matt
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdbool.h>
#include <stdarg.h>
#include <nrf52_bitfields.h>
#include <app_error.h>
#include <app_util_platform.h>
#include <app_uart.h>
#include "nrf_drv_config.h"

#define UART_BUFFER_SIZE 51

void debug_init();
void debug_printf(const char *template, ...);

#endif /* DEBUG_H_ */
