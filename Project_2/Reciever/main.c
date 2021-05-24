#include <p30fxxxx.h>
#include <outcompare.h>
#include "timer2.h"

#define PR_REG_VAL 2499

_FOSC(CSW_FSCM_OFF & XT_PLL4);//instruction takt je isti kao i kristal
_FWDT(WDT_OFF);


unsigned int broj1,broj2;

unsigned char tempRX0 = 0, tempRX1, tempRX, lights = 1, fat = 0, temp, r = 0;
unsigned int count = 0;

void initREG(){
    ADPCFG |= 0b1111111101101;//kazemo da nije analogni vec digitalni pin
    TRISB  &= 0b0000000010010;//konfigurisemo kao izlaz
    TRISDbits.TRISD1 = 0;
    TRISDbits.TRISD2 = 0;
    TRISDbits.TRISD3 = 0;
}
void initUART1(void)
{
    U1BRG=0x0207;//ovim odredjujemo baudrate
    U1MODEbits.ALTIO=1;//biramo koje pinove koristimo za komunikaciju osnovne ili alternativne
    IEC0bits.U1RXIE=1;//omogucavamo rx1 interupt
    U1STA&=0xfffc;
    U1MODEbits.UARTEN=1;//ukljucujemo ovaj modul
    U1STAbits.UTXEN=1;//ukljucujemo predaju
}
void __attribute__((__interrupt__)) _U1RXInterrupt(void) 
{
    IFS0bits.U1RXIF = 0;
    temp = U1RXREG;
    if(temp) {
        tempRX1 = tempRX0;
        tempRX0 = temp;
        if(tempRX0 == tempRX1) tempRX = temp;
    }
} 
void WriteUART1(unsigned int data)
{
	  while(!U1STAbits.TRMT);

    if(U1MODEbits.PDSEL == 3)
        U1TXREG = data;
    else
        U1TXREG = data & 0xFF;
}


void __attribute__((__interrupt__)) _T2Interrupt(void)
{
   	TMR2 = 0;
    IFS0bits.T2IF = 0;
}

void initPWM(){
    
    PR2 = 2499;//odredjuje frekvenciju po formuli
    T2CONbits.TON = 1;//ukljucujemo timer koji koristi
    
    OC2RS = 20;//postavimo pwm
    OC2R = 1000;//inicijalni pwm pri paljenju samo
    OC2CON = OC_IDLE_CON & OC_TIMER2_SRC & OC_PWM_FAULT_PIN_DISABLE& T2_PS_1_256;//konfiguracija pwma
    
    OC3RS=20;//postavimo pwm
    OC3R=1000;//inicijalni pwm pri paljenju samo
    OC3CON =OC_IDLE_CON & OC_TIMER2_SRC & OC_PWM_FAULT_PIN_DISABLE& T2_PS_1_256;//konfiguracija pwma
    
    OC4RS=20;//postavimo pwm
    OC4R=1000;//inicijalni pwm pri paljenju samo
    OC4CON =OC_IDLE_CON & OC_TIMER2_SRC & OC_PWM_FAULT_PIN_DISABLE& T2_PS_1_256;//konfiguracija pwma
    
}

int main(void){
    
    initREG();
    initUART1();
    initPWM();
    
    LATBbits.LATB6 = 1;
    LATBbits.LATB8 = 1;
    LATBbits.LATB9 = 0;
    LATBbits.LATB10 = 1;
    LATBbits.LATB11 = 1;
    LATBbits.LATB12 = 0;

	while(1){
        
        
        if((tempRX) == 0xFF){
            LATBbits.LATB0 = 0;
            LATBbits.LATB5 = 0;
            OC4RS = 0;
            LATBbits.LATB2 = 0;
            LATBbits.LATB3 = 0;
            OC2RS = 0;
            
            LATBbits.LATB8 = 1;
            LATBbits.LATB12 = 0;
            LATBbits.LATB6 = 1;
            LATBbits.LATB9 = 0;
                lights = 1;
        }
        else{
            if((tempRX & 0x20) == 0x20){
                if(lights == 1){
                    lights = 0;
                    LATBbits.LATB10 = ~LATBbits.LATB10;
                    LATBbits.LATB11 = ~LATBbits.LATB11;
                }
            }
            else{
                lights = 1;
            }
            
            if((tempRX & 0x40) == 0x40){
                OC4RS = 1900;
            }
            else{
                OC4RS = 0;
            }
            
            if((tempRX & 0b00000101) == 0b00000101){//spinL
                LATBbits.LATB0 = 0;
                LATBbits.LATB5 = 1;
                OC3RS = 1900;
                LATBbits.LATB2 = 0;
                LATBbits.LATB3 = 1;
                OC2RS = 1900;
            
                LATBbits.LATB8 = 1;
                LATBbits.LATB12 = 0;
                LATBbits.LATB6 = 1;
                LATBbits.LATB9 = 1;
            }
            else if((tempRX & 0b00001001) == 0b00001001){//spinR
                LATBbits.LATB0 = 1;
                LATBbits.LATB5 = 0;
                OC3RS = 1900;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 0;
                OC2RS = 1900;
            
                LATBbits.LATB8 = 1;
                LATBbits.LATB12 = 1;
                LATBbits.LATB6 = 1;
                LATBbits.LATB9 = 0;
            }
            else if((tempRX & 0b00000001) == 0b00000001){//down
                LATBbits.LATB0 = 0;
                LATBbits.LATB5 = 1;
                OC3RS = 1900;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 0;
                OC2RS = 1900;
            
                LATBbits.LATB8 = ~LATBbits.LATB8;
                LATBbits.LATB12 = 0;
                LATBbits.LATB6 = ~LATBbits.LATB6;
                LATBbits.LATB9 = 0;
            }
            else if((tempRX & 0b00001100) == 0b00001100){//left + right
                LATBbits.LATB0 = 0;
                LATBbits.LATB5 = 0;
                OC3RS = 0;
                LATBbits.LATB2 = 0;
                LATBbits.LATB3 = 0;
                OC2RS = 0;
            
                LATBbits.LATB8 = 1;
                LATBbits.LATB12 = 0;
                LATBbits.LATB6 = 1;
                LATBbits.LATB9 = 0;
            }
            else if((tempRX & 0b00000100) == 0b00000100){//left
                LATBbits.LATB0 = 0;
                LATBbits.LATB5 = 0;
                OC3RS = 0;
                LATBbits.LATB2 = 0;
                LATBbits.LATB3 = 1;
                OC2RS = 1900;
            
                LATBbits.LATB8 = 1;
                LATBbits.LATB12 = 0;
                LATBbits.LATB6 = ~LATBbits.LATB6;
                LATBbits.LATB9 = LATBbits.LATB6;
            }
            else if((tempRX & 0b00001000) == 0b00001000){//right
                LATBbits.LATB0 = 1;
                LATBbits.LATB5 = 0;
                OC3RS = 1900;
                LATBbits.LATB2 = 0;
                LATBbits.LATB3 = 0;
                OC2RS = 0;
            
                LATBbits.LATB8 = ~LATBbits.LATB8;
                LATBbits.LATB12 = LATBbits.LATB8;
                LATBbits.LATB6 = 1;
                LATBbits.LATB9 = 0;
            }
            else if((tempRX & 0b00000010) == 0b00000010){//up
                LATBbits.LATB0 = 1;
                LATBbits.LATB5 = 0;
                OC3RS = 1900;
                LATBbits.LATB2 = 0;
                LATBbits.LATB3 = 1;
                OC2RS = 1900;
            
                LATBbits.LATB8 = 1;
                LATBbits.LATB12 = 0;
                LATBbits.LATB6 = 1;
                LATBbits.LATB9 = 0;
            }
            else if((tempRX & 0b00010000) == 0b00010000){//boost
                LATBbits.LATB0 = 1;
                LATBbits.LATB5 = 0;
                OC3RS = 2500;
                LATBbits.LATB2 = 0;
                LATBbits.LATB3 = 1;
                OC2RS = 2500;
            
                LATBbits.LATB8 = 1;
                LATBbits.LATB12 = 0;
                LATBbits.LATB6 = 1;
                LATBbits.LATB9 = 0;
            }
            else{
                LATBbits.LATB0 = 0;
                LATBbits.LATB5 = 0;
                OC3RS = 0;
                LATBbits.LATB2 = 0;
                LATBbits.LATB3 = 0;
                OC2RS = 0;
            
                LATBbits.LATB8 = 1;
                LATBbits.LATB12 = 0;
                LATBbits.LATB6 = 1;
                LATBbits.LATB9 = 0;
            
            }
        
            //fatality
            if((tempRX & 0b00100001) == 0b00100001){
                if(fat == 0){
                    fat++;
                }
            }
            else if((tempRX & 0b00100100) == 0b00100100){
                if(fat == 1){
                    fat++;
                }
            }
            else if((tempRX & 0b00101000) == 0b00101000){
                if(fat == 2){
                    fat++;
                }
            }
            else if((tempRX | 0b11011111) == 0b11011111){
                fat = 0;
            }
        
            while(fat == 3){
                LATBbits.LATB0 = 1;
                LATBbits.LATB5 = 0;
                OC3RS = 2500;
                LATBbits.LATB2 = 1;
                LATBbits.LATB3 = 0;
                OC2RS = 2500;
                if(count == 0){
                    LATBbits.LATB6 = ~LATBbits.LATB6;
                    OC4RS = 1900;
                }
                if(count == 16384){
                    LATBbits.LATB8 = ~LATBbits.LATB8;
                    OC4RS = 0;
                }
                if(count == 32768){
                    LATBbits.LATB12 = ~LATBbits.LATB12;
                    OC4RS = 1900;
                }
                if(count == 49152){
                    LATBbits.LATB9 = ~LATBbits.LATB9;
                    OC4RS = 0;
                }
                count++;
                if((tempRX & 0b00000010) == 0b00000010 && tempRX != 0xFF){
                    fat = 0;
                    LATBbits.LATB6 = 1;
                    LATBbits.LATB8 = 1;
                    LATBbits.LATB10 = 1;
                    LATBbits.LATB11 = 1;
                    lights = 1;
                }
            }
        }
      
        WriteUART1(tempRX);
        for(broj1 = 0; broj1 < 400; broj1++)
            for(broj2 = 0; broj2 < 500; broj2++);
    }//od whilea
return 0;
}//od maina


//za 100% mora se staviti OC1RS>PR2 ili koji je vec izabran

//za 0% dovoljno je staviti OC1RS na 0 
//rezolucija pwma zavisi od izabrane periode


