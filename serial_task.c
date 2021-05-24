#include "wramp.h"

int counter = 0;

void transmitCharSp2(int c) {
    // check if the TDS bit is set
    while (!(WrampSp2->Stat & 0x2));
    
    // transmit character to serial port 2
    WrampSp2->Tx = c;
}

/**
 * Sets the format of the time to "\rmm:ss"
 * i.e. minutes and seconds
*/
void format_1(int counter) {
    int minutes = counter / 100 / 60;
    int seconds = counter / 100 % 60;
    
    transmitCharSp2('\r');
    transmitCharSp2(minutes / 10 + '0');
    transmitCharSp2(minutes % 10 + '0');
    transmitCharSp2(':');
    transmitCharSp2(seconds / 10 + '0');
    transmitCharSp2(seconds % 10 + '0');
    transmitCharSp2(' ');
    transmitCharSp2(' ');
}

/**
 * Sets the format of the time to "\rssss.ss"
 * i.e. seconds pring to two decimal places
*/
void format_2(int counter) {
    transmitCharSp2('\r');
    transmitCharSp2(counter / 100000 % 10 + '0');
    transmitCharSp2(counter / 10000 % 10 + '0');
    transmitCharSp2(counter / 1000 % 10 + '0');
    transmitCharSp2(counter / 100 % 10 + '0');
    transmitCharSp2('.');
    transmitCharSp2(counter / 10 % 10 + '0');
    transmitCharSp2(counter % 10 + '0');
}

void format_3(int counter) {
    transmitCharSp2('\r');
    transmitCharSp2(counter / 100000 % 10 + '0');
    transmitCharSp2(counter / 10000 % 10 + '0');
    transmitCharSp2(counter / 1000 % 10 + '0');
    transmitCharSp2(counter / 100 % 10 + '0');
    transmitCharSp2(counter / 10 % 10 + '0');
    transmitCharSp2(counter % 10 + '0');
    transmitCharSp2(' ');
}

void format_clear() {
    transmitCharSp2('\r');
    transmitCharSp2(' ');
    transmitCharSp2(' ');
    transmitCharSp2(' ');
    transmitCharSp2(' ');
    transmitCharSp2(' ');
    transmitCharSp2(' ');
    transmitCharSp2(' ');
}

void serial_main() {

    int rx = 0;
    int currFormat = 0;

main_loop:
    while(1) {

        // will wait for character to be set
        rx = WrampSp2->Rx;

        if (rx == '1' || rx == '2' || rx == '3' || rx == 'q') { // 1, 2, 3, or q have been recieved
            while (1) {

                rx = WrampSp2->Rx;

                if (rx == 'q') {
                    format_clear();
                    return;
                }
                
                if (rx == '1' || currFormat == '1') {
                    currFormat = '1';
                    format_1(counter);
                }
                   
                
                if (rx == '2' || currFormat == '2') {
                    currFormat = '2';
                    format_2(counter);
                }

                if (rx == '3' || currFormat == '3') {
                    currFormat = '3';
                    format_3(counter);
                }
            }
        }
        format_1(counter);
    }    
}
