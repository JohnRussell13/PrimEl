#include <p30fxxxx.h>
#include "tajmeri.h"

_FOSC(CSW_FSCM_OFF & XT_PLL4);
_FWDT(WDT_OFF);

unsigned int brojac_ms,stoperica,ms,sekund,j,i, x, y;
unsigned char tempRX, start, fl;

unsigned int x0, x1, x2, m;

void Delay_ms (int vreme){
	stoperica = 0;
	while(stoperica < vreme);
}

void __attribute__ ((__interrupt__)) _T2Interrupt(void){ // svakih 1ms
    TMR2 =0;
    stoperica++;//brojac za funkciju Delay_ms
	IFS0bits.T2IF = 0; 
}

void initUART1(void){
    U1BRG=0x0040;//ovim odredjujemo baudrate
    U1MODEbits.ALTIO=1;//biramo koje pinove koristimo za komunikaciju osnovne ili alternativne
    IEC0bits.U1RXIE=1;//omogucavamo rx1 interupt
    U1STA&=0xfffc;
    U1MODEbits.UARTEN=1;//ukljucujemo ovaj modul
    U1STAbits.UTXEN=1;//ukljucujemo predaju
}

void __attribute__((__interrupt__)) _U1RXInterrupt(void) {
    IFS0bits.U1RXIF = 0;
    tempRX=U1RXREG;
}

void WriteUART1(unsigned int data){
	  while(!U1STAbits.TRMT);

    if(U1MODEbits.PDSEL == 3)
        U1TXREG = data;
    else
        U1TXREG = data & 0xFF;
}

void WriteUART1dec2string(unsigned long data){
	j = 1;
    while(1){
        if(data / j < 10){
            break;
        }
        j = j * 10;
    }
    i = data / j;
    while(i >= 0 && i < 10){
        WriteUART1('0' + i);
        data = data % j;
        j = j / 10;
        i = data / j;
    }
}

void rotateR(){
    LATBbits.LATB0 = 1;
    LATBbits.LATB5 = 0;
    LATBbits.LATB2 = 1;
    LATBbits.LATB3 = 0;
    Delay_ms(60000);
    Delay_ms(5000);
    LATBbits.LATB0 = 0;
    LATBbits.LATB5 = 0;
    LATBbits.LATB2 = 0;
    LATBbits.LATB3 = 0;
}

void rotateL(){
    LATBbits.LATB0 = 0;
    LATBbits.LATB5 = 1;
    LATBbits.LATB2 = 0;
    LATBbits.LATB3 = 1;
    Delay_ms(60000);
    Delay_ms(5000);
    LATBbits.LATB0 = 0;
    LATBbits.LATB5 = 0;
    LATBbits.LATB2 = 0;
    LATBbits.LATB3 = 0;
}

void straight(){
    LATBbits.LATB0 = 1;
    LATBbits.LATB5 = 0;
    LATBbits.LATB2 = 0;
    LATBbits.LATB3 = 1;
}

void stop(){
    LATBbits.LATB0 = 0;
    LATBbits.LATB5 = 0;
    LATBbits.LATB2 = 0;
    LATBbits.LATB3 = 0;
}

void main (void){
    initUART1();
    
    ADPCFGbits.PCFG1=1;//defini?emo da pin B4 nije analogni vec digitalni
	ADPCFGbits.PCFG8=1;//defini?emo da pin B5 nije analogni vec digitalni
    ADPCFGbits.PCFG10=1;//defini?emo da pin B4 nije analogni vec digitalni
	ADPCFGbits.PCFG11=1;//defini?emo da pin B5 nije analogni vec digitalni
    
    ADPCFGbits.PCFG0=1;//defini?emo da pin B4 nije analogni vec digitalni
	ADPCFGbits.PCFG2=1;//defini?emo da pin B5 nije analogni vec digitalni
    ADPCFGbits.PCFG3=1;//defini?emo da pin B4 nije analogni vec digitalni
	ADPCFGbits.PCFG5=1;//defini?emo da pin B5 nije analogni vec digitalni

	TRISBbits.TRISB1=1;//konfigurisemo kao ulaz
	TRISBbits.TRISB8=0;//konfigurisemo kao izlaz
	TRISBbits.TRISB10=0;//konfigurisemo kao izlaz
	TRISBbits.TRISB11=1;//konfigurisemo kao ulaz
    
	TRISBbits.TRISB0=0;//konfigurisemo kao izlaz
	TRISBbits.TRISB2=0;//konfigurisemo kao izlaz
	TRISBbits.TRISB3=0;//konfigurisemo kao izlaz
	TRISBbits.TRISB5=0;//konfigurisemo kao izlaz
    
	TRISDbits.TRISD1=0;//konfigurisemo kao izlaz
	TRISDbits.TRISD2=0;//konfigurisemo kao izlaz

    Init_T2();
    LATBbits.LATB8=0;
    LATBbits.LATB10=0;
    
    //EN
    LATDbits.LATD1 = 1;
    LATDbits.LATD2 = 1;
    stop();
    
    start = 0;
    fl = 1;
    x0 = 0;
    x1 = 0;
    x2 = 0;
	while(1)
	{
        //s = start
        if(tempRX == 's'){
            start = 1;
        }
        //p = pause
        if(tempRX == 'p'){
            start = 0;
            stop();
        }
        //sensor reading
        LATBbits.LATB8=1;
        LATBbits.LATB10=1;
        Delay_ms(1);
        LATBbits.LATB8=0;
        LATBbits.LATB10=0;
       
        while(PORTBbits.RB1 == 0);
        stoperica = 0;
        x=0;
        y=0;
        while(x == 0 || y == 0){
            if(PORTBbits.RB1 == 0 && x == 0){
                x = stoperica;
            }
            if(PORTBbits.RB11 == 0 && y == 0){
                y = stoperica;
            }
        }
        
        //Bluetooth send
        WriteUART1('x');
        WriteUART1dec2string(x/6);
        WriteUART1(' ');
        WriteUART1('y');
        WriteUART1dec2string(y/6);
        WriteUART1(10);
        
        x2 = x1;
        x1 = x0;
        x0 = x;
        
        if(start){
            if(fl){
                straight();
                if(y < 60){
                    rotateR();
                    fl = 0;
                    x0 = 0;
                    x1 = 0;
                    x2 = 0;
                }
            }

            else{
                WriteUART1('h');
                WriteUART1(' ');
                WriteUART1dec2string(x0);
                WriteUART1(' ');
                WriteUART1dec2string(x1);
                WriteUART1(' ');
                WriteUART1dec2string(x2);
                WriteUART1(10);
                
                
                    if(x0 > 180 && x1 > 180 && x2 > 180){
                        //passing the corner
                        straight();
                        Delay_ms(10000);
                        rotateL();

                        straight();
                        Delay_ms(60000);
                        Delay_ms(30000);
                        
                        WriteUART1('L');
                        WriteUART1(10);
                        
                        x0 = 0;
                        x1 = 0;
                        x2 = 0;
                    }
                    else if(y > 60){
                        
                        if(x0 && (x0 < x1 && x1 < x2 || x0 < 40 && x1 < 40 && x2 < 40)){
                            LATBbits.LATB0 = 1;
                            LATBbits.LATB5 = 0;
                            LATBbits.LATB2 = 1;
                            LATBbits.LATB3 = 0;
                            Delay_ms(10000);
                            LATBbits.LATB0 = 0;
                            LATBbits.LATB5 = 0;
                            LATBbits.LATB2 = 0;
                            LATBbits.LATB3 = 0;
                            x0 = 0;
                            x1 = 0;
                            x2 = 0;
                            
                            WriteUART1('C');
                            WriteUART1('R');
                            WriteUART1(10);
                        }
                        else if(x2 && (x0 > x1 && x1 > x2 || x0 > 50 && x1 > 50 && x2 > 50)){
                            LATBbits.LATB0 = 0;
                            LATBbits.LATB5 = 1;
                            LATBbits.LATB2 = 0;
                            LATBbits.LATB3 = 1;
                            Delay_ms(10000);
                            LATBbits.LATB0 = 0;
                            LATBbits.LATB5 = 0;
                            LATBbits.LATB2 = 0;
                            LATBbits.LATB3 = 0;
                            x0 = 0;
                            x1 = 0;
                            x2 = 0;
                            
                            WriteUART1('C');
                            WriteUART1('L');
                            WriteUART1(10);
                        }
                        straight();
                        Delay_ms(10000);
                    }
                    else{
                        rotateR();
                        
                        WriteUART1('R');
                        WriteUART1(10);
                        
                        x0 = 0;
                        x1 = 0;
                        x2 = 0;
                    }
            }
        }
        Delay_ms(5000);
	}//while(1)



}//main


