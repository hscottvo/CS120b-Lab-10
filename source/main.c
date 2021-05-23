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
        case '\0': PORTB = 0x1F; SetBit(PORTB, 7, 0); break;
        case '1': PORTB = 0x01; SetBit(PORTB, 7, 1); break;
        case '2': PORTB = 0x01; SetBit(PORTB, 7, 1); break;
        case '3': PORTB = 0x03; SetBit(PORTB, 7, 1); break;
        case '4': PORTB = 0x04; SetBit(PORTB, 7, 1); break;
        case '5': PORTB = 0x05; SetBit(PORTB, 7, 1); break;
        case '6': PORTB = 0x06; SetBit(PORTB, 7, 1); break;
        case '7': PORTB = 0x07; SetBit(PORTB, 7, 1); break;
        case '8': PORTB = 0x08; SetBit(PORTB, 7, 1); break;
        case '9': PORTB = 0x09; SetBit(PORTB, 7, 1); break;
        case 'A': PORTB = 0x0A; SetBit(PORTB, 7, 1); break;
        case 'B': PORTB = 0x0B; SetBit(PORTB, 7, 1); break;
        case 'C': PORTB = 0x0C; SetBit(PORTB, 7, 1); break;
        case 'D': PORTB = 0x0D; SetBit(PORTB, 7, 1); break;
        case '*': PORTB = 0x0E; SetBit(PORTB, 7, 1); break;
        case '0': PORTB = 0x00; SetBit(PORTB, 7, 1); break;
        case '#': PORTB = 0x0F; SetBit(PORTB, 7, 1); break;
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
