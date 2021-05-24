#include <p30fxxxx.h>
#include "timer2.h"
#define TMR2_period 1000 //used for PR2

void Init_T2(void){
	TMR2 = 0;//start value of timer2 register
	PR2 = TMR2_period;//when TMR2 == PR2 we have an interrupt
	T2CONbits.TCS = 0;//timer clock source is internal
	IFS0bits.T2IF = 0;//on start, flag is down
	IEC0bits.T2IE = 1;//enable interrupts for timer2
	T2CONbits.TON = 1;//start timer
}