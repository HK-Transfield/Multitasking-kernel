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
main_loop:
    while(1) {

        
        /*
        
        Need to fix it so that if any other character is pressed it will ignore it
        */
        switch (WrampSp2->Rx)
        {
            // no characters have been recieved from SP2
            case 0:
                format_1(counter);
                break;

            // sent '1' to SP2  
            case '1': 
                format_1(counter);
                break;
            
            // sent '2' to SP2
            case '2':
                format_2(counter);        
                break;

            case '3':
                format_3(counter);
                break;

            case 'q':
                format_clear();
                return;
            
            default:
                break;
        }
    }    
}
