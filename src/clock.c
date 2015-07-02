#include <segments.h>
#include <buttons.h>
#include <ds1337.h>

#include <stddef.h>
#include <util/delay.h>

int main(int argc, char **argv) {
	DDRB = 0;
	DDRC = 0;
	DDRD = 0;
	segments_init();
//	buttons_init();

	while (1) {
//		buttons_update();
//		segments_set_digit(0, button_is_down(0) ? 1 : 0);
//		segments_set_digit(1, button_is_down(1) ? 1 : 0);
//		segments_set_digit(2, button_is_down(2) ? 1 : 0);
//		segments_set_digit(3, button_is_down(3) ? 1 : 0);
		segments.digits[1] = PINB;
		segments.digits[2] = PINC;
		segments.digits[3] = PIND;
		segments_draw();
//		_delay_ms(1);
	}
}
