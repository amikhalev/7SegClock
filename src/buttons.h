/**
 * @file buttons.h
 * @author Alex Mikhalev
 * @date Jul 1, 2015
 *
 * Allows for reading the state of the buttons
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

/************
 * Includes *
 ************/

#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>

/*************
 * Constants *
 *************/

#define NUM_BUTTONS			 	(4)

#define BUTTONS_PRESSED_TICKS	100
#define BUTTONS_HELD_TICKS		1000

/**********************
 * Pin/port constants *
 **********************/

#define BUTTON0_DDR				DDRB
#define BUTTON0_PORT			PORTB
#define BUTTON0_PIN				PINB
#define BUTTON0_BIT				_BV(PINB2)

#define BUTTON1_DDR				DDRB
#define BUTTON1_PORT			PORTB
#define BUTTON1_PIN				PINB
#define BUTTON1_BIT				_BV(PINB3)

#define BUTTON2_DDR				DDRB
#define BUTTON2_PORT			PORTB
#define BUTTON2_PIN				PINB
#define BUTTON2_BIT				_BV(PINB1)

#define BUTTON3_DDR				DDRB
#define BUTTON3_PORT			PORTB
#define BUTTON3_PIN				PINB
#define BUTTON3_BIT				_BV(PINB4)

/*********
 * Types *
 *********/

typedef uint8_t buttons_state_t;

/********************
 * Global variables *
 ********************/

buttons_state_t buttons_current_state;
buttons_state_t buttons_last_state;
uint16_t button_counters[NUM_BUTTONS];

/********************
 * Public functions *
 ********************/

/**
 * Initializes the buttons, including setting up global variables and inputs.
 */
void buttons_init();

/**
 * Updates the state of the buttons.
 */
void buttons_update();

/**
 * Checks if a button is currently depressed.
 * @param button The button number, in the range [0, NUM_BUTTONS)
 * @return A boolean indicating if the button is down.
 */
bool button_is_down(uint8_t button);

/**
 * Checks if a button has been pressed.
 * @param button The button number, in the range [0, NUM_BUTTONS)
 * @return True for one "cycle" when the button is pressed.
 */
bool button_pressed(uint8_t button);

/**
 * Checks if a button was held.
 * @param button The button number, in the range [0, NUM_BUTTONS)
 * @return True for one "cycle" when the button was held.
 */
bool button_held(uint8_t button);

bool button_repeated(uint8_t button);

#endif /* BUTTONS_H_ */
