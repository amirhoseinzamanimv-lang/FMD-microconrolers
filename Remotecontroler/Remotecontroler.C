//Project: Remotecontroler.prj
// Device: FT61EC2X
// Memory: Flash 2KX14b, EEPROM 256X8b, SRAM 128X8b
// Author: Amirhosein zamani
//Company: SEPAHBOD
//Version:
//   Date: 
//===========================================================
#include "SYSCFG.h"
#define  LED1    PC3
#define  RX      PC1
#define  Button1 PA5
#define  unchar  unsigned char
////3
#define Relay1  PC4
#define Relay2  PC5
#define Relay3  PC0
////3
/* -- variables ----------------------------------------------------- */
static unsigned char code[24] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
 //////3
volatile unsigned int learnStep = 0;   // 0=none, 1=learn ch1, 2=ch2, 3=ch3
unsigned int learned[3][3];             // 3 channels x 3 code parts                                
 //////3                               
volatile unsigned char i             = 0;
volatile unsigned char EEReadData1   = 0;

volatile unsigned int saver1 = 0;
volatile unsigned int saver2 = 0;
volatile unsigned int saver3= 0;
volatile const int Timeout = 10000;
volatile unsigned int saver11 = 0;
volatile unsigned int saver22 = 0;
volatile unsigned int saver33 = 0;

/* Low, High, Index kept as globals to save stack on small MCU */
int Low, High, Index;

/* -- EEPROM -------------------------------------------------------- */
unsigned char EEADR1;

void POWER_INITIAL(void)

{
 
    OSCCON  = 0B01110001;   /* 8MHz internal                         */

    PORTA   = 0B00000000;
    TRISA   = 0B00100000;   /* PA5 input (Button1)  */
    INTCON = 0;  	        // Disable all interrupts
                 
    PORTC   = 0B00000010;
    TRISC   = 0B00000010;   /* PC1 input (RX)                        */
    WPUA    = 0B00100000;   /* pull-up on PA5                        */
    WPUC    = 0B00000000;   /* pull-up on PC1                        */

    ANSEL   = 0B00000000;

    OPTION  = 0B00000000;   /* was 0B01000100 (1:32) ? WAY too fast  */
    TMR0    = 86;           /* preload ï¿½ also reloaded in ISR        */

    MSCKCON = 0B00000000;
    CMCON0  = 0B00000111;

    T0IE    = 1;
    T0IF    = 0;

    INTEDG  = 1;
    INTE    = 1;
    INTF    = 0;

    PEIE    = 1;
    GIE     = 0;
}
unchar EEPROMread1(unchar EEAddr1)
{
    unchar ReEEPROMread1;
    EEADR1 = EEAddr1;
    RD = 1;
    ReEEPROMread1 = EEDAT;
    return ReEEPROMread1;
}

void EEPROMwrite1(unchar EEAddr1, unchar Data1)
{
    GIE = 0;
    while(GIE);
    EEADR1 = EEAddr1;
    EEDAT  = Data1;
    EEIF   = 0;
    EECON1 = 0x34;
    WR     = 1;
    while(WR);
    GIE = 1;
}

/* -- delays -------------------------------------------------------- */
void delay_us(unsigned char Time)
{
    unsigned char a;
    for(a = 0; a < Time; a++) NOP();
}

void DelayMs(unsigned char Time)
{
    unsigned char a, b;
    for(a = 0; a < Time; a++)
        for(b = 0; b < 5; b++)
            delay_us(197);
}


void Timer0_Init (void)  
{
	OPTION = 0B00000010;

	T0IF = 0;						
}
/* -- Timer1 -------------------------------------------------------- */
void Timer1_Init(void)
{
    T1CON  = 0B00110001;
    TMR1L  = 0;
    TMR1H  = 0;
    TMR1ON = 0;
}

/* -- ISR ----------------------------------------------------------- */
void interrupt ISR(void)
{
  
}