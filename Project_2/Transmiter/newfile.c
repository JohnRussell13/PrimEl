#include <p30fxxxx.h>
#include <outcompare.h>

#define PR_REG_VAL 2499

_FOSC(CSW_FSCM_OFF & XT_PLL4);//instruction takt je isti kao i kristal
_FWDT(WDT_OFF);


unsigned int broj1,broj2,r0, r1;

unsigned char tempRX[8],  n=0, lights = 1, fat = 0, temp, ww[6];
unsigned int count = 0;

void initREG(){
    ADPCFG |= 0b0000001111111;//kazemo da nije analogni vec digitalni pin
    TRISB |= 0b0000001111111;//konfigurisemo kao ulaz
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
} 
void WriteUART1(unsigned int data)
{
	  while(!U1STAbits.TRMT);

    if(U1MODEbits.PDSEL == 3)
        U1TXREG = data;
    else
        U1TXREG = data & 0xFF;
}

int main(void){
    
    initREG();
    initUART1();

	while(1){
        if(1){
            WriteUART1(0);
            WriteUART1(0);
            WriteUART1(0);
            WriteUART1(0);
        }
        
        if(1){
            if(((~PORTB) & 0b0000001111111) == 0){
                WriteUART1(0xFF);
            }
        
            else{
                WriteUART1(((~PORTB) & 0b0000001111111));
            }
        }
        
        
        //WriteUART1(((~PORTB) & 0b0000001111111));
    }//od whilea
return 0;
}//od maina


//za 100% mora se staviti OC1RS>PR2 ili koji je vec izabran

//za 0% dovoljno je staviti OC1RS na 0 
//rezolucija pwma zavisi od izabrane periode


