/*	Author: jxie031
 *  Partner(s) Name: Xichao Wang
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn(){
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff(){
	TCCR1B = 0x00;
}

void TimerISR(){
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr ==0){
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}
unsigned char blinkingLED = 0x00;
enum aStates {astart,astate1, astate2, astate3} astate;
enum bStates {bstart, bstate1, bstate2} bstate;
enum cStates {cstart} cstate;

unsigned short acount = 0;
unsigned short bcount = 0;
unsigned short ccount = 0;
void ThreeLEDsSM(){
	switch(astate){
		case astart:
			astate = astate1;
			break;
		case astate1:
			if(acount == 0)
				astate = astate2;
			break;
		case astate2:
			if(acount == 0)
				astate = astate3;
			break;
		case astate3:
			if(acount == 0)
				astate = astate1;
			break;				
		default:
			astate = astart;
			break;
	}
		switch(astate){
		case astart:
			break;
		case astate1:
			if(acount == 0)
				astate = astate2;
			break;
		case astate2:
			if(acount == 0)
				astate = astate3;
			break;
		case astate3:
			if(acount == 0)
				astate = astate1;
			break;				
		default:
			astate = astart;
			break;
	}
	
}
void BlinkingLEDSM(){
	switch(bstate){
		case bstart:
			bstate = bstate1;
			break;
		case bstate1:
			bstate = bstate2;
			break;
		case bstate2:
			bstate = bstate1;
			break;
		default:
			bstate = bstart;
			break;
	}
	switch(bstate){
		case bstart:
			break;
		case bstate1:
			
	}
}
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
	
    /* Insert your solution below */
    while (1) {

    }
    return 1;
}
