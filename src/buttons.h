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

/*****************
 * Helper macros *
 *****************/

#define __BUTTONS_EMTPY(...)
#define __BUTTONS_DEFER(...) __VA_ARGS__ EMPTY()
#define __BUTTONS_OBSTRUCT(...) __VA_ARGS__ DEFER(EMPTY)()
#define __BUTTONS_EXPAND(...) __VA_ARGS__
#define __BUTTONS_CAT(a, ...) __BUTTONS_PRIMITIVE_CAT(a, __VA_ARGS__)
#define __BUTTONS_PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__

#define BUTTON_DDR(button_name)  __BUTTONS_CAT(DDR, __BUTTONS_EXPAND(button_name##_REG))
#define BUTTON_PORT(button_name) __BUTTONS_CAT(PORT, __BUTTONS_EXPAND(button_name##_REG))
#define BUTTON_PIN(button_name)  __BUTTONS_CAT(PIN, __BUTTONS_EXPAND(button_name##_REG))
#define BUTTON_BIT(button_name)  _BV(__BUTTONS_EXPAND(button_name##_NUM))

#define BUTTON_READ(button_name) (BUTTON_PIN(button_name) & BUTTON_BIT(button_name))
#define BUTTON_SET(button_name, type) __BUTTONS_EXPAND(BUTTON_ ## type(button_name)) |= BUTTON_BIT(button_name)
#define BUTTON_RESET(button_name, type) __BUTTONS_EXPAND(BUTTON_ ## type(button_name)) &= ~BUTTON_BIT(button_name)

/**********************
 * Pin/port constants *
 **********************/

#define BUTTON0_REG				B
#define BUTTON0_NUM				2

#define BUTTON1_REG				B
#define BUTTON1_NUM				3

#define BUTTON2_REG				B
#define BUTTON2_NUM				1

#define BUTTON3_REG				B
#define BUTTON3_NUM				4

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
