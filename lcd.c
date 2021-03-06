#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
void cmd_help()
{
	printf("\nAvailable commands:\n");
	printf("\n");
	printf("lcd init\n");
	printf("Initialiazes LEDs and buttons. Use only once in the beginning.\n\n");
	printf("lcd color red|green|blue on|off\n");
	printf("One LED at a time. Use as many times as necessary for desired effect.\n\n");
	printf("lcd button select|up|down|left|right\n");
	printf("Get the current state of a button. 0 - released, 1 - pressed.\n\n");
	printf("lcd write <text>\n");
	printf("Always starts at the beginning, so you must pass the whole text to be displayed.\n");
	printf("Display custom characters with '\\0' - '\\8'.\n\n");
	printf("lcd char <slot> <row1> <row2> ... <row8>\n");
	printf("Set custom character in slots 0 - 8.\n");
	printf("Pass all 8 rows as a pixel mask 0 - 31.\n");
	printf("Use online tools like http://www.quinapalus.com/hd44780udg.html for generation.\n\n");
}

/**
 *
 */
void invalid_args()
{
	printf("Invalid arguments.\n\n");
	cmd_help();
	exit(1);
}

/**
 *
 */
void require_argc(int argc, int min_count)
{
	if (argc < min_count + 1) // 0th is filename
	{
		printf("Invalid argument count.\n\n");
		cmd_help();
		exit(1);
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
void cmd_init()
{
	init_leds_and_buttons();
}

/**
 *
 */
void cmd_color(int argc, char **argv)
{
	require_argc(argc, 3);

	int color;
	if (arg_is(argv[2], "red"))
	{
		color = AF_RED;
	}
	else if (arg_is(argv[2], "green"))
	{
		color = AF_GREEN;
	}
	else if (arg_is(argv[2], "blue"))
	{
		color = AF_BLUE;
	}
	else
	{
		invalid_args();
	}

	int state;
	if (arg_is(argv[3], "on"))
	{
		state = LOW;
	}
	else if (arg_is(argv[3], "off"))
	{
		state = HIGH;
	}
	else
	{
		invalid_args();
	}

	digitalWrite(color, state);
}

void set_state(char *text)
{
	FILE *f;
	f = fopen("state", "w");
	fprintf(f, text);
	fclose(f);
}

void restore_state()
{
	int lcd = lcdInit(2, 16, 4, AF_RS, AF_E, AF_DB4, AF_DB5, AF_DB6, AF_DB7, 0, 0, 0, 0);
	lcdPosition(lcd, 0, 0);

	FILE *f;
	char c;
	int special = 0;

	f = fopen("state", "r");
	while (1)
	{
		c = fgetc(f);
		if (feof(f))
		{
			break;
		}

		if (!special)
		{
			if (c == '\\')
			{
				special = 1;
			}
			else
			{
				lcdPutchar(lcd, c);
			}
		}
		else
		{
			lcdPutchar(lcd, c - 48); // ASCII char "0" to int 0
			special = 0;
		}
	}
	fclose(f);
}

/**
 *
 */
void cmd_char(int argc, char **argv)
{
	require_argc(argc, 10);

	int position = atoi(argv[2]);
	unsigned char img[8];

	int i;
	for (i = 0; i < 8; i++)
	{
		img[i] = atoi(argv[i +3]);
	}

	int lcd = lcdInit(2, 16, 4, AF_RS, AF_E, AF_DB4, AF_DB5, AF_DB6, AF_DB7, 0, 0, 0, 0);
	lcdCharDef(lcd, position, img);

	restore_state();
}

/**
 *
 */
void cmd_write(int argc, char **argv)
{
	require_argc(argc, 2);
	set_state(argv[2]);
	restore_state();
}

/**
 *
 */
void cmd_button(int argc, char **argv)
{
	require_argc(argc, 2);

	int button;
	if (arg_is(argv[2], "select"))
	{
		button = AF_SELECT;
	}
	else if (arg_is(argv[2], "up"))
	{
		button = AF_UP;
	}
	else if (arg_is(argv[2], "down"))
	{
		button = AF_DOWN;
	}
	else if (arg_is(argv[2], "left"))
	{
		button = AF_LEFT;
	}
	else if (arg_is(argv[2], "right"))
	{
		button = AF_RIGHT;
	}
	else
	{
		invalid_args();
	}

	printf("%d\n", digitalRead(button) == LOW); // LOW == pressed
}

/**
 *
 */
int main (int argc, char **argv)
{
	// Setup internals evritiem
	wiringPiSetup();
	mcp23017Setup(AF_BASE, 0x20);

	require_argc(argc, 1);

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

	return 0;
}
