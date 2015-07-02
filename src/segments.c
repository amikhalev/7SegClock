#include <segments.h>

#include <string.h>
#include <assert.h>

void segments_init() {
	for (size_t i = 0; i < SEGMENTS_NUM_PARTS; i++) {
		segments.raw_parts[i] = 0x0;
	}
	segments_draw_flags.current_part = SEGMENTS_NUM_PARTS - 1;
	SEGMENTS_SEGMENTS_DDR |= SEGMENTS_SEGMENTS_BITS;
	SEGMENTS_PART0_DDR |= SEGMENTS_PART0_BIT;
	SEGMENTS_PART1_DDR |= SEGMENTS_PART1_BIT;
	SEGMENTS_PART2_DDR |= SEGMENTS_PART2_BIT;
	SEGMENTS_PART3_DDR |= SEGMENTS_PART3_BIT;
	SEGMENTS_PART4_DDR |= SEGMENTS_PART4_BIT;
}

//  0
// 6 1
//  7
// 5 2
//  3

static uint8_t digit_to_segment[10] = { 0b00111111, // 0
		0b00000110, // 1
		0b01011011, // 2
		0b01001111, // 3
		0b01010110, // 4
		0b01101101, // 5
		0b01111101, // 6
		0b00000111, // 7
		0b01111111, // 8
		0b01101111  // 9
		};

void segments_set_digit(uint8_t digit, uint8_t number) {
	assert(digit < SEGMENTS_NUM_DIGITS);
	assert(number <= 9);

	segments.digits[digit] = digit_to_segment[number];
}

void segments_draw() {
	switch (segments_draw_flags.current_part) {
	case 0:
		SEGMENTS_PART0_PORT &= ~SEGMENTS_PART0_BIT;
		break;
	case 1:
		SEGMENTS_PART1_PORT &= ~SEGMENTS_PART1_BIT;
		break;
	case 2:
		SEGMENTS_PART2_PORT &= ~SEGMENTS_PART2_BIT;
		break;
	case 3:
		SEGMENTS_PART3_PORT &= ~SEGMENTS_PART3_BIT;
		break;
	case 4:
		SEGMENTS_PART4_PORT &= ~SEGMENTS_PART4_BIT;
		break;
	}
	segments_draw_flags.current_part++;
	if (segments_draw_flags.current_part >= SEGMENTS_NUM_PARTS) {
		segments_draw_flags.current_part = 0;
	}
	SEGMENTS_SEGMENTS_PORT &= ~SEGMENTS_SEGMENTS_BITS;
	SEGMENTS_SEGMENTS_PORT |= SEGMENTS_SEGMENTS_BITS
			& segments.raw_parts[segments_draw_flags.current_part];
	switch (segments_draw_flags.current_part) {
	case 0:
		SEGMENTS_PART0_PORT |= SEGMENTS_PART0_BIT;
		break;
	case 1:
		SEGMENTS_PART1_PORT |= SEGMENTS_PART1_BIT;
		break;
	case 2:
		SEGMENTS_PART2_PORT |= SEGMENTS_PART2_BIT;
		break;
	case 3:
		SEGMENTS_PART3_PORT |= SEGMENTS_PART3_BIT;
		break;
	case 4:
		SEGMENTS_PART4_PORT |= SEGMENTS_PART4_BIT;
		break;
	}
}
