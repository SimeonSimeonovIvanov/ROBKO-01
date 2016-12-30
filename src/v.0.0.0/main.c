/*
		The^day^of^DooM

	Create Date:	14.03.2009
	Last Update:	11.04.2009
	Full Change:	18.05.2013 - For Board "v.0.0.1"
	Last Update:	18.05.2013
	Last Update:	19.05.2013 - Add ModBUS Suport
	Last Update:	21.05.2013
	Last Update:	30.05.2013
	Last Update:	01.06.2013
	Last Update:	11.06.2013 - Add POWER INT3 (LM311)
	Full Channge:	05.12.2013 - Bug Fix in CNC function G01
	Last Update:	06.12.2013
	Last Update:	07.12.2013 - Add basic RTC suport

	Project->Configuration option:
		1) Кварц: 16.000 MHz
		3) Ниво на оптимизация: -Os

		Motors map:

	PORTA[0..3] - MCU_M1n	( MOTOR[0] )
	PORTA[4..7] - MCU_M2n	( MOTOR[1] )

	PORTB[4..7] - MCU_M6n	( MOTOR[5] )

	PORTC[0..3] - MCU_M4n	( MOTOR[3] )
	PORTC[4..7] - MCU_M3n	( MOTOR[2] )

	PORTD[4..7] - MCU_M5n	( MOTOR[4] )
*/

#include <util/delay.h>

#include "app/clock_sync.h"
#include "app/dhcp_client.h"
#include "app/httpd.h"
#include "arch/spi.h"
#include "arch/uart.h"
#include "net/arp.h"
#include "net/ethernet.h"
#include "net/hal.h"
#include "net/icmp.h"
#include "net/ip.h"
#include "net/tcp.h"
#include "net/udp.h"
#include "sys/clock.h"
#include "sys/timer.h"

#include "i2c.h"
#include "ds1307.h"

#include "ROBKO_01.h"

//#define __DEBUG__
#define __USE_HALF_STEP__
#define SIZE_OF_ARRAY( arr )		( sizeof(arr) / sizeof(*arr) )

#include <string.h>
#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>

typedef struct {
	int16_t pos;
	uint16_t step;
	uint8_t state;
	int8_t direction;
} BI_STEP_MOTOR, LP_BI_STEP_MOTOR;

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "mbutils.h"

// ----------------------------------- Defines --------------------------------------------

// MB_FUNC_READ_DISCRETE_INPUTS					( 2 )
#define REG_DISC_START							1
#define REG_DISC_SIZE							32

volatile unsigned char ucRegDiscBuf[ REG_DISC_SIZE / 8 ] = { 0, 0 };
uint8_t inPort[ REG_DISC_SIZE ];
// -----------------------------------------------------------------
// MB_FUNC_READ_COILS							( 1 )
// MB_FUNC_WRITE_SINGLE_COIL					( 5 )
// MB_FUNC_WRITE_MULTIPLE_COILS					( 15 )
#define REG_COILS_START							1
#define REG_COILS_SIZE							24

volatile unsigned char ucRegCoilsBuf[ REG_COILS_SIZE / 8 ];
volatile uint8_t outPort[ REG_COILS_SIZE ];
// -----------------------------------------------------------------
// MB_FUNC_READ_INPUT_REGISTER					(  4 )
#define REG_INPUT_START							1
#define REG_INPUT_NREGS							15

volatile uint16_t uiRegInputBuf[ REG_INPUT_NREGS ];
// -----------------------------------------------------------------
// MB_FUNC_READ_HOLDING_REGISTER				( 3 )
// MB_FUNC_WRITE_REGISTER						( 6 )
// MB_FUNC_READWRITE_MULTIPLE_REGISTERS			( 23 )
// MB_FUNC_WRITE_MULTIPLE_REGISTERS				( 16 )
#define REG_HOLDING_START						1
#define REG_HOLDING_NREGS						15

volatile uint16_t uiRegHolding[REG_HOLDING_NREGS];

/* --------------------------------- Static variables ---------------------------------- */

volatile UCHAR ucSlaveID = 10;

//uint8_t isRun = 0;
volatile uint8_t uiModbusTimeOutCounter = 0;

volatile char arrStep[] = {
  #ifndef __USE_HALF_STEP__
	0b10101010,
	0b01100110,
	0b01010101,
	0b10011001
  #else
	0b10001000,
	0b10101010,
	0b00100010,
	0b01100110,
	0b01000100,
	0b01010101,
	0b00010001,
	0b10011001
  #endif
};

// Don't channge!!!
EEMEM char dontUsesAddressZeroOfEEPROM;
EEMEM BI_STEP_MOTOR EEPROM_arrMotor[6];

volatile BI_STEP_MOTOR arrMotor[6];

volatile char flagAllMotorNotMove;

float arrPos[6]; // [ X mm. : Y mm. : Z mm. : P deg. : R deg. : G deg. ]

float X1, Y1, Z1, P1, R1, G1;
int X2, Y2, Z2, P2, R2, G2;
float a, delta, dx, dy, dz, dP, dR, dG;

volatile uint16_t timer_events = 0;
volatile bool net_link_up = 0, flagDhcp = 0;
volatile uint8_t mac_addr[6] = { 'F', 'O', 'O', 'B', 'A', 'R' };

/* ------------------------------- Start implementation -------------------------------- */
void robko01_SavePos(void);
void robko01_LoadPos(void);
static void dhcp_client_event_callback(enum dhcp_client_event event);

int main(void)
{
	int i;
	eMBErrorCode eStatus;
	uint8_t flagDrawLine = 0;
	uint8_t mbFlagInitPort, mbFlagInitPortOld = 2;

	/////////////////////////////////////////////////////////////////////////////
	DDRA = 0xFF;
	DDRB = 0xF7;
	DDRC = 0xFF;
	DDRD = 0xF0;
	DDRE = 0x06;
	DDRF = 0x0F;
	/////////////////////////////////////////////////////////////////////////////
	TWBR = 72;
	TWSR = 0x00;
	/////////////////////////////////////////////////////////////////////////////
	memset( &inPort, 0, sizeof(outPort) );
	memset( &outPort, 0, sizeof(outPort) );
	memset( &ucRegCoilsBuf, 0, sizeof(ucRegCoilsBuf) );
	/////////////////////////////////////////////////////////////////////////////
	robko01_LoadPos();
	while( 0 ) {
		asm("nop\n");
		robko01_SavePos();
		asm("nop\n");
		robko01_LoadPos();
		asm("nop\n");
	}
	/////////////////////////////////////////////////////////////////////////////
	spi_init();
	/////////////////////////////////////////////////////////////////////////////
	hal_init((unsigned char*)mac_addr);
	// initialize ethernet protocol stack
	ethernet_init((unsigned char*)mac_addr);
	arp_init();
	ip_init(0, 0, 0);
	icmp_init();
	tcp_init();
	udp_init();
	/////////////////////////////////////////////////////////////////////////////
	OCR3A = 15000;
#ifndef __USE_HALF_STEP__
	OCR3A *= 2;
#endif
#ifdef __DEBUG__
	OCR3A /= 10;
#endif
	TCNT3 = 0;
	ETIMSK = 1<<OCIE3A;
	TCCR3B = 1<<CS31;
	///////////////////////////////////////////////////////////////////////////////
	// setup interval timer
	TCCR2 |= (1<<WGM21) | (0<<WGM20) | (1<<CS22) | (0<<CS21) | (1<<CS20);
	TIMSK |= (1<<OCIE2);
	OCR2 = F_CPU / 1024 / 1000;
	///////////////////////////////////////////////////////////////////////////////
	// start clock
	//clock_init();
	// start http server
	//httpd_init(88);
	///////////////////////////////////////////////////////////////////////////////
	// Rising Edge of INT3
	EICRA	= 1<<ISC31 | 1<<ISC30;
	EIMSK	= 1<<INT3;
	EIFR	= 1<<INTF3;
	///////////////////////////////////////////////////////////////////////////////

	sei();
	while(1) {
		///////////////////////////////////////////////////////////////////////////
		if( PINE & (1<<PE6) ) {
			mbFlagInitPort = 1;
		} else {
			mbFlagInitPort = 0;
		}

		if( mbFlagInitPort != mbFlagInitPortOld ) {
			eMBDisable();

			if( mbFlagInitPort ) {
				eStatus = eMBTCPInit(502);
			} else {
				eStatus = eMBInit( MB_RTU, ucSlaveID, 0, 115200, MB_PAR_EVEN );
			}

			if( MB_ENOERR == eStatus ) {
				eMBEnable();
			}
			
			mbFlagInitPortOld = mbFlagInitPort;
		}
		///////////////////////////////////////////////////////////////////////////
		for(i = 0; i < 5; i++) {
			if( PINE & 1<<( 3 + i ) ) {
				ucRegDiscBuf[ i / 8] |= ( 1<<i );
				inPort[ i ] = 1;
			} else {
				ucRegDiscBuf[ i / 8] &= ~( 1<<i );
				inPort[ i ] = 0;
			}
		}

		flagAllMotorNotMove = 0;
		cli();
		for( i = 0 ; i < 6 ; i++ ) {
			uiRegInputBuf[i] = arrMotor[i].step;
			uiRegInputBuf[6 + i] = (unsigned int)arrMotor[i].pos;

			flagAllMotorNotMove |= arrMotor[i].step;
		}
		sei();
		flagAllMotorNotMove = !flagAllMotorNotMove;
		uiRegInputBuf[12] = uiRegHolding[6];

		/* handle network link changes */
		if( !net_link_up && hal_link_up() ) {
			dhcp_client_start(dhcp_client_event_callback);
			net_link_up = 1;
		} else {
			if( net_link_up && !hal_link_up() ) {
				dhcp_client_abort();
				ip_init( 0, 0, 0 );
				net_link_up = 0;
			}
		}

		if( 1 == flagDhcp ) {
			uint8_t ip_addr[4] = { 192, 168, 0, 11 };
			uint8_t netmask[4] = { 255, 255, 255, 0 };
			uint8_t gateway[4] = { 192, 168, 0, 1 };
			ip_init(
				(unsigned char*)ip_addr,
				(unsigned char*)netmask,
				(unsigned char*)gateway
			);

			net_link_up = 1;
			flagDhcp = 0;
		}

		/* receive packets */
		while(ethernet_handle_packet());

		cli();
		volatile uint8_t timer_ticks = 2;timer_events;
		timer_events = 0;
		sei();

		/* handle timeouts */
		while( timer_ticks-- ) {
			timer_interval();
		}

		eMBPoll();

		for( i = 0; i < 15; i++ ) {
			if(i < 8) {
				if( ( 1<<i ) & ucRegCoilsBuf[0] ) {
					outPort[i] = 1;
				} else {
					outPort[i] = 0;
				}
			} else {
				if( ( 1<<( i - 8 ) ) & ucRegCoilsBuf[1] ) {
					outPort[i] = 1;
				} else {
					outPort[i] = 0;
				}
			}
		}

		cli();
		for( i = 0; i < 6; i++ ) {
		
			if( outPort[6 + i] ) {
				arrMotor[i].step = 1;
				arrMotor[i].direction = outPort[i];
			}
		}
		if( outPort[9] ) {
			arrMotor[3].direction = !arrMotor[3].direction;
		}	
		sei();

		switch( 0x00ff & uiRegHolding[6] ) {
		
		// BEGIN: EMS
		case 3: {
			cli();
			for( i = 0; i < 6 ; i++ ) {
				arrMotor[i].step = 0;
			}
			flagDrawLine = 0;
			uiRegHolding[6] = 0;
			sei();
		} // END: EMS
		 break;

		// BEGIN: Set HOME
		case 33: {
			cli();
			for( i = 0; i < 6 ; i++ ) {
				arrMotor[i].step = arrMotor[i].pos = 0;
			}
			flagDrawLine = 0;
			uiRegHolding[6] = 0;
			sei();
		} // END: Set HOME
		 break;

		// BEGIN: Move By Step
		case 1: {
			cli();

			for( i = 0; i < 6 ; i++ ) {
				arrMotor[i].step = uiRegHolding[i];
				arrMotor[i].direction = outPort[i];
			}
			arrMotor[3].direction = !arrMotor[ 3 ].direction;

			uiRegHolding[6] = 0;
			sei();
		} // END: Move By Step
		 break;

		// BEGIN: Move By Pos
		case 2: {
			cli();
			for( i = 0; i < 6 ; i++ ) {
				int dPos = arrMotor[i].pos + (int)uiRegHolding[7 + i];

				if( dPos < 0 ) {
					dPos = -dPos;
					arrMotor[i].direction = 0;
				} else {
					arrMotor[i].direction = 1;
				}

				arrMotor[i].step = dPos;
			}

			arrMotor[3].direction = !arrMotor[3].direction;

			uiRegHolding[6] = 0;
			sei();
		} // END: Move By Pos
		 break;

		// Save current motors value
		case 5:
			robko01_SavePos();
			uiRegHolding[6]	= 0;
		 break;

		// BEGIN: Line
		case 4: {
			if( flagAllMotorNotMove ) {
				if( !flagDrawLine ) {
					Robko01_Forward_Kinematics_From_Motor_Pos(
						(int16_t*)&uiRegInputBuf[6], arrPos, 0.0f
					);

					X1 = arrPos[0];
					Y1 = arrPos[1];
					Z1 = arrPos[2];
					P1 = arrPos[3];
					R1 = arrPos[4];
					G1 = arrPos[5];

					if( 0x0100 & uiRegHolding[6] ) {
						X2 = (int)uiRegHolding[7];
						dx = X2 - X1;
					} else {
						X2 = X1;
						dx = 0.0f;
					}

					if( 0x0200 & uiRegHolding[6] ) {
						Y2 = (int)uiRegHolding[8];
						dy = Y2 - Y1;
					} else {
						Y2 = Y1;
						dy = 0.0f;
					}

					if( 0x0400 & uiRegHolding[6] ) {
						Z2 = (int)uiRegHolding[9];
						dz = Z2 - Z1;
					} else {
						Z2 = Z1;
						dz = 0.0f;
					}

					if( 0x0800 & uiRegHolding[6] ) {
						P2 = (int)uiRegHolding[10];
						dP = P2 - P1;
					} else {
						P2 = P1;
						dP = 0.0f;
					}

					if( 0x1000 & uiRegHolding[6] ) {
						R2 = (int)uiRegHolding[11];
						dR = R2 - R1;
					} else {
						R2 = R1;
						dR = 0.0f;
					}

					if( 0x2000 & uiRegHolding[6] ) {
						G2 = (int)uiRegHolding[12];
						dG = G2 - G1;
					} else {
						G2 = G1;
						dG = 0.0f;
					}

					float c = sqrt( pow( dx, 2 ) + pow( dy, 2 ) + pow( dz, 2 ) );

					if( !c ) {
						delta = 0.001;
					} else {
						delta = 1 / c;
					}

					if( delta > 0.001 ) {
						delta = 0.001;
					}

					flagDrawLine = 1;
				} else {
					if( a <= ( 1.0f + delta ) ) {
						float Xn = X1 + a * dx;
						float Yn = Y1 + a * dy;
						float Zn = Z1 + a * dz;
						float Pn = P1 + a * dP;
						float Rn = R1 + a * dR;
						float Gn = G1 + a * dG;
						int temp[6];

						Robko01_Inverse_Kinematics//_fig4_4
						(
							Xn, Yn, Zn,
							(float)Pn,
							(float)Rn,
							(float)Gn,
							0.0f,
							temp
						);

						cli();
						OCR3A = 15000;
#ifndef __USE_HALF_STEP__
						OCR3A *= 2;
#endif
						for( i = 0; i < 6 ; i++ ) {
							int dPos = arrMotor[i].pos + (int)temp[i];

							if( dPos < 0 ) {
								dPos = -dPos;
								arrMotor[i].direction = 0;
							} else {
								arrMotor[i].direction = 1;
							}

							arrMotor[i].step = dPos;
						}

						arrMotor[3].direction = !arrMotor[3].direction;

						a += delta;

						sei();
					} else {
						uiRegHolding[6]	= 0;
						flagDrawLine = 0;
						a = 0.0f;
					}
				}
			}
		} // END: Line
		 break;

		// BEGIN: Line
		case 44: {
			int arrMotorPos2[6];
			static int arrMotorPos1[6];
			static int arrMotorPosDelta[6];

			if( flagAllMotorNotMove ) {
				if( !flagDrawLine ) {

					Robko01_Inverse_Kinematics//_fig4_4
					(
						(int)uiRegHolding[7], (int)uiRegHolding[8], (int)uiRegHolding[9],
						(int)uiRegHolding[10], (int)uiRegHolding[11], (int)uiRegHolding[12],
						0.0f,
						arrMotorPos2
					);

					//arrMotorPos2[1] = -arrMotorPos2[1];
					//arrMotorPos2[2] = -arrMotorPos2[2];

					for( i = 0; i < 6; i++ ) {
						arrMotorPos1[i] = (int)uiRegInputBuf[6 + i];
						arrMotorPosDelta[i] = arrMotorPos2[i] - arrMotorPos1[i];
					}

					a = 0.0f;
					delta = 0.001f;
					flagDrawLine = 1;
				} else {
					if( a <= ( 1.0f + delta ) ) {
						int temp[6];

						for( i = 0; i < 6; i++ ) {
							temp[i] = (int)((float)arrMotorPos1[i] + a * (float)arrMotorPosDelta[i]);
						}

						cli();
						OCR3A = 15000;
#ifndef __USE_HALF_STEP__
						OCR3A *= 2;
#endif
						for( i = 0; i < 6 ; i++ ) {
							int dPos = arrMotor[i].pos + temp[i];

							if( dPos < 0 ) {
								dPos = -dPos;
								arrMotor[i].direction = 0;
							} else {
								arrMotor[i].direction = 1;
							}

							arrMotor[i].step = dPos;
						}
						arrMotor[3].direction = !arrMotor[3].direction;
						sei();

						a += delta;
					} else {
						uiRegHolding[6]	= 0;
						flagDrawLine = 0;
						a = 0.0f;
					}
				}
			}
		} // END: Line
		 break;

		}

		/////////////////////////////////////////////////////////////
		if( arrMotor[0].pos > T1Max ) {
			arrMotor[0].pos = T1Max;
		}
		
		if( arrMotor[0].pos < T1Min ) {
			arrMotor[0].pos = T1Min;
		}
		/////////////////////////////////////////////////////////////
		if( arrMotor[1].pos > T2Max ) {
			arrMotor[1].pos = T2Max;
		}
		
		if( arrMotor[1].pos < T2Min ) {
			arrMotor[1].pos = T2Min;
		}
		/////////////////////////////////////////////////////////////
	}

	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////
eMBErrorCode eMBRegDiscreteCB
(
	UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete
)
{
	volatile eMBErrorCode eStatus = MB_ENOERR;
	volatile short iNDiscrete = ( short )usNDiscrete;
	volatile unsigned short usBitOffset;

	// MB_FUNC_READ_DISCRETE_INPUTS          ( 2 )
	/* Check if we have registers mapped at this block. */
	if( (usAddress >= REG_DISC_START) &&
		(usAddress + usNDiscrete <= REG_DISC_START + REG_DISC_SIZE)
	) {
		usBitOffset = ( unsigned short )( usAddress - REG_DISC_START );
		while(iNDiscrete > 0) {
			*pucRegBuffer++ =
			xMBUtilGetBits( (unsigned char*)ucRegDiscBuf, usBitOffset,
                            (unsigned char)(iNDiscrete>8? 8:iNDiscrete)
			);
			iNDiscrete -= 8;
			usBitOffset += 8;
		}
		return MB_ENOERR;
	}
	
	return eStatus;
}

eMBErrorCode eMBRegCoilsCB
(
	UCHAR * pucRegBuffer, USHORT usAddress,
	USHORT usNCoils, eMBRegisterMode eMode
)
{
    short           iNCoils = ( short )usNCoils;
    unsigned short  usBitOffset;
	
	/* Check if we have registers mapped at this block. */
	if( (usAddress >= REG_COILS_START) &&
		(usAddress + usNCoils <= REG_COILS_START + REG_COILS_SIZE)
	) {
		usBitOffset = (unsigned short)(usAddress - REG_COILS_START);
		switch(eMode) {
		/*
			Read current values and pass to protocol stack.
			MB_FUNC_READ_COILS						( 1 )
		*/
		case MB_REG_READ:
			while( iNCoils > 0 ) {
				*pucRegBuffer++ =
				xMBUtilGetBits( (unsigned char*)ucRegCoilsBuf, usBitOffset,
								(unsigned char)((iNCoils > 8) ? 8 : iNCoils)
				);
				usBitOffset += 8;
				iNCoils -= 8;
			}
		 return MB_ENOERR;
		 
		 /*
		 	Update current register values.
		 	MB_FUNC_WRITE_SINGLE_COIL				( 5 )
			MB_FUNC_WRITE_MULTIPLE_COILS			( 15 )
		 */
		 case MB_REG_WRITE:
		 	while( iNCoils > 0 ) {
				xMBUtilSetBits( (unsigned char*)ucRegCoilsBuf, usBitOffset,
								(unsigned char)((iNCoils > 8) ? 8 : iNCoils),
								*pucRegBuffer++
				);
				usBitOffset += 8;
				iNCoils -= 8;
			}
		 return MB_ENOERR;
		}
	}
	
	return MB_ENOREG;
}

eMBErrorCode eMBRegInputCB
(
	UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs
)
{
	unsigned int iRegIndex;
 
	// MB_FUNC_READ_INPUT_REGISTER           (  4 )
	if( (usAddress >= REG_INPUT_START) &&
		(usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS)
	) {
		iRegIndex = (int)(usAddress - REG_INPUT_START);
		while( usNRegs > 0 ) {
			*pucRegBuffer++ = (unsigned char)(uiRegInputBuf[iRegIndex] >> 8);
            *pucRegBuffer++ = (unsigned char)(uiRegInputBuf[iRegIndex] & 0xFF);
			++iRegIndex;
			--usNRegs;
		}
		return MB_ENOERR;
	}
	return MB_ENOREG;
}

eMBErrorCode eMBRegHoldingCB
(
	UCHAR * pucRegBuffer, USHORT usAddress,
	USHORT usNRegs, eMBRegisterMode eMode
)
{
	unsigned int iRegIndex;
	
	/* Check if we have registers mapped at this block. */
	if( (usAddress >= REG_HOLDING_START) &&
		(usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS)
	) {
		switch(eMode) {
		case MB_REG_READ:
			iRegIndex = (int)(usAddress - 1);
			while( usNRegs > 0 ) {
				*pucRegBuffer++ = uiRegHolding[iRegIndex]>>8;
				*pucRegBuffer++ = uiRegHolding[iRegIndex];
				++iRegIndex;
				--usNRegs;
			}
		 return MB_ENOERR;

		/*
		 	Update current register values.
		 	MB_FUNC_WRITE_MULTIPLE_REGISTERS             (16)
		*/
		case MB_REG_WRITE: {
			iRegIndex = (int)(usAddress - 1);
			while( usNRegs > 0 ) {
				uiRegHolding[iRegIndex]  = (*pucRegBuffer++)<<8;
				uiRegHolding[iRegIndex] |= *pucRegBuffer++;
				++iRegIndex;
				--usNRegs;
			}
		}
		 return MB_ENOERR;
		}
	}

	return MB_ENOREG;
}
///////////////////////////////////////////////////////////////////////////////////////////
//#define __ROBKO_01_SAVE_DATA_TO_DS1307__
void robko01_SavePos(void)
{	
#ifdef __ROBKO_01_SAVE_DATA_TO_DS1307__
	uint8_t i;

	///////////////////////////////////////////////
	char data;
	i2c_start( DS1307_ID, DS1307_ADDR, TW_WRITE );
	i2c_write( 0x00 );
	i2c_stop( );
	i2c_start( DS1307_ID, DS1307_ADDR, TW_READ );
	i2c_read( &data, ACK );
	i2c_stop( );
	// // // // // // // // // // // // // // // //
	i2c_start( DS1307_ID, DS1307_ADDR, TW_WRITE );
	i2c_write( 0x00 );
	i2c_write( 0x7F & data );
	i2c_stop( );
	///////////////////////////////////////////////

	// Start the I2C Write Transmission
	i2c_start( DS1307_ID, DS1307_ADDR, TW_WRITE );

	// Start from Address 0x08
	i2c_write( 0x08 );
	
	// Write the data to the DS1307 address start at 0x08
	// DS1307 automatically will increase the Address.
	for( i = 0; i < 6; i++ ) {
		i2c_write( arrMotor[i].pos>>8 );
		i2c_write( arrMotor[i].pos );

		i2c_write( arrMotor[i].step>>8 );
		i2c_write( arrMotor[i].step );

		i2c_write( arrMotor[i].state );

		i2c_write( arrMotor[i].direction );
	}
	// Stop I2C Transmission
	i2c_stop();
#else
	eeprom_write_block( &arrMotor, EEPROM_arrMotor, sizeof( EEPROM_arrMotor ) );
#endif
}

void robko01_LoadPos(void)
{
	uint8_t i;

#ifdef __ROBKO_01_SAVE_DATA_TO_DS1307__
	///////////////////////////////////////////////
	char data;
	i2c_start( DS1307_ID, DS1307_ADDR, TW_WRITE );
	i2c_write( 0x00 );
	i2c_stop( );
	i2c_start( DS1307_ID, DS1307_ADDR, TW_READ );
	i2c_read( &data, ACK );
	i2c_stop( );
	// // // // // // // // // // // // // // // //
	i2c_start( DS1307_ID, DS1307_ADDR, TW_WRITE );
	i2c_write( 0x00 );
	i2c_write( 0x7F & data );
	i2c_stop( );
	///////////////////////////////////////////////

	// First we initial the pointer register to address 0x00
	// Start the I2C Write Transmission
	i2c_start( DS1307_ID, DS1307_ADDR, TW_WRITE );
	
	// Start from Address 0x08
	i2c_write( 0x08 );
	
	// Stop I2C Transmission
	i2c_stop();
	
	// Start the I2C Read Transmission
	i2c_start( DS1307_ID, DS1307_ADDR, TW_READ );

	for( i = 0 ; i < 6 ; i++ ) {
		char data_h, data_l;

		i2c_read( &data_h, ACK );
		i2c_read( &data_l, ACK );
		arrMotor[i].pos  = (data_h<<8);
		arrMotor[i].pos |= (data_l<<0);

		i2c_read( &data_h, ACK );
		i2c_read( &data_l, ACK );
		arrMotor[i].step  = 0;(data_h<<8);
		arrMotor[i].step |= 0;(data_l<<0);

		i2c_read( (char*)&arrMotor[i].state, ACK );
		i2c_read( (char*)&arrMotor[i].direction, ACK );

		uiRegInputBuf[0 + i] = arrMotor[i].step;
		uiRegInputBuf[6 + i] = (unsigned int)arrMotor[i].pos;

		ucRegCoilsBuf[0] = arrMotor[i].direction ?  (1<<i) : 0;
	}
	// Stop I2C Transmission
	i2c_stop();
#else
	eeprom_read_block( &arrMotor, EEPROM_arrMotor, sizeof( EEPROM_arrMotor ) );
#endif

	for( i = 0 ; i < 6 ; i++ ) {
		uiRegInputBuf[0 + i] = arrMotor[i].step = 0;
		uiRegInputBuf[6 + i] = (unsigned int)arrMotor[i].pos;
		ucRegCoilsBuf[0] = arrMotor[i].direction ?  (1<<i) : 0;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////
ISR( INT3_vect )
{
	return;

	volatile static BI_STEP_MOTOR arrMotorTemp[6];
	eeprom_read_block( &arrMotorTemp, EEPROM_arrMotor, sizeof( arrMotorTemp ) );

	if( PINE & ( 1<<PE3 ) ) {
		int i;

		if( memcmp( &arrMotor, &arrMotorTemp, sizeof( arrMotor ) ) ) {
			robko01_SavePos();
		}

		for( i = 0; i < SIZE_OF_ARRAY( arrMotor ); i++ ) {
			arrMotor[i].step = 0;
		}

		while( PINE & ( 1<<PE3 ) ) {
			asm("nop\n");
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////
ISR( TIMER3_COMPA_vect )
{
	uint8_t i;
	static char n = 0;
	uint8_t newPorta = 0, newPortb = 0, newPortc = 0, newPortd = 0;

	TCNT3 = 0;
	if( OCR3A > 8000 ) {
		OCR3A -= 100;
	}

	if( uiModbusTimeOutCounter ) {
		--uiModbusTimeOutCounter;
	} else {
		memset( &ucRegCoilsBuf, 0, sizeof(ucRegCoilsBuf) );
		memset( &uiRegInputBuf, 0, sizeof(uiRegInputBuf) );
		memset( &uiRegHolding, 0, sizeof(uiRegHolding) );

		for( i = 0; i < SIZE_OF_ARRAY( arrMotor ); i++ ) {
			arrMotor[ i ].step = 0;
		}

		return;
	}

	if( !( PINE & ( 1<<PE3 ) ) ) {
		
		for( i = 0; i < SIZE_OF_ARRAY( arrMotor ); i++ ) {

			if( arrMotor[i].step ) {
				
				switch( i ) {
				case 0: newPorta |= ( 0x0F & arrStep[ arrMotor[i].state ] ); break;
				case 1: newPorta |= ( 0xF0 & arrStep[ arrMotor[i].state ] ); break;
				case 2: newPortc |= ( 0xF0 & arrStep[ arrMotor[i].state ] );
					// Запазва текущия ъгъл на хващача:
					if( !arrMotor[5].step ) {
						arrMotor[5].step = 1;
						arrMotor[5].direction = !arrMotor[2].direction;
						arrMotor[5].pos += arrMotor[5].direction ? 1 : -1;
					}
				 break;
				case 3: newPortc |= ( 0x0F & arrStep[ arrMotor[i].state ] ); break;
				case 4: newPortd |= ( 0xF0 & arrStep[ arrMotor[i].state ] ); break;
				case 5: newPortb |= ( 0xF0 & arrStep[ arrMotor[i].state ] ); break;
				}

				if( arrMotor[i].direction ) {
					if( ++arrMotor[i].state > SIZE_OF_ARRAY( arrStep ) - 1 ) {
						arrMotor[i].state = 0;
					}

					if( 3 != i ) --arrMotor[i].pos;
					else ++arrMotor[i].pos;
				} else {
					if( (char)--arrMotor[i].state < 0 ) {
						arrMotor[i].state = SIZE_OF_ARRAY( arrStep ) - 1;
					}

					if( 3 != i ) ++arrMotor[i].pos;
					else --arrMotor[i].pos;
				}

				--arrMotor[i].step;
			}

		}

		if( ++n > 1 ) {
			n = 0;
		}
	}

	PORTA = ( 0xFF & newPorta );
	PORTB = ( 0xF0 & newPortb ) | ( 0x0F & PORTB );
	PORTC = ( 0xFF & newPortc );
	PORTD = ( 0xF0 & newPortd ) | ( 0x0F & PORTD );
}
///////////////////////////////////////////////////////////////////////////////////////////
ISR(TIMER2_COMP_vect)
{
	/* fires 100 times a second / every 10ms */
	++timer_events;
}

void dhcp_client_event_callback(enum dhcp_client_event event)
{
	switch( event ) {
	case DHCP_CLIENT_EVT_LEASE_ACQUIRED:
//		printf_P(PSTR("[dhcp] lease acquired\n"));
	 break;
	case DHCP_CLIENT_EVT_LEASE_DENIED:
//		printf_P(PSTR("[dhcp] lease denied\n"));
		flagDhcp = 1;
	 break;
	case DHCP_CLIENT_EVT_LEASE_EXPIRING:
//		printf_P(PSTR("[dhcp] lease expiring\n"));
	 break;
	case DHCP_CLIENT_EVT_LEASE_EXPIRED:
//		printf_P(PSTR("[dhcp] lease expired\n"));
	 break;
	case DHCP_CLIENT_EVT_TIMEOUT:
//		printf_P(PSTR("[dhcp] timeout\n"));
		flagDhcp = 1;
	 break;
	case DHCP_CLIENT_EVT_ERROR:
//		printf_P(PSTR("[dhcp] error\n"));
		flagDhcp = 1;
	 break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////
