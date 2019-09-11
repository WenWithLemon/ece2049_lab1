#include <msp430.h>
#include <stdlib.h>
#include "peripherals.h"

// Declare globals here
int alienXPos;
int alienYPos;
int level = 1;

char aliens[5][6] = {{0,0,0,0,0,0},
                     {0,0,0,0,0,0},
                     {0,0,0,0,0,0},
                     {0,0,0,0,0,0},
                     {0,0,0,0,0,0}};

// Function Prototypes
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
        i = 50000;// SW Delay
        while (i > 0) {
            i--;        // could also have used while (i)
        }

    }
}

void makeColumns() {
    Graphics_drawStringCentered(&g_sContext, "1", AUTO_STRING_LENGTH, 10, 90, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, "2", AUTO_STRING_LENGTH, 25, 90, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, "3", AUTO_STRING_LENGTH, 40, 90, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, "4", AUTO_STRING_LENGTH, 55, 90, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, "5", AUTO_STRING_LENGTH, 70, 90, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, "6", AUTO_STRING_LENGTH, 85, 90, OPAQUE_TEXT);
    Graphics_flushBuffer(&g_sContext);
}

void generateAliens() {
    // this counts how many aliens there are present
    int c = 0;
    // will be the positions of the aliens
    int i, j;

    // calculate the number of aliens based on which level we're on
    int numAliens = (level + 3) * 2;
    while (c <= numAliens) {
        // make the positions random
        i = rand() % 5;
        j = rand() % 6;
        // if there is no alien here, then put one
        // if not then re loop
        if (aliens[i][j] == 0) {
            aliens[i][j] = j + 1;
            c++;
        }
    }
}

int drawAliens(int xPos, int yPos) {
    int i, j;
    // clear the screen
    Graphics_clearDisplay(&g_sContext);
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 6; j++) {
            // calculate the alien's position based on it's position in the array

            alienXPos = xPos + j*15 + 10;
            alienYPos = yPos + i*10 + 5;
            if (aliens[i][j] != 0) {
                Graphics_drawCircle(&g_sContext, alienXPos, alienYPos, 4);
            }
        }
    }
    makeColumns();
    // r holds the row number of the lowest alien
    int r = checkRows();
    if (r > 0) {
        if (alienYPos > (76 + 8.5 * r)) {
            return 2;
        }
    }
    else {
        return 1;
    }
    return 0;
}

// returns 0 if no aliens in whole row
// returns 1 if there is at least 1 alien
int checkRow(int row) {
    // create counter
    int i = 0;
    // since there are 6 columns in a row
    // we need to loop 6 times
    while (i < 6) {
        // if there is no alien, increase the counter
        if (aliens[row][i] == 0) {
            i++;
        }
        // if there is an alien, then return 1
        else {
            return 1;
        }
    }
    // return 0 if there are no aliens in row
    return 0;
}

int checkRows() {
    // make an array to store the results of checking the rows
    int rows[5];
    // declare counter
    int i;
    // check all the rows and store results in array
    for (i = 0; i < 5; i++) {
        rows[i] = checkRow(i);
    }
    // create loopCounter to see how many times the loop happens
    int loopCounter = 1;
    for (i = 4; i >= 0; i--) {
        // if there is an alien at row i, then return loopCounter
        if (rows[i]) {
            return loopCounter;
        }
        loopCounter++;
    }
    // if there are no aliens in the entire array then return 0
    return 0;
}

// sets all the values in the aliens array to 0
void clearAliens() {
    int i , j;
    for (i = 0; i<5; i++) {
        for (j = 0; j<6; j++) {
            aliens[i][j] = 0;
        }
    }
}

void killAlien(int col) {
    // if the key value is less than one, don't do anything
    if (col + 0x30 < 1) {
        return;
    }
    int c = 4;
    // traverse the rows of the pressed column (starting at the bottom) and kill the first alien
    while (c >= 0) {
        if (aliens[c][col-1] != 0) {
            aliens[c][col-1] = 0;
            return;
        }
        c--;
    }
}



#define MENU 0
#define COUNTDOWN 1
#define LEVELS 2
#define WIN 3
#define LOSE 4

int state = MENU;

// Main
void main(void) {
    // Define some local variables
    char key = 0;
    WDTCTL = WDTPW | WDTHOLD;       // Stop watchdog timer
    initLeds();
    configDisplay();
    configKeypad();

    while (1)    // Forever loop
    {
        switch(state) {
        case MENU:
            Graphics_clearDisplay(&g_sContext);
            Graphics_drawStringCentered(&g_sContext, "Welcome", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
            Graphics_drawStringCentered(&g_sContext, "To", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
            Graphics_drawStringCentered(&g_sContext, "Space", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
            Graphics_drawStringCentered(&g_sContext, "Invaders", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);
            Graphics_drawStringCentered(&g_sContext, "Press *", AUTO_STRING_LENGTH, 48, 55, TRANSPARENT_TEXT);
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
            state = LEVELS;
            generateAliens();
            break;

        case LEVELS:
            printf("need this or error");
            int y = 0;
            int status = 0;
            while(!status) {
                int column;
                int i;
                status = drawAliens(0, (5*y));
                if (status == 1) {
                    state = WIN;
                }
                else if (status == 2) {
                    state = LOSE;
                }
                y++;
                for(i = 0; i < 2000*(3/level); i++) {
                    column = (int)getKey() - 0x30;
                    if (column > 0) {
                        break;
                    }
                }
               killAlien(column);
               column = 0;
            }
            break;

        case WIN:
            level++;
            Graphics_clearDisplay(&g_sContext);
            Graphics_drawStringCentered(&g_sContext, "Next Level", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);
            Graphics_flushBuffer(&g_sContext);
            swDelay(3);
            if (level == 4) {
                Graphics_clearDisplay(&g_sContext);
                Graphics_drawStringCentered(&g_sContext, "You win :)", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "Your", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "Parents", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "Are", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "Proud", AUTO_STRING_LENGTH, 48, 55, TRANSPARENT_TEXT); 
                Graphics_flushBuffer(&g_sContext);
                swDelay(3);
                level = 1;
                clearAliens();
                state = MENU;
                break;
            }
            clearAliens();
            state = COUNTDOWN;
            break;

        case LOSE:
            clearAliens();
            level = 1;
            Graphics_clearDisplay(&g_sContext);
            Graphics_drawStringCentered(&g_sContext, "You lost :(", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
            Graphics_drawStringCentered(&g_sContext, "Your", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
            Graphics_drawStringCentered(&g_sContext, "Parents", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
            Graphics_drawStringCentered(&g_sContext, "Are", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);
            Graphics_drawStringCentered(&g_sContext, "Ashamed", AUTO_STRING_LENGTH, 48, 55, TRANSPARENT_TEXT); 
            Graphics_flushBuffer(&g_sContext);
            swDelay(5);
            state = MENU;
            break;
        }
    }
}
