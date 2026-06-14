#include <common.h>
#include <command.h>
#include <asm/io.h>
#include "mach/molchip_gpio.h"

#define LED_ON_LEVEL	gpio_mode_low
#define LED_OFF_LEVEL	gpio_mode_high

static u8 led_gpio;

static void gpio_led_init(void) {
	gpio_request(led_gpio, "led_gpio");
	gpio_direction_output(led_gpio, LED_OFF_LEVEL);
}

static void gpio_led_on(void) {
	gpio_set_value(led_gpio, LED_ON_LEVEL);
}

static void gpio_led_off(void) {
	gpio_set_value(led_gpio, LED_OFF_LEVEL);
}

static void led_blink(int delay) {
	gpio_led_on();
	mdelay(delay);
	gpio_led_off();
	mdelay(delay);
}

int do_led_blink(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[]) {
	const char *str_cmd;
	u32 delay, count, i;

	if (argc < 3) {
		goto show_usage;
	}

	led_gpio = simple_strtoul(argv[1], NULL, 10);
	str_cmd = argv[2];

	switch (*str_cmd) {
	case 'o': // turn on the led
		if (argc != 3) goto show_usage;
		gpio_led_init();
		gpio_led_on();
		return 0;

	case 'f': // turn off the led
		if (argc != 3) goto show_usage;
		gpio_led_init();
		gpio_led_off();
		return 0;

	case 'b': // Blink the LED
		if (argc != 5) goto show_usage;
		delay = simple_strtoul(argv[3], NULL, 10);
		count = simple_strtoul(argv[4], NULL, 10);

		gpio_led_init();
		if (count > 10) {
			// Infinite blinking
			while (1) {
				led_blink(delay);
			}
		} else {
			// Blink for a limited number of times
			for (i = 0; i < count; i++) {
				led_blink(delay);
			}
		}
		return 0;

	default:
		goto show_usage;
	}

show_usage:
	// printf("Usage:\n");
	// printf("  grled <gpio> on        - Turn on LED\n");
	// printf("  grled <gpio> off       - Turn off LED\n");
	// printf("  grled <gpio> blink <delay_ms> <count> - Blink LED\n");
	return CMD_RET_USAGE;
}

U_BOOT_CMD(
	blinkled, 5, 0, do_led_blink,
	"Control LED blinking",
	"<gpio> <command> <delay> <count>\n"
	"  - gpio:   GPIO number of the LED\n"
	"  - command: 'o' to turn on, 'f' to turn off, 'b' to blink\n"
	"  - delay:  Time in milliseconds for each blink (used with 'b')\n"
	"  - count:  Number of blinks (use >10 for infinate blinking)"
);
