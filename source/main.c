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

unsigned char pad = 0x00;
unsigned char lock_state = 0x01;
unsigned char lock_signal = 0x01; // 1 -> locked; 0 -> unlocked

void set_PWM(double frequency) {
    static double current_frequency;

    if (frequency != current_frequency) {
        if (!frequency) {TCCR3B &= 0x08;}
        else {TCCR3B |= 0x03;}

        if (frequency < 0.954) {OCR3A = 0xFFFF; }

        else if (frequency > 31250) { OCR3A = 0x0000; }

        else {OCR3A = (short)(8000000 / (128 * frequency)) - 1;}

        TCNT3 = 0;
        current_frequency = frequency;
    }
}

void PWM_on() {
    TCCR3A = (1 << COM3A0);

    TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);

    set_PWM(0);
}

void PWM_off() {
    TCCR3A = 0x00;
    TCCR3B = 0x00;
}

int keypad_tick(int state) {
    pad = GetKeypadKey();
    switch(pad) {
        case '\0': PORTB = 0x00 | (PINB & 0x81); break;
        case '1': PORTB = 0x20 | (PINB & 0x81); break;
        case '2': PORTB = 0x20 | (PINB & 0x81); break;
        case '3': PORTB = 0x20 | (PINB & 0x81); break;
        case '4': PORTB = 0x20 | (PINB & 0x81); break;
        case '5': PORTB = 0x20 | (PINB & 0x81); break;
        case '6': PORTB = 0x20 | (PINB & 0x81); break;
        case '7': PORTB = 0x20 | (PINB & 0x81); break;
        case '8': PORTB = 0x20 | (PINB & 0x81); break;
        case '9': PORTB = 0x20 | (PINB & 0x81); break;
        case 'A': PORTB = 0x20 | (PINB & 0x81); break;
        case 'B': PORTB = 0x20 | (PINB & 0x81); break;
        case 'C': PORTB = 0x20 | (PINB & 0x81); break;
        case 'D': PORTB = 0x20 | (PINB & 0x81); break;
        case '*': PORTB = 0x20 | (PINB & 0x81); break;
        case '0': PORTB = 0x20 | (PINB & 0x81); break;
        case '#': PORTB = 0x20 | (PINB & 0x81); break;
        default: PORTB = 0x00; break;
    }
    return state;
}

unsigned char pass[5] = {'1', '2', '3', '4'};
unsigned char pass_index = 0x00;
unsigned char new_pass[4] = {'\0', '\0', '\0', '\0'};
unsigned char new_index = 0x00;

enum pass_states {pass_wait, pass_start, pass_input, pass_input_press, pass_unlock, pass_set, pass_set_wait};

int check_pass(int state) {
    unsigned char input = ~PINB & 0x80;
    switch(state) {
        case pass_wait:
            if(pad == '#') {
                if (input != 0x00) {
                    state = pass_set;
                } else state = pass_start;
            } else state = pass_wait;
            pass_index = 0x00;
            break;
        case pass_start: 
            if (pad == '#') {
                if (input != 0x00) {
                    state = pass_set;
                } else state = pass_start;
            }
            else if (pad == '\0') state = pass_input;
            else state = pass_wait;
            break;
        case pass_set:
            if(input != 0x00 && new_index < 4){
                new_index = 0x00;
                for (unsigned char i = 0; i < 4; i++) {
                    new_pass[i] = '\0';
                }
                state = pass_wait;
            } else if (input != 0x00 && new_index >= 4) {
                state = pass_check_                
            } else {
                if(pad == '#' || pad == '\0') state = pass_wait;
                else {
                    new_pass[i] = pad;
                    new_index += 1;
                    state = pass_set_wait;
                }
            }
            break;
        case pass_set_wait: 
            if(input!= 0x00) {
                new_index = 0x00;
                for (unsigned char i = 0; i < 4; i++) {
                    new_pass[i] = '\0';
                }
                state = pass_wait;
            } else if (pad == '\0') state = pass_set;
            else state = pass_set_wait;
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
    return state;
}

unsigned char bell_timer = 0x0C;
double song[11] = {440, 349.23, 392.00, 261.63, 261.63, 0, 261.63, 392, 440, 349.23, 349.23};
unsigned char bell_index = 0x00;
unsigned char bell_time = 0x0F;

enum bell_states {bell_wait, bell_play};

int doorbell_tick(int state) {
    unsigned char input = (~PINA & 0x80);
    switch(state) {
        case bell_wait: 
            if (input != 0x00) {
                state = bell_play;
            } else state = bell_wait;
            break;
        case bell_play:
            if (bell_time <= bell_timer) {
                bell_time += 1;
                state = bell_play;
            } else if (bell_index <= 11) {
                bell_time = 0;
                set_PWM(song[bell_index]);
                bell_index += 1;
                state = bell_play;
            } else {
                bell_time = 0;
                bell_index = 0;
                set_PWM(0);
                state = bell_wait;
            }
            break;
        default: state = bell_wait;
    }
    return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0x5F; PORTB = 0xA0;
    DDRC = 0xF0; PORTC = 0x0F;
    /* Insert your solution below */
    static task task1, task2, task3, task4, task5;
    task *tasks[] = {&task1, &task2, &task3, &task4, &task5};
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

    task5.state = start;
    task5.period = 50;
    task5.elapsedTime = task5.period;
    task5.TickFct = &doorbell_tick;

    unsigned long GCD = tasks[0]->period;
    for(unsigned long i = 1; i < numTasks; i++){
        GCD = findGCD(GCD, tasks[i]->period);
    }

    TimerSet(GCD);
    TimerOn();
    PWM_on();

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