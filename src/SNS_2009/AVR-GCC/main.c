/*
		The^day^of^DooM

	Create Date:	14.03.2009
	Last Update:	11.04.2009

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

#include <avr/io.h>
#include <string.h>
#include <avr/delay.h>
#include <avr/signal.h>
#include <avr/interrupt.h>

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

void setRobko01Position(ROBKO_MOTOR_POSITION arrMotors[], int lenght)
{
	unsigned int i;

	for(i=0; i<lenght; i++) {
		while(arrMotor[arrMotors[i].motor].step);
		arrMotor[arrMotors[i].motor].step = arrMotors[i].step;
		arrMotor[arrMotors[i].motor].direction = arrMotors[i].direction;
	}
}

//#define DEBUG

int main(void)
{
	unsigned char i = 0;
	ROBKO_MOTOR_POSITION arrMotors[] = {
		{ 2, 0, 1000},
		{ 2, 1, 1000},
	};
	
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

	TCNT1 = 0;
	TIMSK = 1<<OCIE1A;
	TCCR1B = 1<<CS11;

#ifdef DEBUG
	arrMotor[0].step = 1111;
	arrMotor[0].direction = 1;
#endif

	sei();
	while(1) {
		/*
		while(arrMotor[2].step);
		arrMotor[2].step = 100;
		arrMotor[2].direction = 0;
		while(arrMotor[2].step);
		arrMotor[2].step = 100;
		arrMotor[2].direction = 1;
		*/

		if(PIND & 1<<PD4) {
			if(!arrMotor[i].step) arrMotor[i].step = 1;
			arrMotor[i].direction = 0;
		}

		if(PIND & 1<<PD5) {
			_delay_ms(255);

			if(++i > sizeof(arrMotor)/sizeof(*arrMotor) - 1) i = 0;

			while(PIND & 1<<PD5);
		}

		if(PIND & 1<<PD6) {
			if(!arrMotor[i].step) arrMotor[i].step = 1;
			arrMotor[i].direction = 1;
		}
	}

	return 0;
}

INTERRUPT(SIG_OUTPUT_COMPARE1A)
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
