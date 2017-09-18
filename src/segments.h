/**
 * @file segments.h
 * @author Alex Mikhalev
 * @date June 15, 2015
 *
 * Provides support for drawing to a 4 digit 7-segment display
 */

#ifndef SEGMENTS_H_
#define SEGMENTS_H_

/************
 * Includes *
 ************/

#include <stdint.h>
#include <avr/io.h>

/*************
 * Constants *
 *************/

#define SEGMENTS_NUM_DIGITS 	(4)
#define SEGMENTS_NUM_PARTS 		(SEGMENTS_NUM_DIGITS + 1)

#define SEGMENTS_DOTS_COLON		0b00000011
#define SEGMENTS_DOTS_DEGREES	0b00000100

/**********************
 * Pin/port constants *
 **********************/

#define SEGMENTS_SEGMENTS_PORT	PORTD
#define SEGMENTS_SEGMENTS_DDR	DDRD
#define SEGMENTS_SEGMENTS_BITS 	0b01111111

#define SEGMENTS_PART0_DDR		DDRC
#define SEGMENTS_PART0_PORT		PORTC
#define SEGMENTS_PART0_BIT		_BV(PORTC3)

#define SEGMENTS_PART1_DDR		DDRC
#define SEGMENTS_PART1_PORT		PORTC
#define SEGMENTS_PART1_BIT		_BV(PORTC2)

#define SEGMENTS_PART2_DDR		DDRC
#define SEGMENTS_PART2_PORT		PORTC
#define SEGMENTS_PART2_BIT		_BV(PORTC1)

#define SEGMENTS_PART3_DDR		DDRC
#define SEGMENTS_PART3_PORT		PORTC
#define SEGMENTS_PART3_BIT		_BV(PORTC0)

#define SEGMENTS_PART4_DDR		DDRB
#define SEGMENTS_PART4_PORT		PORTB
#define SEGMENTS_PART4_BIT		_BV(PORTB0)

/*********
 * Types *
 *********/

struct segments {
	union {
		uint8_t raw_parts[SEGMENTS_NUM_PARTS];
		struct {
			uint8_t digits[SEGMENTS_NUM_DIGITS];
			uint8_t dots;
		};
	};
};

struct segments_draw_flags {
	uint8_t current_part :3;
};

/********************
 * Global variables *
 ********************/

struct segments segments;
struct segments_draw_flags segments_draw_flags;

/********************
 * Public functions *
 ********************/

/**
 * Initializes the segments functionality, including initializing all global
 * variables and setting up outputs.
 */
void segments_init();

/**
 * Sets a digit in the segments
 * @param digit The digit to set, in the range [0, @c SEGMENTS_NUM_DIGITS)
 * @param number The number in range [0, 9] to set the digit to.
 */
void segments_set_digit(uint8_t digit, uint8_t number);

/**
 * Draws the digits to the segments.
 */
void segments_draw();

#endif /* SEGMENTS_H_ */
