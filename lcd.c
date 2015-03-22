#include <stdio.h>
#include <wiringPi.h>
#include <mcp23017.h>
#include <lcd.h>

#define AF_BASE         100
#define AF_RED          (AF_BASE + 6)
#define AF_GREEN        (AF_BASE + 7)
#define AF_BLUE         (AF_BASE + 8)

#define AF_E            (AF_BASE + 13)
#define AF_RW           (AF_BASE + 14)
#define AF_RS           (AF_BASE + 15)

#define AF_DB4          (AF_BASE + 12)
#define AF_DB5          (AF_BASE + 11)
#define AF_DB6          (AF_BASE + 10)
#define AF_DB7          (AF_BASE +  9)

#define AF_SELECT       (AF_BASE +  0)
#define AF_RIGHT        (AF_BASE +  1)
#define AF_DOWN         (AF_BASE +  2)
#define AF_UP           (AF_BASE +  3)
#define AF_LEFT         (AF_BASE +  4)

/**
 * Setup LEDs and button pins, as this is independent from MCP23017 chip itself
 */
void init_leds_and_buttons() {
	pinMode(AF_RED, OUTPUT);
	pinMode(AF_GREEN, OUTPUT);
	pinMode(AF_BLUE, OUTPUT);

	// Blackout
	digitalWrite(AF_RED, HIGH);
	digitalWrite(AF_GREEN, HIGH);
	digitalWrite(AF_BLUE, HIGH);

	// Buttons 
	int i;
	for (i = 0; i <= 4; i++)
	{
		pinMode(AF_BASE + i, INPUT);
		pullUpDnControl(AF_BASE + i, PUD_UP);
	}

	// Unused & off
	pinMode(AF_RW, OUTPUT);
	digitalWrite(AF_RW, LOW);
}

/**
 *
 */
int require_argc(int argc, int min_count)
{
	if (argc >= min_count + 1) // 0th is filename
	{
		return 1;
	}
	else
	{
		printf("Invalid arguments.\n");
		return 0;
	}
}


/**
 *
 */
int arg_is(char *arg, char *val)
{
	return strcmp(arg, val) == 0;
}

/**
 *
 */
void cmd_help()
{
	printf("halp now\n");
}

/**
 *
 */
void cmd_init()
{
	printf("init\n");
}

/**
 *
 */
void cmd_color(int argc, char **argv)
{
	printf("color\n");
}

/**
 *
 */
void cmd_char(int argc, char **argv)
{
	printf("char\n");
}

/**
 *
 */
void cmd_write(int argc, char **argv)
{
	printf("write\n");
}

/**
 *
 */
void cmd_button(int argc, char **argv)
{
	printf("button\n");
}

/**
 *
 */
int main (int argc, char **argv)
{
	// Setup internals evritiem
	wiringPiSetup();
	mcp23017Setup(AF_BASE, 0x20);

	int lcd;

	if (!require_argc(argc, 1))
	{
		cmd_help();
		return 0;
	}

	if (arg_is(argv[1], "init"))
	{
		cmd_init();
	}
	else if (arg_is(argv[1], "color"))
	{
		cmd_color(argc, argv);
	}
	else if (arg_is(argv[1], "char"))
	{
		cmd_char(argc, argv);
	}
	else if (arg_is(argv[1], "write"))
	{
		cmd_write(argc, argv);
	}
	else if (arg_is(argv[1], "button"))
	{
		cmd_button(argc, argv);
	}
	else
	{
		cmd_help();
	}

	printf("END\n");
	return 0;
}
