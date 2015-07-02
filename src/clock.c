#include <segments.h>
#include <buttons.h>
#include <ds1337.h>

#include <stddef.h>
#include <util/delay.h>

int main(int argc, char **argv) {
	segments_init();
	buttons_init();

	uint8_t counters[4] = { 0, 0, 0, 0 };

#define INCR(i) \
	if (++counters[i] >= 10) { \
		counters[i] = 0; \
	}

	while (1) {
		buttons_update();
		if (button_is_down(0)) {
			INCR(0)
		}
		if (button_pressed(1)) {
			INCR(1)
		}
		if (button_pressed(2)) {
			INCR(2)
		}
		for (size_t i = 0; i < sizeof(counters) / sizeof(counters[0]); i++) {
			segments_set_digit(i, counters[i]);
		}
		segments_draw();
		_delay_ms(1);
	}
}
