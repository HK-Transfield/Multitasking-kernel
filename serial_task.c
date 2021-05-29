/*******************************
 * Harmon Transfield
 * 1317381
 * 
 * Serial Task
 * Multitasking Kernel, COMPX203
 *******************************/

#include "wramp.h"

int counter = 0;

void transmitCharSp2(int c)
{
    // check if the TDS bit is set
    while (!(WrampSp2->Stat & 0x2))
        ;

    // transmit character to serial port 2
    WrampSp2->Tx = c;
}

/**
 * Sets the format of the time to "\rmm:ss"
 * i.e. minutes and seconds.
 * 
 * @param counter The current value from the counter that will be writen to SP2
*/
void format_1(int counter)
{
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
 * 
 * @param counter The current value from the counter that will be writen to SP2
*/
void format_2(int counter)
{
    transmitCharSp2('\r');
    transmitCharSp2(counter / 100000 % 10 + '0');
    transmitCharSp2(counter / 10000 % 10 + '0');
    transmitCharSp2(counter / 1000 % 10 + '0');
    transmitCharSp2(counter / 100 % 10 + '0');
    transmitCharSp2('.');
    transmitCharSp2(counter / 10 % 10 + '0');
    transmitCharSp2(counter % 10 + '0');
}

/**
 * Will set the format to "\rtttttt"
 * i.e. the number of timer interrupts.
 * 
 * @param counter The current value from the counter that will be writen to SP2
*/
void format_3(int counter)
{
    transmitCharSp2('\r');
    transmitCharSp2(counter / 100000 % 10 + '0');
    transmitCharSp2(counter / 10000 % 10 + '0');
    transmitCharSp2(counter / 1000 % 10 + '0');
    transmitCharSp2(counter / 100 % 10 + '0');
    transmitCharSp2(counter / 10 % 10 + '0');
    transmitCharSp2(counter % 10 + '0');
    transmitCharSp2(' ');
}

/**
 * Program is exiting, write a goodbye message to SP2
*/
void format_clear()
{
    transmitCharSp2('\r');
    transmitCharSp2('G');
    transmitCharSp2('o');
    transmitCharSp2('o');
    transmitCharSp2('d');
    transmitCharSp2('b');
    transmitCharSp2('y');
    transmitCharSp2('e');
}

/**
 * Main entry point for the serial task
*/
void serial_main()
{
    int pressed = 0; // tracks the last valid key pressed by the user

    while (1)
    {
        if (WrampSp2->Rx == '1' || WrampSp2->Rx == '2' || WrampSp2->Rx == '3' || WrampSp2->Rx == 'q')
        {
            while (1)
            {

                if (WrampSp2->Rx == 'q')
                {
                    format_clear();
                    return;
                }

                if (WrampSp2->Rx == '1' || pressed == '1')
                {
                    pressed = '1';
                    format_1(counter);
                }

                if (WrampSp2->Rx == '2' || pressed == '2')
                {
                    pressed = '2';
                    format_2(counter);
                }

                if (WrampSp2->Rx == '3' || pressed == '3')
                {
                    pressed = '3';
                    format_3(counter);
                }
            }
        }

        // if no buttons have been pressed, print in first format
        format_1(counter);
    }
}
