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

// unsigned char pad = 0x00;
// unsigned char lock_state = 0x01;

// int keypad_tick(int state) {
//     pad = GetKeypadKey();
//     switch(pad) {
//         case '\0': PORTB = 0x00 | (~PINB & 0x81); break;
//         case '1': PORTB = 0x40 | (~PINB & 0x81); break;
//         case '2': PORTB = 0x40 | (~PINB & 0x81); break;
//         case '3': PORTB = 0x40 | (~PINB & 0x81); break;
//         case '4': PORTB = 0x40 | (~PINB & 0x81); break;
//         case '5': PORTB = 0x40 | (~PINB & 0x81); break;
//         case '6': PORTB = 0x40 | (~PINB & 0x81); break;
//         case '7': PORTB = 0x40 | (~PINB & 0x81); break;
//         case '8': PORTB = 0x40 | (~PINB & 0x81); break;
//         case '9': PORTB = 0x40 | (~PINB & 0x81); break;
//         case 'A': PORTB = 0x40 | (~PINB & 0x81); break;
//         case 'B': PORTB = 0x40 | (~PINB & 0x81); break;
//         case 'C': PORTB = 0x40 | (~PINB & 0x81); break;
//         case 'D': PORTB = 0x40 | (~PINB & 0x81); break;
//         case '*': PORTB = 0x40 | (~PINB & 0x81); break;
//         case '0': PORTB = 0x40 | (~PINB & 0x81); break;
//         case '#': PORTB = 0x40 | (~PINB & 0x81); break;
//         default: PORTB = 0x00; break;
//     }
//     return state;
// }

// unsigned char pass[5] = {'1', '2', '3', '4', '5'};
// unsigned char pass_index;

// enum pass_states {pass_wait};

// int check_pass(int state) {
//     unsigned char input = (~PINB & 0x80);
//     state = pass_wait;
//     switch(state) {
//         case pass_wait:
//             PORTB = (((input >> 7) && (~PINB & 0x01)) | (PINB & 0xFE));
//     }
//     switch(state) {

//     }
//     return state;
// }

// int main(void) {
//     /* Insert DDR and PORT initializations */
//     DDRB = 0x7F; PORTB = 0x80;
//     DDRC = 0xF0; PORTC = 0x0F;
//     /* Insert your solution below */
//     static task task1, task2;
//     task *tasks[] = {&task1, &task2};
//     const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
//     const char start = -1;

//     task1.state = start;
//     task1.period = 50;
//     task1.elapsedTime = task1.period;
//     task1.TickFct = &keypad_tick;

//     task2.state = start;
//     task2.period = 50;
//     task2.elapsedTime = task2.period;
//     task2.TickFct = &check_pass;

//     unsigned long GCD = tasks[0]->period;
//     for(unsigned long i = 1; i < numTasks; i++){
//         GCD = findGCD(GCD, tasks[i]->period);
//     }

//     TimerSet(GCD);
//     TimerOn();

//     while (1) {
//         for(unsigned long i = 0; i < numTasks; i++) {
//             if(tasks[i]->elapsedTime == tasks[i]->period) {
//                 tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
//                 tasks[i]->elapsedTime = 0;
//             }
//             tasks[i]->elapsedTime += GCD;
//         }
//         while(!TimerFlag);
//         TimerFlag = 0;
//     }        
//     return 0;
// }
int main(void) {
    DDRB = 0x7F; PORTB = 0x80;
    DDRC = 0xF0; PORTC = 0x0F;
    while(1) {
        PORTB = 0xFF;
    }
    return 0;
}