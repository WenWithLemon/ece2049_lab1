/************** ECE2049 DEMO CODE ******************/
/**************  13 March 2019   ******************/
/***************************************************/

#include <msp430.h>
#include <stdlib.h>

/* Peripherals.c and .h are where the functions that implement
 * the LEDs and keypad, etc are. It is often useful to organize
 * your code by putting like functions together in files.
 * You include the header associated with that file(s)
 * into the main file of your project. */
#include "peripherals.h"

// Function Definition
void swDelay(char numLoops) {
    // This function is a software delay. It performs
    // useless loops to waste a bit of time
    //
    // Input: numLoops = number of delay loops to execute
    // Output: none
    //
    // smj, ECE2049, 25 Aug 2013

    volatile unsigned int i,j;  // volatile to prevent removal in optimization
                                // by compiler. Functionally this is useless code

    for (j=0; j<numLoops; j++) {
        i = 50000;                  // SW Delay
        while (i > 0)               // could also have used while (i)
           i--;
    }
}

struct Alien {
    int yPos;
    int xPos;
    char killNum;
} typedef Alien;

Alien generateAlien() {
    Alien a;
    int r = rand() % 9;
    a.xPos = r * 10;
    a.yPos = 15;
    a.killNum = r;
    return a;
}
void drawAlien(Alien a){
    Graphics_drawStringCentered(&g_sContext, "a", AUTO_STRING_LENGTH, a.xPos, a.yPos, TRANSPARENT_TEXT);
    a.yPos = a.yPos + 10;
}
// Declare globals here
int state = 0;
#define MENU 0
#define COUNTDOWN 1
#define LEVEL1 2
#define LEVEL2 3
#define LEVEL3 4
#define LEVEL4 5
long unsigned int mainCounter = 0;

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

    Alien a[4];
    int c;
    for (c = 0; c < 4; c++) {
        a[c] = generateAlien();
    }

    int alienCounter = 0;
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
                long unsigned int i;
                for (i = 0; i < 200000; i++) {
                    if (getKey() == '*') {
                        state = COUNTDOWN;
                        break;
                    }
                }
                break;

            case COUNTDOWN:
                Graphics_clearDisplay(&g_sContext); // Clear the display
                Graphics_drawStringCentered(&g_sContext, "3", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);
                Graphics_flushBuffer(&g_sContext);
                swDelay(2);

                Graphics_clearDisplay(&g_sContext); // Clear the display
                Graphics_drawStringCentered(&g_sContext, "2", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);
                Graphics_flushBuffer(&g_sContext);
                swDelay(2);

                Graphics_clearDisplay(&g_sContext);
                Graphics_drawStringCentered(&g_sContext, "1", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);
                Graphics_flushBuffer(&g_sContext);
                swDelay(2);

                state = LEVEL1;
                break;
            case LEVEL1:
                if (mainCounter % 40000 == 0) {
                    Graphics_clearDisplay(&g_sContext); // Clear the display
                    if (alienCounter >= 0) {
                        drawAlien(a[0]);
                    }
                    if (alienCounter >= 1) {
                        drawAlien(a[1]);
                    }
                    if (alienCounter >= 2) {
                        drawAlien(a[2]);
                    }
                    if (alienCounter >= 3) {
                        drawAlien(a[3]);
                    }
                    Graphics_flushBuffer(&g_sContext);
                    alienCounter++;
                }
        }
    }
    mainCounter++;
}

