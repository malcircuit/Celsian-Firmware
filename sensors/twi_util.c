/*
 * twi_util.c
 *
 *  Created on: Jul 31, 2016
 *      Author: matt
 */

#include "twi_util.h"

static app_twi_t twi_instance = APP_TWI_INSTANCE(TWI0_INSTANCE_INDEX);

static uint8_t is_twi_init = false;

void twi_init()
{
	if (!is_twi_init)
	{
		ret_code_t err_code = 0;
		nrf_drv_twi_config_t config = {
				.scl = TWI0_CONFIG_SCL,
				.sda = TWI0_CONFIG_SDA,
				.frequency = TWI0_CONFIG_FREQUENCY,
				.interrupt_priority = TWI0_CONFIG_IRQ_PRIORITY
		};
		APP_TWI_INIT(&twi_instance, &config, MAX_QUEUE_LENGTH, err_code);
		APP_ERROR_CHECK(err_code);

		is_twi_init = true;
	}
}


uint8_t twi_read_status(uint8_t address)
{
	if(!is_twi_init)
	{
		twi_init();
	}

	uint8_t data = 0;
    app_twi_transfer_t transfer[] = {
			APP_TWI_READ(address, &data, 1, 0)
    };

	APP_ERROR_CHECK(app_twi_perform(&twi_instance, transfer, 1, NULL));
	return data;
}

void twi_write_command(uint8_t address, uint8_t command)
{
	if(!is_twi_init)
	{
		twi_init();
	}

    app_twi_transfer_t transfer[] = {
    		APP_TWI_WRITE(address, &command, 1, 0)
    };

	APP_ERROR_CHECK(app_twi_perform(&twi_instance, transfer, 1, NULL));
}

uint8_t twi_read_reg_byte(uint8_t address, uint8_t reg)
{
	if(!is_twi_init)
	{
		twi_init();
	}

	uint8_t data = 0;
    app_twi_transfer_t transfer[] = {
    		APP_TWI_WRITE(address, &reg, 1, APP_TWI_NO_STOP),
			APP_TWI_READ(address, &data, 1, 0)
    };

	APP_ERROR_CHECK(app_twi_perform(&twi_instance, transfer, 2, NULL));
	return data;
}

// TODO: Verify that this works
void twi_write_byte(uint8_t address, uint8_t reg, uint8_t data)
{
	if(!is_twi_init)
	{
		twi_init();
	}

	uint8_t bytes[] = {reg, data};

	app_twi_transfer_t transfer[] = {
			APP_TWI_WRITE(address, bytes, 2, 0),
	};

	APP_ERROR_CHECK(app_twi_perform(&twi_instance, transfer, 1, NULL));
}

// TODO: Verify that this works
//void twi_write_bytes(uint8_t address, uint8_t reg, uint8_t *data, uint8_t length)
//{
//	if(!is_twi_init)
//	{
//		twi_init();
//	}
//
//	uint8_t *bytes = (uint8_t *)malloc(length + 1);
//	bytes[0] = reg;
//
//	for (int i = 0; i < length; i++)
//	{
//		bytes[1 + i] = data[i];
//	}
//
//	app_twi_transfer_t transfer[] = {
//			APP_TWI_WRITE(address, bytes, length, 0),
//	};
//
//	APP_ERROR_CHECK(app_twi_perform(&twi_instance, transfer, 1, NULL));
//}

void twi_write_word(uint8_t address, uint8_t reg, uint8_t *word)
{
	if(!is_twi_init)
	{
		twi_init();
	}

	uint8_t bytes[3] = {reg, word[0], word[1]};

	app_twi_transfer_t transfer[] = {
			APP_TWI_WRITE(address, bytes, 3, 0),
	};

	APP_ERROR_CHECK(app_twi_perform(&twi_instance, transfer, 1, NULL));
}

void twi_read_reg_bytes(uint8_t address, uint8_t reg, uint8_t *data, uint8_t length)
{
	if(!is_twi_init)
	{
		twi_init();
	}

	if (length > 0)
	{
		app_twi_transfer_t transfer[] = {
				APP_TWI_WRITE(address, &reg, 1, APP_TWI_NO_STOP),
				APP_TWI_READ(address, data, length, 0)
		};

		APP_ERROR_CHECK(app_twi_perform(&twi_instance, transfer, 2, NULL));
	}
}

void twi_read_bytes(uint8_t address, uint8_t *data, uint8_t length)
{
	if(!is_twi_init)
	{
		twi_init();
	}

	if (length > 0)
	{
		app_twi_transfer_t transfer[] = {
				APP_TWI_READ(address, data, length, 0)
		};

		APP_ERROR_CHECK(app_twi_perform(&twi_instance, transfer, 1, NULL));
	}
}

uint16_t twi_read_word(uint8_t address, uint8_t reg)
{
	uint8_t data[2] = {0};
	twi_read_reg_bytes(address, reg, data, 2);

	return (data[0] << 8) | data[1];
}

void twi_read_word_crc(uint8_t address, uint8_t reg, uint16_t *word, uint8_t *checksum)
{
	uint8_t data[3] = {0};
	twi_read_reg_bytes(address, reg, data, 3);

	*word = (data[0] << 8) | data[1];
	*checksum = data[2];
}
