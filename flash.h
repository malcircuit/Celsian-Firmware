/*
 * flash.h
 *
 *  Created on: Jul 31, 2016
 *      Author: matt
 */

#ifndef FLASH_H_
#define FLASH_H_

#include <stdbool.h>
#include <app_error.h>
#include <app_util_platform.h>
#include <nrf_drv_spi.h>
#include <nrf_gpio.h>
#include <nrf51_to_nrf52.h>
#include <nrf52.h>
#include <nrf_delay.h>
#include "debug.h"

#define SPI_CS_PIN	15
#define SPI_INSTANCE  1
#define SPI_RX_BUF_LEN 11
#define SPI_TX_BUF_LEN 1

void flash_init();
void flash_get_id_bytes();
void flash_print_id_bytes();
void flash_print_id();

#endif /* FLASH_H_ */
