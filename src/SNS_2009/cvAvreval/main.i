/*
		The^day^of^DooM

	Create Date:	14.03.2009
	Last Update:	11.04.2009
	Full Channge:	12.04.2009 - For cvAvreval
	Last Update:	18.04.2009

	Project->Configuration option:
		1) Кварц: 7.3728MHz
		3) Ниво на оптимизация: -Os

	PA0/PA3 - motor 0
	PA4/PA7 - motor 1
	PB0/PB3 - motor 2
	PB4/PB7 - motor 3
	PC0/PC3 - motor 4
	PC4/PC7 - motor 5
*/
// CodeVisionAVR C Compiler
// (C) 1998-2000 Pavel Haiduc, HP InfoTech S.R.L.
#pragma used+
void delay_us(unsigned int n);
void delay_ms(unsigned int n);
#pragma used-
// CodeVisionAVR C Compiler
// (C) 1998-2006 Pavel Haiduc, HP InfoTech S.R.L.
// Prototypes for standard I/O functions
// CodeVisionAVR C Compiler
// (C) 1998-2002 Pavel Haiduc, HP InfoTech S.R.L.
// Variable length argument list macros
typedef char *va_list;
#pragma used+
char getchar(void);
void putchar(char c);
void puts(char *str);
void putsf(char flash *str);
char *gets(char *str,unsigned int len);
void printf(char flash *fmtstr,...);
void sprintf(char *str, char flash *fmtstr,...);
void snprintf(char *str, unsigned int size, char flash *fmtstr,...);
void vprintf (char flash * fmtstr, va_list argptr);
void vsprintf (char *str, char flash * fmtstr, va_list argptr);
void vsnprintf (char *str, unsigned int size, char flash * fmtstr, va_list argptr);
signed char scanf(char flash *fmtstr,...);
signed char sscanf(char *str, char flash *fmtstr,...);
                                               #pragma used-
#pragma library stdio.lib
// CodeVisionAVR C Compiler
// (C) 1998-2004 Pavel Haiduc, HP InfoTech S.R.L.
// I/O registers definitions for the ATmega32
#pragma used+
sfrb TWBR=0;
sfrb TWSR=1;
sfrb TWAR=2;
sfrb TWDR=3;
sfrb ADCL=4;
sfrb ADCH=5;
sfrw ADCW=4;      // 16 bit access
sfrb ADCSRA=6;
sfrb ADCSR=6;     // for compatibility with older code
sfrb ADMUX=7;
sfrb ACSR=8;
sfrb UBRRL=9;
sfrb UCSRB=0xa;
sfrb UCSRA=0xb;
sfrb UDR=0xc;
sfrb SPCR=0xd;
sfrb SPSR=0xe;
sfrb SPDR=0xf;
sfrb PIND=0x10;
sfrb DDRD=0x11;
sfrb PORTD=0x12;
sfrb PINC=0x13;
sfrb DDRC=0x14;
sfrb PORTC=0x15;
sfrb PINB=0x16;
sfrb DDRB=0x17;
sfrb PORTB=0x18;
sfrb PINA=0x19;
sfrb DDRA=0x1a;
sfrb PORTA=0x1b;
sfrb EECR=0x1c;
sfrb EEDR=0x1d;
sfrb EEARL=0x1e;
sfrb EEARH=0x1f;
sfrw EEAR=0x1e;   // 16 bit access
sfrb UBRRH=0x20;
sfrb UCSRC=0X20;
sfrb WDTCR=0x21;
sfrb ASSR=0x22;
sfrb OCR2=0x23;
sfrb TCNT2=0x24;
sfrb TCCR2=0x25;
sfrb ICR1L=0x26;
sfrb ICR1H=0x27;
sfrb OCR1BL=0x28;
sfrb OCR1BH=0x29;
sfrw OCR1B=0x28;  // 16 bit access
sfrb OCR1AL=0x2a;
sfrb OCR1AH=0x2b;
sfrw OCR1A=0x2a;  // 16 bit access
sfrb TCNT1L=0x2c;
sfrb TCNT1H=0x2d;
sfrw TCNT1=0x2c;  // 16 bit access
sfrb TCCR1B=0x2e;
sfrb TCCR1A=0x2f;
sfrb SFIOR=0x30;
sfrb OSCCAL=0x31;
sfrb TCNT0=0x32;
sfrb TCCR0=0x33;
sfrb MCUCSR=0x34;
sfrb MCUCR=0x35;
sfrb TWCR=0x36;
sfrb SPMCR=0x37;
sfrb TIFR=0x38;
sfrb TIMSK=0x39;
sfrb GIFR=0x3a;
sfrb GICR=0x3b;
sfrb OCR0=0X3c;
sfrb SPL=0x3d;
sfrb SPH=0x3e;
sfrb SREG=0x3f;
#pragma used-
// Interrupt vectors definitions
// Needed by the power management functions (sleep.h)
#asm
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x80
	.EQU __sm_mask=0x70
	.EQU __sm_powerdown=0x20
	.EQU __sm_powersave=0x30
	.EQU __sm_standby=0x60
	.EQU __sm_ext_standby=0x70
	.EQU __sm_adc_noise_red=0x10
	.SET power_ctrl_reg=mcucr
	#endif
#endasm
// CodeVisionAVR C Compiler
// (C) 1998-2005 Pavel Haiduc, HP InfoTech S.R.L.
// Prototypes for string functions
#pragma used+
char *strcat(char *str1,char *str2);
char *strcatf(char *str1,char flash *str2);
char *strchr(char *str,char c);
signed char strcmp(char *str1,char *str2);
signed char strcmpf(char *str1,char flash *str2);
char *strcpy(char *dest,char *src);
char *strcpyf(char *dest,char flash *src);
unsigned char strcspn(char *str,char *set);
unsigned char strcspnf(char *str,char flash *set);
unsigned int strlenf(char flash *str);
char *strncat(char *str1,char *str2,unsigned char n);
char *strncatf(char *str1,char flash *str2,unsigned char n);
signed char strncmp(char *str1,char *str2,unsigned char n);
signed char strncmpf(char *str1,char flash *str2,unsigned char n);
char *strncpy(char *dest,char *src,unsigned char n);
char *strncpyf(char *dest,char flash *src,unsigned char n);
char *strpbrk(char *str,char *set);
char *strpbrkf(char *str,char flash *set);
signed char strpos(char *str,char c);
char *strrchr(char *str,char c);
char *strrpbrk(char *str,char *set);
char *strrpbrkf(char *str,char flash *set);
signed char strrpos(char *str,char c);
char *strstr(char *str1,char *str2);
char *strstrf(char *str1,char flash *str2);
unsigned char strspn(char *str,char *set);
unsigned char strspnf(char *str,char flash *set);
char *strtok(char *str1,char flash *str2);
 unsigned int strlen(char *str);
void *memccpy(void *dest,void *src,char c,unsigned n);
void *memchr(void *buf,unsigned char c,unsigned n);
signed char memcmp(void *buf1,void *buf2,unsigned n);
signed char memcmpf(void *buf1,void flash *buf2,unsigned n);
void *memcpy(void *dest,void *src,unsigned n);
void *memcpyf(void *dest,void flash *src,unsigned n);
void *memmove(void *dest,void *src,unsigned n);
void *memset(void *buf,unsigned char c,unsigned n);
#pragma used-
#pragma library string.lib
/*
CodeVisionAVR C Compiler
Prototypes for mathematical functions

Portions (C) 1998-2001 Pavel Haiduc, HP InfoTech S.R.L.
Portions (C) 2000-2001 Yuri G. Salov
*/
#pragma used+
unsigned char cabs(signed char x);
unsigned int abs(int x);
unsigned long labs(long x);
float fabs(float x);
signed char cmax(signed char a,signed char b);
int max(int a,int b);
long lmax(long a,long b);
float fmax(float a,float b);
signed char cmin(signed char a,signed char b);
int min(int a,int b);
long lmin(long a,long b);
float fmin(float a,float b);
signed char csign(signed char x);
signed char sign(int x);
signed char lsign(long x);
signed char fsign(float x);
unsigned char isqrt(unsigned int x);
unsigned int lsqrt(unsigned long x);
float sqrt(float x);
float floor(float x);
float ceil(float x);
float fmod(float x,float y);
float modf(float x,float *ipart);
float ldexp(float x,int expon);
float frexp(float x,int *expon);
float exp(float x);
float log(float x);
float log10(float x);
float pow(float x,float y);
float sin(float x);
float cos(float x);
float tan(float x);
float sinh(float x);
float cosh(float x);
float tanh(float x);
float asin(float x);
float acos(float x);
float atan(float x);
float atan2(float y,float x);
#pragma used-
#pragma library math.lib
/*
        The^day^of^DooM

    Create date:	27.10.2007
    Last Update:	01.11.2007
*/
void videoSendData(unsigned char data)
void videoSendData(unsigned char data)
{
	while(PIND.6);
	while(!(UCSRA & (1<<5)));
	UDR = data;
}
//at x,y with color 1=white 0=black 2=invert
void videoPlotPoint(unsigned char x, unsigned char y, unsigned char c)
{
	videoSendData(1);
	videoSendData(x);
	videoSendData(y);
	videoSendData(c);
}
/*
void videoPutCharXY(unsigned char x, unsigned char y, unsigned char c)
{
	videoSendData(3);
	videoSendData(x);
	videoSendData(y);
	videoSendData(c);
}
*/
void videoPutStrXY(unsigned char x, unsigned char y, unsigned char *str)
{
	unsigned char i = 0;
	videoSendData(2);
	videoSendData(x);
	videoSendData(y);
	do {
		videoSendData(str[i]);
	} while(str[i++]);
}
void videoInit(void)
{
	// USART initialization
	// Communication Parameters: 8 Data, 1 Stop, No Parity
	// USART Receiver: Off
	// USART Transmitter: On
	// USART Mode: Asynchronous
	// USART Baud Rate: 76.8k
	UCSRA=0x00;
	UCSRB=0x08;
	UCSRC=0x86;
	UBRRH=0x00;
	UBRRL=0x05;
}
 
//****************************************************************************
// Note: we used PS/2 mouse interaction code originally developed 
// for a Minesweeper Program by Chee Ming, Chaw and Elaine Siu for 476 in 2003 
// 
// They developed the following functions in this source file:
// 	mouse_send
// 	mouse_read
// 	poll_mouse
// 	reset_mouse
// The remaining functions we developed. 
//****************************************************************************
                                     /*
        Create date:    01.11.2007
        Last Update:    17.04.2009
*/
                                                                          #asm    
        .equ PORTD = 0x12
        .equ DDRD  = 0x11       

        sbi DDRD, 3
        cbi PORTD, 3
        .ENDM
        
        cbi DDRD, 3
        sbi PORTD, 3
        .ENDM
        
        sbi DDRD, 2
        cbi PORTD, 2
        .ENDM
        
        cbi DDRD, 2
        sbi PORTD, 2
        .ENDM
#endasm
          //**********************************************************     
// S} a byte of data to mouse. It consist of 1 start bit, 8 data bits, 1 parity bit, 1 stop bit. 
// After the mouse recieved the data correctly, the mouse will s} back an ACK bit.
// "data" is the data to be sent. parity bit is odd parity of the 8 bits data.   
// Data is read by mouse at rising edge of CLK.
void mouse_send(char data)
{ 
  unsigned char Bit,parity,i;
  #asm("RELDATA");        
  #asm("RELCLK");
  delay_us(300);
  #asm("CLKLOW");               //Pull CLK Low
  delay_us(300);                // need to wait for at least 100us
  #asm("DATALOW");              // Pull DATA Low when holding CLK Low
  delay_us(100);
  #asm("RELCLK");                //Release CLK and continue to hold Data Low. This is also the Start bit.
  delay_us(40);
  while(PIND.2  !=0) { }        // wait for CLK to go low 
  i=0; Bit=0b00000001;                                                     
  parity=1;                     // odd parity = 1 ^ bit0 ^ bit1 ^ ... ^ bit7
       while(i<=8)
    {
              if(i==8)                  // If i=8 s} parity bit otherwise, s} the next bit of data.
      { if(parity)              // DATA_OUT=(i==8? parity : (data & Bit) );
          { #asm("RELDATA"); }          
        else 
          { #asm("DATALOW"); }
       }
      else
      { if(data & Bit)
          { #asm("RELDATA"); }
        else
          { #asm("DATALOW"); }
      }
      while(PIND.2  !=1            ) {}                    // wait for CLK to go high. mouse read data at rising edge.
      while(PIND.2  !=0)  {}                    // wait for CLK to go low. 
      parity=parity ^ PORTD.3 ;                 // update parity bit.
      Bit=Bit<<1;                               // Prepare mask
      i++;
    }
  #asm("RELDATA");                              // Release Data line. This is the stop bit. 
  delay_us(50);                                 // give time for transient to settle.
  while(PIND.3  !=0) {}                        // wait for Data line to go low               
  while(PIND.2  !=0)  {}                        // wait for CLK to go low. Start of ACK from mouse.
  while(PIND.3  ==0 || PIND.2  ==0) {}         // } of ACK from mouse.               
  #asm("CLKLOW");                               // Inhibit the mouse from transmitting data.
}                                                           
//**********************************************************
// This function read a byte of data from the mouse and return the byte
unsigned char mouse_read(void)
{
  unsigned char i,t[10];  
  #asm("RELCLK");         //release the CLK 
  #asm("RELDATA");        //release DATA line.
  delay_us(200);          // wait for at least 100us
  while(PIND.2  ==1            )  {} // wait for falling edge. we read data at falling edge of CLK
  i=0;  
  delay_us(5);
    while(PIND.2  ==0)   {} // wait for CLK to go high.
  while(i<=8)
    {
      while(PIND.2  ==1            ) {} // wait for falling edge of CLK   
        t[i]=PIND.3  ;        // store each bit
        i++;       
      while(PIND.2  ==0) {}
    }   
  while(PIND.2  ==0) {}  
  #asm("CLKLOW");          //pull CLK low to prevent mouse from s}ing data until being asked
  return 128*t[7]+64*t[6]+32*t[5]+16*t[4]+8*t[3]+4*t[2]+2*t[1]+t[0]; //combine the collected bits into a byte
  }
//*******************************************                                
// This routine poll the mouse's status and movement data.
 /*void poll_mouse(void)                                                       
{  
    mouse_send(0xeb);                     //Request data from mouse
    mouse_read();                         //Read the acknowledge byte
    mouse_status=mouse_read();            //Read mouse status
    mouse_x=mouse_read();                 //Read mouse horizontal movement
    mouse_y=mouse_read();                 //Read mouse vertical movement
}
                                                                                */
//**********************************************
// This function is called to reset the mouse and put it into remote mode.
// In remote mode, mouse only s} data upon request from the microcontroller.
// This function is called when manual reset of the mouse is required. This is for
// overcoming interference problem from some unknown EM field.
void reset_mouse(void)
{
  mouse_send(0xff);     // reset mouse
  mouse_read();         // read mouse acknowledge (0xFA)
  mouse_read();         // read some blank bytes.  
  mouse_read();     
  mouse_send(0xf0);     // Set mouse to remote mode.
  mouse_read();         // read mouse acknowledge (0xFA)           
  delay_ms(100);
} 
char mouse(char *x, char *y)
{
    int cursor_x, cursor_y;
    char mouse_status, mouse_y, mouse_x;
         cursor_x = *x;
    cursor_y = *y;
        mouse_send(0xeb);                     //Request data from mouse
    mouse_read();                         //Read the acknowledge byte
    mouse_status=mouse_read();            //Read mouse status
    mouse_x=mouse_read();                 //Read mouse horizontal movement
    mouse_y=mouse_read();                 //Read mouse vertical movement
        if(mouse_x) //calculate horizontal position according to its sign and value
        if(mouse_status & 0b00010000) cursor_x -= 256-mouse_x;
        else cursor_x += mouse_x;
        if(mouse_y)
        if(mouse_status & 0b00100000) cursor_y += 256-mouse_y;
        else cursor_y -= mouse_y;
            if(cursor_x > 127) cursor_x = 127;
    else if(cursor_x < 1) cursor_x = 0;
    if(cursor_y > 99) cursor_y = 99;
    else if(cursor_y < 1) cursor_y = 0;
                 *x = cursor_x;
    *y = cursor_y;
        if(mouse_status & 0b00000001) return 1;
    return 0;
}  
void cursorDraw(char cursor_x, char cursor_y)
{
    static char xOld=0, yOld=0;
    if(xOld != cursor_x || yOld != cursor_y) videoPlotPoint(xOld, yOld, 0);
    videoPlotPoint(cursor_x, cursor_y, 1);
        xOld = cursor_x;
    yOld = cursor_y;
}
void mouseInit(void)
{  
  //init ports                                                               
  DDRD &= 0b11110011;    // PD3 DATA IN, PD2 CLOCK IN
     DDRD.3  = 1; 
  PORTD.3 = 1;
  delay_ms(500); 
  DDRD &= 0b11110011;
      DDRD.2  = 1; 
  PORTD.2 = 1;
  delay_ms(500); 
  DDRD &= 0b11110011;
    #asm("RELCLK");
  #asm("RELDATA");
    reset_mouse();
}
   /* PORTD */
typedef struct {
	unsigned int step;
	unsigned char direction, state;
} BI_STEP_MOTOR, *BI_STEP_MOPTOR;
volatile char arrStep[] = {
	0b00001000,
	0b00001010,
	0b00000010,
	0b00000110,
	0b00000100,
	0b00000101,
	0b00000001,
	0b00001001
};
volatile BI_STEP_MOTOR arrMotor[6];
typedef struct {
        unsigned char motor, direction;
	unsigned int step;
} ROBKO_MOTOR_POSITION;
//#define DEBUG
//interrupt [TIM1_COMPA]
void timer1_compa_isr(void)
{
	unsigned char i;
	unsigned char newPorta = 0, newPortb =0, newPortc = 0;
        	TCNT1 = 0;
	for(i=0; i<sizeof(arrMotor)/sizeof(*arrMotor); i++) {
		if(arrMotor[i].step) {
			switch(i) {
			case 0: newPorta |= arrStep[arrMotor[i].state]; break;
			case 1: newPorta |= arrStep[arrMotor[i].state]<<4; break;
			case 2: newPortb |= arrStep[arrMotor[i].state]; break;
			case 3: newPortb |= arrStep[arrMotor[i].state]<<4; break;
			case 4: newPortc |= arrStep[arrMotor[i].state]; break;
			case 5: newPortc |= arrStep[arrMotor[i].state]<<4;
			}			
						if(arrMotor[i].direction) {
				if(++arrMotor[i].state > sizeof(arrStep)-1) {
					arrMotor[i].state = 0;
					--arrMotor[i].step;
				}
			} else {
				if((char)arrMotor[i].state-- < 1) {
					arrMotor[i].state = sizeof(arrStep)-1;
					--arrMotor[i].step;
				}
			}
		}
	}
		PORTA = newPorta;
	PORTB = newPortb;
	PORTC = newPortc;   
}
    int main(void)
{                           
    int n = 0, m = 0, dx = 0, dy = 0;
	unsigned char x, y, mouse_status, motor, direction, step;
		char buffer[22];
	DDRA = DDRB = DDRC = 0xff;
	memset(&arrMotor, 0, sizeof(arrMotor));
 	// 1us = 1.085
	OCR1AH = 3333>>8;
	OCR1AL = (unsigned char)3333; 
			//TCNT1 = 0;
	//TIMSK = 1<<OCIE1A;
	//TCCR1B = 1<<CS11;
		                   mouseInit();
    videoInit();
        	#asm sei;
	#endasm
    while(1) {
        if(++n>1000) {
            n = 0;
            timer1_compa_isr();
                        if(++m>10) {
                m = 0;
	                                        mouse_send(0xeb);                  //Request data from mouse
                mouse_read();                      //Read the acknowledge byte
                mouse_status = mouse_read();       //Read mouse status
                dx = mouse_read();                 //Read mouse horizontal movement
                dy = mouse_read();                 //Read mouse vertical movement
                                if(dx && (mouse_status & 0b00010000)) dx = -(256-dx);
                if(dy && (mouse_status & 0b00100000)) dy = -(256-dy);
                sprintf(buffer, "Mouse dx = %d    ", dx); videoPutStrXY(3, 0, buffer);
                sprintf(buffer, "Mouse dy = %d    ", dy); videoPutStrXY(3, 11, buffer);
                if(dx>=0) x = dx;
                else x = -dx;
                if(dy>=0) y = dy;
                else y = -dy;
                                motor = direction = step = 0;
                if(x > 77 && y > 77) {
                    if(dx>0 && dy<0) {
                        step = 8;
                        motor = 2;
                        direction = 1;
                    } else if(dx<0 && dy>0) {
                        motor = 2;
                        direction = 0;
                    }
                                                       if(dx<0 && dy<0) {
                        step = 8;
                        motor = 3;
                        direction = 1;
                    } else if(dx>0 && dy>0) {
                        step = 8;
                        motor = 3;
                        direction = 0;
                    }
                } else {
                    if(x > 88) { 
                        step = 8;
                        motor = 0;
                        if(dx<0) direction = 1;
                        else direction = 0;
                    } else if(y > 88) {
                        step = 8;
                        motor = 1;
                        if(dy < 0) direction = 1;
                        else direction = 0;
                    }
                }
                                sprintf(buffer, "Motor %d ", motor); videoPutStrXY(3, 22, buffer);  
                sprintf(buffer, "Step %d ", step); videoPutStrXY(3, 33, buffer);
                sprintf(buffer, "Direction %d ", direction); videoPutStrXY(3, 44, buffer);
                                arrMotor[motor].step = step;
                arrMotor[motor].direction = direction; 
                                //arrMotor[motor].step += (direction == arrMotor[motor].direction) ? step : -step;
                //if(!arrMotor[motor].step) arrMotor[motor].direction = direction;    
            }
        }
                /*
        if(PIND & 1<<PD4) {
            if(!arrMotor[i].step) arrMotor[i].step = 1;
            arrMotor[i].direction = 0;
        }  
		
        if(PIND & 1<<PD5) {
            delay_ms(255);

            if(++i > sizeof(arrMotor)/sizeof(*arrMotor) - 1) i = 0;

            while(PIND & 1<<PD5);
        }

        if(PIND & 1<<PD6) {
            if(!arrMotor[i].step) arrMotor[i].step = 1;
            arrMotor[i].direction = 1;
        }*/
    }
    return 0;
}
