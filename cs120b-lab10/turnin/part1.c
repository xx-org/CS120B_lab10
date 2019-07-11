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
enum cStates {cstart,coutput} cstate;

unsigned short acount = 0;
const unsigned short atime = 125;
unsigned short bcount = 0;
const unsigned short btime = 125;
//unsigned short ccount = 0;
//unsigned short ctime = 125;
void ThreeLEDsSM(){
	switch(astate){
		case astart:
			acount = atime;
			astate = astate1;
			break;
		case astate1:
			if(acount == 0){
				astate = astate2;
				acount = atime;
			}
			acount --;
			break;
		case astate2:
			if(acount == 0){
				astate = astate3;
				acount = atime;
			}
			acount --;
			break;
		case astate3:
			if(acount == 0){
				astate = astate1;
				acount = atime;
			}
			acount --;
			break;				
		default:
			astate = astart;
			break;
	}
		switch(astate){
		case astart:
			break;
		case astate1:
				blinkingLED &=0xF8;
				blinkingLED |= 0x01;
			break;
		case astate2:
			if((blinkingLED&0x07)!=2){
				blinkingLED &=0xF8;
				blinkingLED |= 0x02;
			}
			break;
		case astate3:
			if((blinkingLED&0x07)!=4){
				blinkingLED &=0xF8;
				blinkingLED |= 0x04;
			}
			break;				
		default:
			break;
	}
	
}
void BlinkingLEDSM(){
	switch(bstate){
		case bstart:
			bstate = bstate1;
			bcount = btime;
			break;
		case bstate1:
			if(bcount ==0){
				bstate = bstate2;
				bcount = btime;
			}
			bcount --;
			break;
		case bstate2:
			if(bcount == 0){
				bstate = bstate1;
				bcount = btime;
			}
			bcount --;
			break;
		default:
			bstate = bstart;
			break;
	}
	switch(bstate){
		case bstart:
			break;
		case bstate1:
			blinkingLED |= 0x08;
			break;
		case bstate2:
			blinkingLED &= 0xF7;
			break;
		default:
			break;
			
	}
}

void CombineLEDsSM(){
	switch(cstate){
		case cstart:
			cstate = coutput;
			break;
		case coutput:
			ThreeLEDsSM();
			BlinkingLEDSM();
			break;
		default:
			cstate = cstart;
			break;
	}
	switch(cstate){
		case cstart:
			break;
		case coutput:
			PORTB = blinkingLED;
			break;
		default:
			break;
	}
}
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(1);
	TimerOn();
    /* Insert your solution below */
    while (1) {
		CombineLEDsSM();
		while(!TimerFlag){}
		TimerFlag = 0;
    }
    return 1;
}
