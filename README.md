LCD-Utility
===========
A simple toolset to operate [Adafruit's RGB LCD kit for Raspberry Pi](http://www.adafruit.com/products/1110) with an easy command line interface directly by hand or from another programs.

Uses [WiringPi LCD library](http://wiringpi.com/dev-lib/lcd-library/).

Compile: `gcc -Wall -o lcd lcd.c -lwiringPi -lwiringPiDev`

Available commands:

`lcd init`  
Initialiazes LEDs and buttons. Use only once in the beginning.

`lcd color red|green|blue on|off`  
One LED at a time. Use as many times as necessary for desired effect.

`lcd button select|up|down|left|right`  
Get the current state of a button. 0 - released, 1 - pressed.

`lcd write <text>`  
Always starts at the beginning, so you must pass the whole text to be displayed.  
Display custom characters with '\0' - '\8'.

`lcd char <slot> <row1> <row2> ... <row8>`  
Set custom character in slots 0 - 8.  
Pass all 8 rows as a pixel mask 0 - 31.  
Use online tools like http://www.quinapalus.com/hd44780udg.html for generation.

Example that should give you a nice image of Euro sign on a green background:  
`./lcd init`  
`./lcd color green on`  
`./lcd char 0 0 6 9 30 8 30 9 6`  
`./lcd write "This is Euro: \0"`
