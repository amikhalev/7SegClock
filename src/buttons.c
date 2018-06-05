#include <buttons.h>

#include <assert.h>

void buttons_init() {
	buttons_current_state = 0;
	buttons_last_state = 0;
	for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
		button_counters[i] = 0;
	}
	BUTTON_RESET(BUTTON0, DDR);
	BUTTON_RESET(BUTTON1, DDR);
	BUTTON_RESET(BUTTON2, DDR);
	BUTTON_RESET(BUTTON3, DDR);
	// The buttons need to be pulled up
	BUTTON_SET(BUTTON0, PORT);
	BUTTON_SET(BUTTON1, PORT);
	BUTTON_SET(BUTTON2, PORT);
	BUTTON_SET(BUTTON3, PORT);
}

static inline buttons_state_t read_current_state() {
	buttons_state_t state = 0;
	// Low means the button is pressed
#define UPDATE_STATE(num) \
	state |= BUTTON_READ(BUTTON ## num) ? 0 : (1 << num)
	UPDATE_STATE(0);
	UPDATE_STATE(1);
	UPDATE_STATE(2);
	UPDATE_STATE(3);
#undef UPDATE_STATE
	return state;
}

void buttons_update() {
	buttons_last_state = buttons_current_state;
	buttons_current_state = read_current_state();
	buttons_state_t bit = 1;
	for (uint8_t i = 0; i < NUM_BUTTONS; i++, bit <<= 1) {
		if (buttons_current_state & bit) {
			button_counters[i]++;
		} else {
			button_counters[i] = 0;
		}
	}
}

#define BUTTON_CHECK(button) assert(button < NUM_BUTTONS)

bool button_is_down(uint8_t button) {
	BUTTON_CHECK(button);
	return buttons_current_state & (0x01 << button);
}

bool button_pressed(uint8_t button) {
	BUTTON_CHECK(button);
	return button_counters[button] == BUTTONS_PRESSED_TICKS;
}

bool button_held(uint8_t button) {
	BUTTON_CHECK(button);
	return button_counters[button] == BUTTONS_HELD_TICKS;
}

bool button_repeated(uint8_t button) {
	BUTTON_CHECK(button);
	uint8_t counter = button_counters[button];
	return counter && ~(counter & 0x00FFFFFF);
}

#undef BUTTON_CHECK
