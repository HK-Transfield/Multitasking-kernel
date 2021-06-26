/*******************************
 * HK Transfield
 * 
 * Parallel Task
 *******************************/

#include "wramp.h"

/**
 * Shows numbers in the SSD as a hexadecimal number
 * by shifting the number logical right.
 * 
 * @param n The number that will be written
*/
void writeHexSSD(int n)
{
    WrampParallel->LowerRightSSD = n;
    n >>= 4;

    WrampParallel->LowerLeftSSD = n;
    n >>= 4;

    WrampParallel->UpperRightSSD = n;
    n >>= 4;

    WrampParallel->UpperLeftSSD = n;
}

/**
 * Shows numbers in the SSD as a decimal number by
 * calculating the remainder and dividing the number
 * by 10.
 * 
 * @param n The number that will be written
*/
void writeDecSSD(int n)
{
    int rem = 0;

    rem = n % 10;
    WrampParallel->LowerRightSSD = rem;
    n /= 10;

    rem = n % 10;
    WrampParallel->LowerLeftSSD = rem;
    n /= 10;

    rem = n % 10;
    WrampParallel->UpperRightSSD = rem;
    n /= 10;

    rem = n % 10;
    WrampParallel->UpperLeftSSD = rem;
}

/**
 * Main entry point to the parallel task
*/
void parallel_main()
{

    // declare variables
    int switches = 0; // stores the value of the parallel switches
    int buttons = 0;  // stores the value of the parallel push buttons
    int pressed = 0;  // tracks the last button pressed

    while (1)
    { // begin infinite loop

        // read values from parallel ports
        switches = WrampParallel->Switches;
        buttons = WrampParallel->Buttons;

        if (buttons == 1 || buttons == 2 || buttons == 4) // any push button has been pressed
        {
            while (1)
            {
                // continuing reading from the parallel port registers
                switches = WrampParallel->Switches;
                buttons = WrampParallel->Buttons;

                if (buttons == 1 || pressed == 1)
                {
                    pressed = 1;
                    writeHexSSD(switches);
                }

                if (buttons == 2 || pressed == 2)
                {
                    pressed = 2;
                    writeDecSSD(switches);
                }

                if (buttons == 4)
                    return;
            }
        }
        else
        {
            // if no buttons have been pressed, print switches as hex
            writeHexSSD(switches);
        }
    }
}
