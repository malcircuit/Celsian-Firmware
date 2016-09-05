/*
 * debug.c
 *
 *  Created on: Jul 31, 2016
 *      Author: matt
 */

#include "debug.h"

static app_uart_comm_params_t const comm_params =
{
		UART0_CONFIG_PSEL_RXD,
		UART0_CONFIG_PSEL_TXD,
		UART0_CONFIG_PSEL_RTS,
		UART0_CONFIG_PSEL_CTS,
		APP_UART_FLOW_CONTROL_DISABLED,
		false,
		UART_BAUDRATE_BAUDRATE_Baud9600
};

static void uart_event_handler(app_uart_evt_t * p_event)
{
    switch (p_event->evt_type)
    {
        case APP_UART_COMMUNICATION_ERROR:
            APP_ERROR_HANDLER(p_event->data.error_communication);
            break;

        case APP_UART_FIFO_ERROR:
            APP_ERROR_HANDLER(p_event->data.error_code);
            break;

        default:
            break;
    }
}

void debug_init()
{
	ret_code_t err_code = 0;
	APP_UART_INIT(&comm_params,
			uart_event_handler,
			UART0_CONFIG_IRQ_PRIORITY,
			err_code);
	APP_ERROR_CHECK(err_code);
}

static void uart_send(char *data, uint8_t length)
{
    uint8_t i = 0;
	do{
		while(app_uart_put(data[i]) != NRF_SUCCESS);
		i++;
	}while(i < length && data[i] != 0);
}

void debug_printf(const char *template, ...)
{
	va_list va_lst;
	va_start(va_lst, template);
	char buffer[UART_BUFFER_SIZE] = {0};
	int char_num = 0;

	char_num = vsnprintf(buffer, UART_BUFFER_SIZE, template, va_lst);
	va_end(va_lst);
	uart_send(buffer, char_num);
}
