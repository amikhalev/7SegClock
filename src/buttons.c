#include <buttons.h>

#include <assert.h>

void buttons_init() {
	buttons_current_state = 0;
	buttons_last_state = 0;
	for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
		button_counters[i] = 0;
	}
	BUTTON0_DDR &= ~BUTTON0_BIT;
	BUTTON1_DDR &= ~BUTTON1_BIT;
	BUTTON2_DDR &= ~BUTTON2_BIT;
	BUTTON3_DDR &= ~BUTTON3_BIT;
	// The buttons need to be pulled up
	BUTTON0_PORT |= BUTTON0_BIT;
	BUTTON1_PORT |= BUTTON1_BIT;
	BUTTON2_PORT |= BUTTON2_BIT;
	BUTTON3_PORT |= BUTTON3_BIT;
}

static inline buttons_state_t get_current_state() {
	buttons_state_t state = 0;
	// Low means the button is pressed
#define UPDATE_STATE(num) \
	state |= (BUTTON ## num ## _PIN & BUTTON ## num ## _BIT) ? 0 : (1 << num)
	UPDATE_STATE(0);
	UPDATE_STATE(1);
	UPDATE_STATE(2);
	UPDATE_STATE(3);
#undef UPDATE_STATE
	return state;
}

void buttons_update() {
	buttons_last_state = buttons_current_state;
	buttons_current_state = get_current_state();
	buttons_state_t bit = 0x01;
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

#undef BUTTON_CHECK
