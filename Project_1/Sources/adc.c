#include <p30fxxxx.h>
#include "adc.h"

void ConfigureADCPins(void){
    //use pins B8 and B9 for ADC (B12 is also used, but configurated outside)
	ADPCFGbits.PCFG8 = 0;
	ADPCFGbits.PCFG9 = 0;
	TRISBbits.TRISB8 = 1;
	TRISBbits.TRISB9 = 1;
}

void ADCinit(void){
    //AD CONFIG
    ADCON1bits.ADSIDL = 0;//what to do in idle mode
    ADCON1bits.FORM = 0;//output is of int type
    ADCON1bits.SSRC = 7;//auto convert
    ADCON1bits.SAMP = 1;//ADC is sampling
    ADCON2bits.VCFG = 7;//voltage ref
    ADCON2bits.CSCNA = 1;//scan inputs from channels CH0x
    ADCON2bits.SMPI = 2;//interrupts at the completion of conversion for each 3rd sample/convert sequence
    ADCON2bits.BUFM = 0;//fill LSB part of a buffer
    ADCON2bits.ALTS = 0;//use only channel A
    ADCON3bits.SAMC = 31;//time between sampling and conversion
    ADCON3bits.ADRC = 1;//internal clock
    ADCON3bits.ADCS = 31;//period of conversion

    //AD INPUT
    //since ALTS = 0, B channel is not important
    ADCHSbits.CH0NB = 0;
    ADCHSbits.CH0NA = 0;
    ADCHSbits.CH0SA = 0;
    ADCHSbits.CH0SB = 0;

    //AD INPUT CONFIG
    ADPCFG = 0;//set all a/d pins as analogs (this can and will be overwritten)
    ADCSSL = 0b0001001100000000;//scan selection register - which analog inputs are to be ADC
    
    ADCON1bits.ASAM = 1;//sampling begins immediately after last conversion completes

    //interrupt
    IFS0bits.ADIF = 1;//on start, flag is up?
    IEC0bits.ADIE = 1;//enable interrupts
}

