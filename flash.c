/*
 * flash.c
 *
 *  Created on: Jul 31, 2016
 *      Author: matt
 */

#include "flash.h"

static const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE);

static const uint8_t spi_tx_buf[SPI_TX_BUF_LEN] = {0x9F};
static uint8_t spi_rx_buf[SPI_RX_BUF_LEN] = {0};
static volatile uint8_t spi_rx_is_ready = false;

static void spi_event_handler(nrf_drv_spi_evt_t const * p_event)
{
	switch(p_event->type)
	{
	case NRF_DRV_SPI_EVENT_DONE:
		spi_rx_is_ready = true;
		break;

	default:
		break;
	}
}

void flash_init()
{
	nrf_drv_spi_config_t spi_config = {
			.sck_pin		= SPI1_CONFIG_SCK_PIN,
		    .mosi_pin		= SPI1_CONFIG_MOSI_PIN,
		    .miso_pin		= SPI1_CONFIG_MISO_PIN,
			.ss_pin 		= SPI_CS_PIN,
		    .irq_priority	= SPI1_CONFIG_IRQ_PRIORITY,
		    .orc			= 0x00,
			.frequency 		= NRF_DRV_SPI_FREQ_125K,
			.mode 			= NRF_DRV_SPI_MODE_0,
		    .bit_order		= NRF_DRV_SPI_BIT_ORDER_MSB_FIRST
	};
	APP_ERROR_CHECK(nrf_drv_spi_init(&spi, &spi_config, spi_event_handler));

	// Enable pullup on MISO
	nrf_gpio_cfg_input(SPI1_CONFIG_MISO_PIN, NRF_GPIO_PIN_PULLUP);
}

void flash_get_id_bytes()
{
	APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, spi_tx_buf, SPI_TX_BUF_LEN, spi_rx_buf, 6));
}

void flash_print_id_bytes()
{
	if (spi_rx_is_ready)
	{
		debug_printf("Manufacturer ID: %.2X\n", spi_rx_buf[1]);
		debug_printf("Device ID: %.4X\n", ((uint16_t)(spi_rx_buf[2]) << 8) | spi_rx_buf[3]);
		debug_printf("EDI length: %.2X\n", spi_rx_buf[4]);
		debug_printf("EDI byte 1: %.2X\n", spi_rx_buf[5]);

		spi_rx_is_ready = false;
	}
}

void flash_print_id()
{
	flash_get_id_bytes();
	nrf_delay_ms(1);
	flash_print_id_bytes();
}
