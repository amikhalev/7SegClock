#include <segments.h>

#include <stddef.h>
#include <util/delay.h>

int main(int argc, char **argv) {
	segments_init();
	segments_set_digit(0, 0);
	segments_set_digit(1, 1);
	segments_set_digit(2, 8);
	segments_set_digit(3, 9);
	segments.dots = SEGMENTS_DOTS_COLON;
	while (1) {
		segments_draw();
		_delay_ms(1);
	}
}
