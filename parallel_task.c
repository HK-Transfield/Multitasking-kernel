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
    int readSwitches = 0;
    int pushButtons = 0;

    while(1) { // begin infinite loop

        // read values from parallal I/O
        readSwitches = WrampParallel->Switches;
        pushButtons = WrampParallel->Buttons;

        if(pushButtons == 1 || pushButtons == 2 || pushButtons == 4) { // rightmost push button was pressed
            while(1) {
                readSwitches = WrampParallel->Switches;
                pushButtons = WrampParallel->Buttons;

                if (pushButtons == 1)
                    writeHexSSD(readSwitches);
                
                if (pushButtons == 2)
                    writeDecSSD(readSwitches);
                
                if (pushButtons == 4)
                    return;
            }
        } else {
            
            // if no buttons have been pressed, print switches as hex
            writeHexSSD(readSwitches);
        }
    }
}
