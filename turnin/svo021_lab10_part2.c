/*	Author: lab
 *  Partner(s) Name: Scott Vo
 *	Lab Section:
 *	Assignment: Lab # 10 Exercise # 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  Demo Link: https://youtu.be/SRcntvBOQ5E
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include "bit.h"
#include "keypad.h"
#include "scheduler.h"
#endif

unsigned char pad = 0x00;
unsigned char lock_state = 0x01;
unsigned char lock_signal = 0x01; // 1 -> locked; 0 -> unlocked

int keypad_tick(int state) {
    pad = GetKeypadKey();
    switch(pad) {
        case '\0': PORTB = 0x00 | (PINB & 0x81); break;
        case '1': PORTB = 0x40 | (PINB & 0x81); break;
        case '2': PORTB = 0x40 | (PINB & 0x81); break;
        case '3': PORTB = 0x40 | (PINB & 0x81); break;
        case '4': PORTB = 0x40 | (PINB & 0x81); break;
        case '5': PORTB = 0x40 | (PINB & 0x81); break;
        case '6': PORTB = 0x40 | (PINB & 0x81); break;
        case '7': PORTB = 0x40 | (PINB & 0x81); break;
        case '8': PORTB = 0x40 | (PINB & 0x81); break;
        case '9': PORTB = 0x40 | (PINB & 0x81); break;
        case 'A': PORTB = 0x40 | (PINB & 0x81); break;
        case 'B': PORTB = 0x40 | (PINB & 0x81); break;
        case 'C': PORTB = 0x40 | (PINB & 0x81); break;
        case 'D': PORTB = 0x40 | (PINB & 0x81); break;
        case '*': PORTB = 0x40 | (PINB & 0x81); break;
        case '0': PORTB = 0x40 | (PINB & 0x81); break;
        case '#': PORTB = 0x40 | (PINB & 0x81); break;
        default: PORTB = 0x00; break;
    }
    return state;
}

unsigned char pass[5] = {'1', '2', '3', '4', '5'};
unsigned char pass_index = 0x00;

enum pass_states {pass_wait, pass_start, pass_input, pass_input_press, pass_unlock};

int check_pass(int state) {
    switch(state) {
        case pass_wait:
            if(pad == '#') {
                state = pass_start;
            } else state = pass_wait;
            pass_index = 0x00;
            break;
        case pass_start: 
            if (pad == '#') state = pass_start;
            else if (pad == '\0') state = pass_input;
            else state = pass_wait;
            break;
        case pass_input: 
            if(pad == '\0') state = pass_input;
            else if (pad == pass[pass_index]) {
                if(pass_index == 4) {
                    state = pass_unlock;
                } else {
                    pass_index += 1;
                    state = pass_input_press;
                }
            }
            else {
                state = pass_wait;
            }
            break;
        case pass_input_press: 
            if (pad == '\0') state = pass_input;
            else state = pass_input_press;
            break;
        case pass_unlock:
            lock_signal = 0x00;
            if (pad == '\0') state = pass_wait;
            else state = pass_unlock;
        default: state = pass_wait;
    }
    return state;
}
enum lock_states {lock_wait, lock_pressed};
int check_lock(int state) {
    unsigned char input = (~PINB & 0x80);
    state = lock_wait;
    switch(state) {
        case lock_wait:
            if(input != 0x00){
                lock_signal = 0x01;
                state = lock_pressed;
            } else state = lock_wait;
            break;
        case lock_pressed: 
            if(input != 0x00) {
                state = lock_pressed;
            } else state = lock_wait;
        default: state = lock_wait;
    }
    return state;
}

int display_lock(int state) {
    if(lock_signal == 0x00) {
        PORTB |= 0x01;
    } else PORTB &= 0xFE;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRB = 0x7F; PORTB = 0x80;
    DDRC = 0xF0; PORTC = 0x0F;
    /* Insert your solution below */
    static task task1, task2, task3, task4;
    task *tasks[] = {&task1, &task2, &task3, &task4};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
    const char start = -1;

    task1.state = start;
    task1.period = 50;
    task1.elapsedTime = task1.period;
    task1.TickFct = &keypad_tick;

    task2.state = start;
    task2.period = 50;
    task2.elapsedTime = task2.period;
    task2.TickFct = &check_pass;

    task3.state = start;
    task3.period = 50;
    task3.elapsedTime = task3.period;
    task3.TickFct = &check_lock;

    task4.state = start;
    task4.period = 50;
    task4.elapsedTime = task4.period;
    task4.TickFct = &display_lock;

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