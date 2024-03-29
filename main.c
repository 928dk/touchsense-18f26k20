/*
 * File:   main.c
 * Author: bg25382
 *
 * Created on September 3, 2012, 8:32 AM
 *
 * (c) ConLan Aps
 * 
 *
 * 
 *
 */

#include <p18f26k20.h>
#include <htc.h>
#include "mtouch.h"
#include <delays.h>
#include <stdio.h> // for uart etc
#include <stdlib.h> //for strings etc
#include <stdint.h> //standard integer types per ISO/IEC 9899:1999


//#include "inttypes.h" 
//#include <GenericTypeDefs.h>
//#include <math.h>
//#include <signal.h>
//#include <stdarg.h>
//#include <string.h>
//#include <time.h>

#pragma config FOSC = INTIO67   // Internal Oscilator block
#pragma config PLLCFG = ON      // PLLCFG Oscillator multiplied by 4
#pragma config PRICLKEN = ON    // Primary clock enabled
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor disabled
#pragma config IESO = OFF       // Oscillator Switchover mode disabled
#pragma config WDTEN = OFF      // Watchdog Timer enabled = off
#pragma config BOREN = OFF      // Brown-out Reset disabled in hardware and software
#pragma config LVP = OFF        // Single supply ICSP Enabled = OFF
#pragma config DEBUG = ON       //
#pragma config PBADEN= OFF      // Port B AD converter off,
#pragma config PWRTEN = OFF      // Power Up Timer enabled (Page34)

// configword 0x300008
#pragma config CP0 = OFF //CP0: Code Protection bit 1 = Block 0 not code-protected 0 = Block 0 code-protected
#pragma config CP1 = OFF //CP1: Code Protection bit 1 = Block 1 not code-protected 0 = Block 1 code-protected

// configword 0x300009
#pragma config CPB = OFF //CPB: Boot Block Code Protection bit
#pragma config CPD = OFF //CPD: Data EEPROM Code Protection bit

// configword 0x30000a
#pragma config WRT0 = OFF //WRT0: Write Protection bit
#pragma config WRT1 = OFF //WRT1: Write Protection bit

// configword 0x30000b
#pragma config WRTB = OFF //WRTB:Boot Block Write Protection bit
#pragma config WRTC = OFF //WRTC: Configuration Register Write Protection bit
#pragma config WRTD = OFF //WRTD: Data EEPROM Write Protection bit

// configword 0x30000c
#pragma config EBTR0 = OFF //EBTR0: Table Read Protection bit
#pragma config EBTR1 = OFF //EBTR1: Table Read Protection bit

// configword 0x30000d
#pragma config EBTRB = OFF //Boot Block Table Read Protection bit

//#####################################
typedef unsigned		u1;
//typedef unsigned short char     u4;     //nibble, semioctet
typedef unsigned char		u8;     //byte,octet
typedef unsigned short		u16;    //unsigned int,halfword,word,short
typedef unsigned short long	u24;
typedef unsigned long		u32;    //word,long
typedef unsigned long long      u64;    //long long, quad, quadword, int6
//typedef unsigned double double  u128;   //octaword, double quadword (nonstandart)

//typedef signed                  s1;
//typedef signed short char       s4;     //nibble, semioctet
typedef signed char		s8;     //byte,octet
typedef signed short		s16;    //signed int,halfword,word,short
typedef signed short long	s24;
typedef signed long		s32;    //word,long
typedef signed long long        s64;    //long long, quad, quadword, int6
//typedef signed double double    s128;   //octaword, double quadword (nonstandart)
/*

char                8-bit  (signed by default)
signed char         8-bit (signed)
unsigned char       8-bit (unsigned)
int                 16 bit (signed)
unsigned int        16-bit (unsigned)
short               Same as int
unsigned short      Same as unsigned int
short long          24-bit (signed)
unsigned short long 24-bit (unsigned)
long                32-bit (signed)
unsigned long       32-bit (unsigned)

 * Signed kan holde  + og - (eks -128 til 128)
 * Unsigned kan kun holde positive v�rdier (eks 0 til 255)

 */

//Capacitivt input (analog)
#define TRIS_K0     TRISBbits.TRISB0
#define TRIS_K1     TRISBbits.TRISB1
#define TRIS_K2     TRISBbits.TRISB2

//Leds
#define	TRIS_RED	TRISAbits.TRISA4
#define	TRIS_GREEN	TRISAbits.TRISA3
#define	TRIS_BLUE	TRISAbits.TRISA2

#define	PORT_RED	PORTAbits.RA4
#define	PORT_GREEN	PORTAbits.RA3
#define	PORT_BLUE	PORTAbits.RA2

//Uart 1
#define TRIS_RX  TRISCbits.TRISC7
#define TRIS_TX  TRISCbits.TRISC6
#define PORT_RX  PORTCbits.RC7 //pin 26 Dual Inline package
#define PORT_TX  PORTCbits.RC6 //pin 25 Dual Inline package

void Delayx(int y);
void Delay10x(int y);
void Init26k20(void);
void InitTimer1(void);
void InitTimer3(void);
void InitUart1(void);

void Init_IO(void);

 //Lightshow (for debug)
 void lightshow(u16 on,u16 off)
 {

PORT_BLUE=1;PORT_GREEN=1;PORT_RED=0; //only red on
Delayx(on);

PORT_BLUE=1;PORT_GREEN=1;PORT_RED=1;//all led's off
Delayx(off);

PORT_BLUE=1;PORT_GREEN=0;PORT_RED=1;//only green on
Delayx(on);

PORT_BLUE=1;PORT_GREEN=1;PORT_RED=1;//all led's off
Delayx(off);

PORT_BLUE=0;PORT_GREEN=1;PORT_RED=1;//only blue on
Delayx(on);

PORT_BLUE=1;PORT_GREEN=1;PORT_RED=1;//all led's off
Delayx(off);

 }

int main(void)//p18F #org 0x0200 w.booter {

{
 Init26k20();
 
INTCONbits.GIE = 1;
INTCONbits.PEIE = 1;


 do//Schedule start:
 {
 lightshow(200,2000);




 }while(1);//~Schedule

    return 0;
}//~Schedule (Main)#####################


void Init26k20(){

    //OSC int 16MHz (62.5ns/inst.)
OSCCONbits.IRCF2 = 1;
OSCCONbits.IRCF1 = 1;
OSCCONbits.IRCF0 = 1;
OSCCONbits.SCS1 = 1;

while(!OSCCONbits.IOFS); //wait until ready

//dioder

ANSEL=0x00; //set ports som digitale
ANSELH=0x00;

//input
//ANSELBbits.ANSB0 = 1; //Port B0 som analog
//ANSELBbits.ANSB1 = 1; //Port B1 som analog
//ANSELBbits.ANSB2 = 1; //Port B2 som analog


//InitTimer0();
InitTimer1();
//InitTimer2();
InitTimer3();

InitUart1();

Init_IO();


}

void Init_IO(void)
{
//Tris v�lger om det er input(1) eller output (0)
//Med Port kan man skrive 0 eller 1 hvis pin er valgt som output
//hvis port er valgt som input kan man l�se om der er 0 eller 1 p� pin

//Leds
TRIS_RED = 0;
TRIS_GREEN = 0;
TRIS_BLUE = 0;
PORT_RED = 1;
PORT_GREEN = 1;
PORT_BLUE = 1;

//Keypad
TRIS_K0 = 1;
TRIS_K1 = 1;
TRIS_K2 = 1;


//Uart1
//PORT_RX = 0;
//PORT_TX = 0;
TRIS_RX = 1;
TRIS_TX = 0;

}

void InitTimer0(void)
{
//T0CON:  TMR0ON T08BIT T0CS T0SE PSA T0PS2 T0PS1 T0PS0
T0CON	=0x00;  //

T0CONbits.T0CS = 1;   //Clk = Fosc
T0CONbits.T0PS = 4;   //PreScaler
T0CONbits.TMR0ON = 1;   //timer 0 on


INTCONbits.TMR0IF = 0;//timer 0 interrupt flag (clear = 0)
INTCONbits.TMR0IE =1; //timer 0 interrupt enable

}

void InitTimer1(void)
{
TMR1H = 0x00;//high byte
TMR1L = 0x00; //low byre
T1CONbits.TMR1CS = 1;   //Clk = Fosc
T1CONbits.T1CKPS = 4;   //PreScaler

T1CONbits.TMR1ON = 1;   //timer 1 on
PIR1bits.TMR1IF = 0;    //timer 1 interrupt flag (clear = 0)
PIE1bits.TMR1IE = 1;    //timer 1 interrupt enable
}

void InitTimer2(void)
{
//T2OUTPS<3:0>
//T2CKPS<1:0>
T2CON=0x1D;  //4Khz PWM, 1ms TMR2IF
T2CONbits.T2CKPS = 4;
T2CONbits.TMR2ON = 1;

}

void InitTimer3(void)
{
TMR3L = 0x00;
T3CONbits.TMR3CS = 1;   //Clk = Fosc
T3CONbits.T3CKPS = 4;   //PreScaler
T3CONbits.T3RD16 = 0;   //16 bits = 0 = 8bits
T3CONbits.TMR3ON = 1;   //timer 3 on

//PIR2bits.TMR3IF = 0;
//PIR2bits.

PIR2bits.TMR3IF = 0;    //timer 3 interrupt flag (clear = 0)
PIE2bits.TMR3IE = 1;    //timer 3 interrupt enable

}

void InitUart1(void)
{
RCSTA1bits.SPEN = 0; //disable serial port
PIE1bits.TXIE = 0; //tx interrupt enable

TXSTA1bits.TX9 = 0; //tx not 9 bit (8bits)
TXSTA1bits.TXEN = 1; //Enable transmit
TXSTA1bits.SYNC = 0; //Async
TXSTA1bits.BRGH = 1; //High speed

RCSTA1bits.RX9 = 0; //8bits rx
RCSTA1bits.CREN = 1; //Continuous reception

BAUDCONbits.BRG16 = 1; //1 = 16bits
SPBRG = 8; //low byte
SPBRGH = 34; //high byte
BRGH = 1;

PIR1bits.RCIF = 0; //interrupt flag clear
PIR1bits.TXIF = 0; //interrupt flag clear

PIE1bits.RCIE = 1; //rx interrupt enable
PIE1bits.TXIE = 0; //tx interrupt enable

RCSTA1bits.SPEN = 1; //Enable serial port
}


void Delayx(int y){
    int x=0;
        for (x=0; x<y; x=x+1) {
            asm("nop ");
     }
}

void Delay10x(int y){
    int x=0;
        for (x=0; x<y; x=x+1) {
            asm("nop ");
            asm("nop ");
            asm("nop ");
            asm("nop ");
            asm("nop ");
            asm("nop ");
            asm("nop ");
            asm("nop ");
            asm("nop ");
            asm("nop ");
    }
}