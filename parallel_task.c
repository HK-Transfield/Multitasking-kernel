/*******************************
 * Harmon Transfield
 * 1317381
 * 
 * Parallel Task
 * Multitasking Kernel, COMPX203
 *******************************/

#include "wramp.h"

void writeHexSSD(int n) {
    WrampParallel->LowerRightSSD = n;
    n = n >> 4;

    WrampParallel->LowerLeftSSD = n;
    n = n >> 4;

    WrampParallel->UpperRightSSD = n;
    n = n >> 4;
    
    WrampParallel->UpperLeftSSD = n;
}

void writeDecSSD(int n) {
    int rem = 0;

    rem = n % 10;
    WrampParallel->LowerRightSSD = rem;
    n = n / 10;

    rem = n % 10;
    WrampParallel->LowerLeftSSD = rem;
    n = n / 10;
    
    rem = n % 10;
    WrampParallel->UpperRightSSD = rem;
    n = n / 10;
    
    rem = n % 10;
    WrampParallel->UpperLeftSSD = rem;
}

void parallel_main() {

    // declase variables
    int switches = 0;   // stores the value of the parallel switches
    int buttons = 0;    // stores the value of the parallel push buttons
    int pressed = 0;    // tracks the last button pressed

    while(1) { // begin infinite loop

        // read values from parallel ports
        switches = WrampParallel->Switches;
        buttons = WrampParallel->Buttons;

        if(buttons == 1 || buttons == 2 || buttons == 4) { // any button has been pressed
            while(1) {
                switches = WrampParallel->Switches;
                buttons = WrampParallel->Buttons;

                if (buttons == 1 || pressed == 1) {
                    pressed = 1;
                    writeHexSSD(switches);
                }
                    
                if (buttons == 2 || pressed == 2) {
                    pressed = 2;
                    writeDecSSD(switches);
                }
                
                if (buttons == 4)
                    return;
            }
        } else {
            
            // if no buttons have been pressed, print switches as hex
            writeHexSSD(switches);
        }
    }
}
