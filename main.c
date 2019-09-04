/************** ECE2049 DEMO CODE ******************/
/**************  13 March 2019   ******************/
/***************************************************/

#include <msp430.h>

/* Peripherals.c and .h are where the functions that implement
 * the LEDs and keypad, etc are. It is often useful to organize
 * your code by putting like functions together in files.
 * You include the header associated with that file(s)
 * into the main file of your project. */
#include "peripherals.h"

// Function Prototypes
void swDelay(char numLoops);

// Declare globals here
int state = 0;
#define MENU 0
// Main
void main(void)

{
    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer. Always need to stop this!!
                                 // You can then configure it properly, if desired

    // Useful code starts here
    initLeds();
    configDisplay();
    configKeypad();

    //dispThree[0] = ' ';
    //dispThree[2] = ' ';

    while (1) {
        switch (state) {
            case MENU:
                // *** Intro Screen ***
                Graphics_clearDisplay(&g_sContext); // Clear the display

                // Write some text to the display
                Graphics_drawStringCentered(&g_sContext, "Welcome", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "To", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "Space", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "Invaders", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "Press *", AUTO_STRING_LENGTH, 48, 55, TRANSPARENT_TEXT);

                // Draw a box around everything because it looks nice
                Graphics_Rectangle box = {.xMin = 5, .xMax = 91, .yMin = 5, .yMax = 91 };
                Graphics_drawRectangle(&g_sContext, &box);
                Graphics_flushBuffer(&g_sContext);
        }
    }





//void swDelay(char numLoops) {
//	// This function is a software delay. It performs
//	// useless loops to waste a bit of time
//	//
//	// Input: numLoops = number of delay loops to execute
//	// Output: none
//	//
//	// smj, ECE2049, 25 Aug 2013
//
//	volatile unsigned int i,j;	// volatile to prevent removal in optimization
//			                    // by compiler. Functionally this is useless code
//
//	for (j=0; j<numLoops; j++) {
//    	i = 50000;					// SW Delay
//   	    while (i > 0)				// could also have used while (i)
//	       i--;
//    }
}
