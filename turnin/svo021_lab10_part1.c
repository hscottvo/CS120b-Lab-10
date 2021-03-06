/*	Author: lab
 *  Partner(s) Name: Scott Vo
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include "bit.h"
#include "keypad.h"
#include "scheduler.h"
#endif


int keypad_tick(int state) {
    unsigned char pad = GetKeypadKey();
    switch(pad) {
        case '\0': PORTB = 0x1F; break;
        case '1': PORTB = 0x81; break;
        case '2': PORTB = 0x81; break;
        case '3': PORTB = 0x83; break;
        case '4': PORTB = 0x84; break;
        case '5': PORTB = 0x85; break;
        case '6': PORTB = 0x86; break;
        case '7': PORTB = 0x87; break;
        case '8': PORTB = 0x88; break;
        case '9': PORTB = 0x89; break;
        case 'A': PORTB = 0x8A; break;
        case 'B': PORTB = 0x8B; break;
        case 'C': PORTB = 0x8C; break;
        case 'D': PORTB = 0x8D; break;
        case '*': PORTB = 0x8E; break;
        case '0': PORTB = 0x80; break;
        case '#': PORTB = 0x8F; break;
        default: PORTB = 0x1B; break;
    }
    return state;
}


int main(void) {
    /* Insert DDR and PORT initializations */
    unsigned char x;
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xF0; PORTC = 0x0F;
    /* Insert your solution below */
    static task task1;
    task *tasks[] = {&task1};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
    const char start = -1;

    task1.state = start;
    task1.period = 50;
    task1.elapsedTime = task1.period;
    task1.TickFct = &keypad_tick;

    unsigned long GCD = tasks[0]->period;
    for(unsigned long i = 1; i < numTasks; i++){
        GCD = findGCD(GCD, tasks[i]->period);
    }

    TimerSet(GCD);
    TimerOn();

    while (1) {
        for(unsigned long i = 0; i < numTasks; i++) {
            if(tasks[i]->elapsedTime == tasks[i]->period) {
                tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
                tasks[i]->elapsedTime = 0;
            }
            tasks[i]->elapsedTime += GCD;
        }
        while(!TimerFlag);
        TimerFlag = 0;
    }        
    return 0;
}
