/*
 * util.h
 *
 *  Created on: Aug 8, 2016
 *      Author: matt
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <nrf52.h>


inline void NOP()
{
	__ASM volatile (" NOP\n");
}

inline unsigned bts(uint8_t bit_pos)
{
	return (1UL << bit_pos);
}

inline unsigned btc(uint8_t bit_pos)
{
	return ~(1UL << bit_pos);
}

inline uint8_t set_byte_bits(uint8_t bite, uint8_t count, ...)
{
	va_list va_lst;
	va_start(va_lst, count);

	for (uint8_t i = 0; i < count; i++)
	{
		bite |= bts(va_arg(va_lst, int));
	}

	return bite;

	va_end(va_lst);
}

inline uint8_t clr_byte_bits(uint8_t bite, uint8_t count, ...)
{
	va_list va_lst;
	va_start(va_lst, count);

	for (uint8_t i = 0; i < count; i++)
	{
		bite &= btc(va_arg(va_lst, int));
	}

	return bite;

	va_end(va_lst);
}

inline uint16_t set_word_bits(uint16_t word, uint8_t count, ...)
{
	va_list va_lst;
	va_start(va_lst, count);

	for (uint8_t i = 0; i < count; i++)
	{
		word |= bts(va_arg(va_lst, int));
	}

	return word;

	va_end(va_lst);
}

inline uint16_t clr_word_bits(uint16_t word, uint8_t count, ...)
{
	va_list va_lst;
	va_start(va_lst, count);

	for (uint8_t i = 0; i < count; i++)
	{
		word &= btc(va_arg(va_lst, int));
	}

	return word;

	va_end(va_lst);
}


#endif /* UTIL_H_ */
