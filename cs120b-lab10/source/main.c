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

void set_PWM(double frequency){
	static double current_frequency;
	if(frequency != current_frequency){
		if(!frequency){TCCR3B &= 0x08;}
		else{TCCR3B |=0x03;}
		if(frequency < 0.954){OCR3A =0xFFFF;}
		else if(frequency > 31250){OCR3A =0x0000;}
		else{OCR3A =(short)(8000000/(128*frequency))-1;}
		
		TCNT3 =0;
		current_frequency=frequency;
	}
}

void PWM_on(){
	TCCR3A = (1<<COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31)|(1<<CS30);
	set_PWM(261.63);
}

void PWM_off(){
	TCCR3A =0x00;
	TCCR3B = 0x00;
}
double C = 2610.63;
double D = 2930.66;
double E = 3290.63;
double F = 3490.23;
double G = 3920.00;
double A = 4400.00;
double B = 4930.88;
double C5 = 5230.25;
double nodes[8] = { 2610.63,2930.66,3290.63,3490.23,3920.00,4400.00,4930.88,5230.25};
unsigned char i;
enum states {start, wait, pressA0, pressA1, pressA2,pressing} state;
unsigned int count;
unsigned char on;
const unsigned int timer = 30;
void tick(){
	unsigned char A = ~PINA;
	unsigned char A0 = ~PINA &0x01;
	unsigned char A1 = ~PINA & 0x02;
	unsigned char A2 = ~PINA & 0x04;
	switch(state){
		case start:
		state = wait;
		break;
		case wait:
		if(A0 && ~A1 && ~A2)
		state = pressA0;
		else if(~A0 && A1 && ~A2)
		state = pressA1;
		else if(~A0 && ~A1 && A2)
		state = pressA2;
		else state = wait;
		break;
		case pressA0:
		state = pressing;
		break;
		case pressA1:
		state = pressing;
		break;
		case pressA2:
		state = pressing;
		break;
		case pressing:
		if(A) state = pressing;
		else state = wait;
		break;
		default:
		state =start;
		break;
	}
	switch(state){
		case start:
		break;
		case wait:
		break;
		case pressA0:
		if(on == 0){
		PWM_on();
		i = 0;
		set_PWM(nodes[i]);
		on = 1;
		}else{
			PWM_off();
			on = 0;
		}
		break;
		case pressA1:
		if(i < 7)
			set_PWM(nodes[++i]);
		break;
		case pressA2:
		if(i > 0)
			set_PWM(nodes[--i]);
		break;
		case pressing:
		break;
		default:
		break;
	}
}

unsigned char blinkingLED = 0x00;
enum aStates {astart,astate1, astate2, astate3} astate;
enum bStates {bstart, bstate1, bstate2} bstate;
enum cStates {cstart,coutput} cstate;

unsigned short acount = 0;
const unsigned short atime = 37;
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
			tick();
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
    DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(1);
	TimerOn();
	count = 0;
	state = start;
    /* Insert your solution below */
    while (1) {
		CombineLEDsSM();
		while(!TimerFlag){}
		TimerFlag = 0;
    }
    return 1;
}
