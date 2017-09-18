#include <segments.h>
#include <buttons.h>
#include <ds1337.h>

#include <stddef.h>
#include <util/delay.h>

#define SET_BCD(FIRST_DIGIT, BCD) \
	segments_set_digit((FIRST_DIGIT) + 1, (BCD).ones); \
	segments_set_digit((FIRST_DIGIT), (BCD).tens)

enum clock_state {
	CLK_ST_TIME,
	CLK_ST_DAY,
	CLK_ST_DATE,
	CLK_ST_YEAR,
	CLK_ST_NUM_STATES
};

enum clock_state clk_st;

union time {
	struct {
		bcd8 seconds;
		bcd8 minutes;
		bcd8 hour;
	};
	struct {
		bcd8 day;
	};
};

union time tm;

int main(int argc, char **argv) {
	segments_init();
	buttons_init();
	ds1337_init();

	while (1) {
		buttons_update();

		switch (clk_st) {
		case CLK_ST_TIME:
			tm.minutes = ds1337_get_minutes();
			tm.hour = ds1337_get_hour();
			if (button_repeated(0)) {
				DS1337_INCR(tm.hour, HOUR_24);
				ds1337_set_hour(tm.hour);
			}
			if (button_repeated(1)) {
				DS1337_INCR(tm.minutes, MINUTES);
				ds1337_set_minutes(tm.minutes);
			}
			SET_BCD(0, tm.hour);
			SET_BCD(2, tm.minutes);

			segments.dots = SEGMENTS_DOTS_COLON;
			break;
		case CLK_ST_DAY:
			tm.day = ds1337_get_day();
			if (button_repeated(0)) {
			}
			break;
		case CLK_ST_DATE:
			break;
		case CLK_ST_YEAR:
			break;
		default:
			clk_st = CLK_ST_TIME;
			break;
		}

		segments_draw();
		_delay_ms(1);
	}
}
