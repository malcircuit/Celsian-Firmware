/*
 * util.c
 *
 *  Created on: Aug 8, 2016
 *      Author: matt
 */

#include "util.h"

extern inline void NOP();
extern inline unsigned bts(uint8_t bit_pos);
extern inline unsigned btc(uint8_t bit_pos);
extern inline uint8_t set_byte_bits(uint8_t bite, uint8_t count, ...);
extern inline uint8_t clr_byte_bits(uint8_t bite, uint8_t count, ...);
extern inline uint16_t set_word_bits(uint16_t word, uint8_t count, ...);
extern inline uint16_t clr_word_bits(uint16_t word, uint8_t count, ...);

