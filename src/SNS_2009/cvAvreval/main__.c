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

#include <delay.h>
#include <stdio.h> // sprintf
#include <mega32.h>
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
#include <string.h>
#include <math.h>
#include "video.h"
#include "mouse.h"
        .equ PORTD = 0x12
        .equ DDRD  = 0x11
        .MACRO DATALOW
        sbi DDRD, 3
        cbi PORTD, 3
        .ENDM
        .MACRO RELDATA
        cbi DDRD, 3
        sbi PORTD, 3
        .ENDM
        .MACRO CLKLOW
        sbi DDRD, 2
        cbi PORTD, 2
        .ENDM
        .MACRO RELCLK
        cbi DDRD, 2
        sbi PORTD, 2
        .ENDM

#define CS11    1
#define OCIE1A  4
/* PORTD */
#define PD7     7
#define PD6     6
#define PD5     5
#define PD4     4
#define PD3     3
#define PD2     2
#define PD1     1
#define PD0     0

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

#ifndef DEBUG
	// 1us = 1.085
	OCR1AH = 3333>>8;
	OCR1AL = (unsigned char)3333;
#else
	OCR1AH = 33>>8;
	OCR1AL = (unsigned char)33;
#endif

	//TCNT1 = 0;
	//TIMSK = 1<<OCIE1A;
	//TCCR1B = 1<<CS11;

#ifdef DEBUG
	arrMotor[0].step = 1111;
	arrMotor[0].direction = 1;
#endif

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
