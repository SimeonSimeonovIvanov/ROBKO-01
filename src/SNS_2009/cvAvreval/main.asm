
;CodeVisionAVR C Compiler V1.25.6 Evaluation
;(C) Copyright 1998-2007 Pavel Haiduc, HP InfoTech s.r.l.
;http://www.hpinfotech.com

;Chip type              : ATmega32
;Program type           : Application
;Clock frequency        : 7,372800 MHz
;Memory model           : Small
;Optimize for           : Speed
;(s)printf features     : int, width
;(s)scanf features      : int, width
;External SRAM size     : 0
;Data Stack size        : 512 byte(s)
;Heap size              : 0 byte(s)
;Promote char to int    : No
;char is unsigned       : No
;8 bit enums            : Yes
;Word align FLASH struct: No
;Enhanced core instructions    : On
;Smart register allocation : On
;Automatic register allocation : On

	#pragma AVRPART ADMIN PART_NAME ATmega32
	#pragma AVRPART MEMORY PROG_FLASH 32768
	#pragma AVRPART MEMORY EEPROM 1024
	#pragma AVRPART MEMORY INT_SRAM SIZE 2048
	#pragma AVRPART MEMORY INT_SRAM START_ADDR 0x60

	.EQU UDRE=0x5
	.EQU RXC=0x7
	.EQU USR=0xB
	.EQU UDR=0xC
	.EQU SPSR=0xE
	.EQU SPDR=0xF
	.EQU EERE=0x0
	.EQU EEWE=0x1
	.EQU EEMWE=0x2
	.EQU EECR=0x1C
	.EQU EEDR=0x1D
	.EQU EEARL=0x1E
	.EQU EEARH=0x1F
	.EQU WDTCR=0x21
	.EQU MCUCR=0x35
	.EQU GICR=0x3B
	.EQU SPL=0x3D
	.EQU SPH=0x3E
	.EQU SREG=0x3F

	.DEF R0X0=R0
	.DEF R0X1=R1
	.DEF R0X2=R2
	.DEF R0X3=R3
	.DEF R0X4=R4
	.DEF R0X5=R5
	.DEF R0X6=R6
	.DEF R0X7=R7
	.DEF R0X8=R8
	.DEF R0X9=R9
	.DEF R0XA=R10
	.DEF R0XB=R11
	.DEF R0XC=R12
	.DEF R0XD=R13
	.DEF R0XE=R14
	.DEF R0XF=R15
	.DEF R0X10=R16
	.DEF R0X11=R17
	.DEF R0X12=R18
	.DEF R0X13=R19
	.DEF R0X14=R20
	.DEF R0X15=R21
	.DEF R0X16=R22
	.DEF R0X17=R23
	.DEF R0X18=R24
	.DEF R0X19=R25
	.DEF R0X1A=R26
	.DEF R0X1B=R27
	.DEF R0X1C=R28
	.DEF R0X1D=R29
	.DEF R0X1E=R30
	.DEF R0X1F=R31

	.MACRO __CPD1N
	CPI  R30,LOW(@0)
	LDI  R26,HIGH(@0)
	CPC  R31,R26
	LDI  R26,BYTE3(@0)
	CPC  R22,R26
	LDI  R26,BYTE4(@0)
	CPC  R23,R26
	.ENDM

	.MACRO __CPD2N
	CPI  R26,LOW(@0)
	LDI  R30,HIGH(@0)
	CPC  R27,R30
	LDI  R30,BYTE3(@0)
	CPC  R24,R30
	LDI  R30,BYTE4(@0)
	CPC  R25,R30
	.ENDM

	.MACRO __CPWRR
	CP   R@0,R@2
	CPC  R@1,R@3
	.ENDM

	.MACRO __CPWRN
	CPI  R@0,LOW(@2)
	LDI  R30,HIGH(@2)
	CPC  R@1,R30
	.ENDM

	.MACRO __ADDB1MN
	SUBI R30,LOW(-@0-(@1))
	.ENDM
	.MACRO __ADDB2MN
	SUBI R26,LOW(-@0-(@1))
	.ENDM
	.MACRO __ADDW1MN
	SUBI R30,LOW(-@0-(@1))
	SBCI R31,HIGH(-@0-(@1))
	.ENDM
	.MACRO __ADDW2MN
	SUBI R26,LOW(-@0-(@1))
	SBCI R27,HIGH(-@0-(@1))
	.ENDM
	.MACRO __ADDW1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	.ENDM
	.MACRO __ADDD1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	SBCI R22,BYTE3(-2*@0-(@1))
	.ENDM
	.MACRO __ADDD1N
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	SBCI R22,BYTE3(-@0)
	SBCI R23,BYTE4(-@0)
	.ENDM

	.MACRO __ADDD2N
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	SBCI R24,BYTE3(-@0)
	SBCI R25,BYTE4(-@0)
	.ENDM

	.MACRO __SUBD1N
	SUBI R30,LOW(@0)
	SBCI R31,HIGH(@0)
	SBCI R22,BYTE3(@0)
	SBCI R23,BYTE4(@0)
	.ENDM

	.MACRO __SUBD2N
	SUBI R26,LOW(@0)
	SBCI R27,HIGH(@0)
	SBCI R24,BYTE3(@0)
	SBCI R25,BYTE4(@0)
	.ENDM

	.MACRO __ANDBMNN
	LDS  R30,@0+@1
	ANDI R30,LOW(@2)
	STS  @0+@1,R30
	.ENDM

	.MACRO __ANDWMNN
	LDS  R30,@0+@1
	ANDI R30,LOW(@2)
	STS  @0+@1,R30
	LDS  R30,@0+@1+1
	ANDI R30,HIGH(@2)
	STS  @0+@1+1,R30
	.ENDM

	.MACRO __ANDD1N
	ANDI R30,LOW(@0)
	ANDI R31,HIGH(@0)
	ANDI R22,BYTE3(@0)
	ANDI R23,BYTE4(@0)
	.ENDM

	.MACRO __ORBMNN
	LDS  R30,@0+@1
	ORI  R30,LOW(@2)
	STS  @0+@1,R30
	.ENDM

	.MACRO __ORWMNN
	LDS  R30,@0+@1
	ORI  R30,LOW(@2)
	STS  @0+@1,R30
	LDS  R30,@0+@1+1
	ORI  R30,HIGH(@2)
	STS  @0+@1+1,R30
	.ENDM

	.MACRO __ORD1N
	ORI  R30,LOW(@0)
	ORI  R31,HIGH(@0)
	ORI  R22,BYTE3(@0)
	ORI  R23,BYTE4(@0)
	.ENDM

	.MACRO __DELAY_USB
	LDI  R24,LOW(@0)
__DELAY_USB_LOOP:
	DEC  R24
	BRNE __DELAY_USB_LOOP
	.ENDM

	.MACRO __DELAY_USW
	LDI  R24,LOW(@0)
	LDI  R25,HIGH(@0)
__DELAY_USW_LOOP:
	SBIW R24,1
	BRNE __DELAY_USW_LOOP
	.ENDM

	.MACRO __CLRD1S
	LDI  R30,0
	STD  Y+@0,R30
	STD  Y+@0+1,R30
	STD  Y+@0+2,R30
	STD  Y+@0+3,R30
	.ENDM

	.MACRO __GETD1S
	LDD  R30,Y+@0
	LDD  R31,Y+@0+1
	LDD  R22,Y+@0+2
	LDD  R23,Y+@0+3
	.ENDM

	.MACRO __PUTD1S
	STD  Y+@0,R30
	STD  Y+@0+1,R31
	STD  Y+@0+2,R22
	STD  Y+@0+3,R23
	.ENDM

	.MACRO __PUTD2S
	STD  Y+@0,R26
	STD  Y+@0+1,R27
	STD  Y+@0+2,R24
	STD  Y+@0+3,R25
	.ENDM

	.MACRO __POINTB1MN
	LDI  R30,LOW(@0+@1)
	.ENDM

	.MACRO __POINTW1MN
	LDI  R30,LOW(@0+@1)
	LDI  R31,HIGH(@0+@1)
	.ENDM

	.MACRO __POINTD1M
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __POINTW1FN
	LDI  R30,LOW(2*@0+@1)
	LDI  R31,HIGH(2*@0+@1)
	.ENDM

	.MACRO __POINTD1FN
	LDI  R30,LOW(2*@0+@1)
	LDI  R31,HIGH(2*@0+@1)
	LDI  R22,BYTE3(2*@0+@1)
	LDI  R23,BYTE4(2*@0+@1)
	.ENDM

	.MACRO __POINTB2MN
	LDI  R26,LOW(@0+@1)
	.ENDM

	.MACRO __POINTW2MN
	LDI  R26,LOW(@0+@1)
	LDI  R27,HIGH(@0+@1)
	.ENDM

	.MACRO __POINTBRM
	LDI  R@0,LOW(@1)
	.ENDM

	.MACRO __POINTWRM
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __POINTBRMN
	LDI  R@0,LOW(@1+@2)
	.ENDM

	.MACRO __POINTWRMN
	LDI  R@0,LOW(@2+@3)
	LDI  R@1,HIGH(@2+@3)
	.ENDM

	.MACRO __POINTWRFN
	LDI  R@0,LOW(@2*2+@3)
	LDI  R@1,HIGH(@2*2+@3)
	.ENDM

	.MACRO __GETD1N
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __GETD2N
	LDI  R26,LOW(@0)
	LDI  R27,HIGH(@0)
	LDI  R24,BYTE3(@0)
	LDI  R25,BYTE4(@0)
	.ENDM

	.MACRO __GETD2S
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	LDD  R24,Y+@0+2
	LDD  R25,Y+@0+3
	.ENDM

	.MACRO __GETB1MN
	LDS  R30,@0+@1
	.ENDM

	.MACRO __GETB1HMN
	LDS  R31,@0+@1
	.ENDM

	.MACRO __GETW1MN
	LDS  R30,@0+@1
	LDS  R31,@0+@1+1
	.ENDM

	.MACRO __GETD1MN
	LDS  R30,@0+@1
	LDS  R31,@0+@1+1
	LDS  R22,@0+@1+2
	LDS  R23,@0+@1+3
	.ENDM

	.MACRO __GETBRMN
	LDS  R@0,@1+@2
	.ENDM

	.MACRO __GETWRMN
	LDS  R@0,@2+@3
	LDS  R@1,@2+@3+1
	.ENDM

	.MACRO __GETWRZ
	LDD  R@0,Z+@2
	LDD  R@1,Z+@2+1
	.ENDM

	.MACRO __GETD2Z
	LDD  R26,Z+@0
	LDD  R27,Z+@0+1
	LDD  R24,Z+@0+2
	LDD  R25,Z+@0+3
	.ENDM

	.MACRO __GETB2MN
	LDS  R26,@0+@1
	.ENDM

	.MACRO __GETW2MN
	LDS  R26,@0+@1
	LDS  R27,@0+@1+1
	.ENDM

	.MACRO __GETD2MN
	LDS  R26,@0+@1
	LDS  R27,@0+@1+1
	LDS  R24,@0+@1+2
	LDS  R25,@0+@1+3
	.ENDM

	.MACRO __PUTB1MN
	STS  @0+@1,R30
	.ENDM

	.MACRO __PUTW1MN
	STS  @0+@1,R30
	STS  @0+@1+1,R31
	.ENDM

	.MACRO __PUTD1MN
	STS  @0+@1,R30
	STS  @0+@1+1,R31
	STS  @0+@1+2,R22
	STS  @0+@1+3,R23
	.ENDM

	.MACRO __PUTBR0MN
	STS  @0+@1,R0
	.ENDM

	.MACRO __PUTDZ2
	STD  Z+@0,R26
	STD  Z+@0+1,R27
	STD  Z+@0+2,R24
	STD  Z+@0+3,R25
	.ENDM

	.MACRO __PUTBMRN
	STS  @0+@1,R@2
	.ENDM

	.MACRO __PUTWMRN
	STS  @0+@1,R@2
	STS  @0+@1+1,R@3
	.ENDM

	.MACRO __PUTBZR
	STD  Z+@1,R@0
	.ENDM

	.MACRO __PUTWZR
	STD  Z+@2,R@0
	STD  Z+@2+1,R@1
	.ENDM

	.MACRO __GETW1R
	MOV  R30,R@0
	MOV  R31,R@1
	.ENDM

	.MACRO __GETW2R
	MOV  R26,R@0
	MOV  R27,R@1
	.ENDM

	.MACRO __GETWRN
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __PUTW1R
	MOV  R@0,R30
	MOV  R@1,R31
	.ENDM

	.MACRO __PUTW2R
	MOV  R@0,R26
	MOV  R@1,R27
	.ENDM

	.MACRO __ADDWRN
	SUBI R@0,LOW(-@2)
	SBCI R@1,HIGH(-@2)
	.ENDM

	.MACRO __ADDWRR
	ADD  R@0,R@2
	ADC  R@1,R@3
	.ENDM

	.MACRO __SUBWRN
	SUBI R@0,LOW(@2)
	SBCI R@1,HIGH(@2)
	.ENDM

	.MACRO __SUBWRR
	SUB  R@0,R@2
	SBC  R@1,R@3
	.ENDM

	.MACRO __ANDWRN
	ANDI R@0,LOW(@2)
	ANDI R@1,HIGH(@2)
	.ENDM

	.MACRO __ANDWRR
	AND  R@0,R@2
	AND  R@1,R@3
	.ENDM

	.MACRO __ORWRN
	ORI  R@0,LOW(@2)
	ORI  R@1,HIGH(@2)
	.ENDM

	.MACRO __ORWRR
	OR   R@0,R@2
	OR   R@1,R@3
	.ENDM

	.MACRO __EORWRR
	EOR  R@0,R@2
	EOR  R@1,R@3
	.ENDM

	.MACRO __GETWRS
	LDD  R@0,Y+@2
	LDD  R@1,Y+@2+1
	.ENDM

	.MACRO __PUTWSR
	STD  Y+@2,R@0
	STD  Y+@2+1,R@1
	.ENDM

	.MACRO __MOVEWRR
	MOV  R@0,R@2
	MOV  R@1,R@3
	.ENDM

	.MACRO __INWR
	IN   R@0,@2
	IN   R@1,@2+1
	.ENDM

	.MACRO __OUTWR
	OUT  @2+1,R@1
	OUT  @2,R@0
	.ENDM

	.MACRO __CALL1MN
	LDS  R30,@0+@1
	LDS  R31,@0+@1+1
	ICALL
	.ENDM

	.MACRO __CALL1FN
	LDI  R30,LOW(2*@0+@1)
	LDI  R31,HIGH(2*@0+@1)
	CALL __GETW1PF
	ICALL
	.ENDM

	.MACRO __CALL2EN
	LDI  R26,LOW(@0+@1)
	LDI  R27,HIGH(@0+@1)
	CALL __EEPROMRDW
	ICALL
	.ENDM

	.MACRO __GETW1STACK
	IN   R26,SPL
	IN   R27,SPH
	ADIW R26,@0+1
	LD   R30,X+
	LD   R31,X
	.ENDM

	.MACRO __NBST
	BST  R@0,@1
	IN   R30,SREG
	LDI  R31,0x40
	EOR  R30,R31
	OUT  SREG,R30
	.ENDM


	.MACRO __PUTB1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RNS
	MOVW R26,R@0
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	CALL __PUTDP1
	.ENDM


	.MACRO __GETB1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R30,Z
	.ENDM

	.MACRO __GETB1HSX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	.ENDM

	.MACRO __GETW1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R31,Z
	MOV  R30,R0
	.ENDM

	.MACRO __GETD1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R1,Z+
	LD   R22,Z+
	LD   R23,Z
	MOVW R30,R0
	.ENDM

	.MACRO __GETB2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R26,X
	.ENDM

	.MACRO __GETW2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	.ENDM

	.MACRO __GETD2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R1,X+
	LD   R24,X+
	LD   R25,X
	MOVW R26,R0
	.ENDM

	.MACRO __GETBRSX
	MOVW R30,R28
	SUBI R30,LOW(-@1)
	SBCI R31,HIGH(-@1)
	LD   R@0,Z
	.ENDM

	.MACRO __GETWRSX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	LD   R@0,Z+
	LD   R@1,Z
	.ENDM

	.MACRO __LSLW8SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	CLR  R30
	.ENDM

	.MACRO __PUTB1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __CLRW1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	CLR  R0
	ST   Z+,R0
	ST   Z,R0
	.ENDM

	.MACRO __CLRD1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	CLR  R0
	ST   Z+,R0
	ST   Z+,R0
	ST   Z+,R0
	ST   Z,R0
	.ENDM

	.MACRO __PUTB2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z,R26
	.ENDM

	.MACRO __PUTW2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z,R27
	.ENDM

	.MACRO __PUTD2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z+,R27
	ST   Z+,R24
	ST   Z,R25
	.ENDM

	.MACRO __PUTBSRX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z,R@1
	.ENDM

	.MACRO __PUTWSRX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	ST   Z+,R@0
	ST   Z,R@1
	.ENDM

	.MACRO __PUTB1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __MULBRR
	MULS R@0,R@1
	MOVW R30,R0
	.ENDM

	.MACRO __MULBRRU
	MUL  R@0,R@1
	MOVW R30,R0
	.ENDM

	.MACRO __MULBRR0
	MULS R@0,R@1
	.ENDM

	.MACRO __MULBRRU0
	MUL  R@0,R@1
	.ENDM

	.MACRO __MULBNWRU
	LDI  R26,@2
	MUL  R26,R@0
	MOVW R30,R0
	MUL  R26,R@1
	ADD  R31,R0
	.ENDM

	.CSEG
	.ORG 0

	.INCLUDE "main.vec"
	.INCLUDE "main.inc"

__RESET:
	CLI
	CLR  R30
	OUT  EECR,R30

;INTERRUPT VECTORS ARE PLACED
;AT THE START OF FLASH
	LDI  R31,1
	OUT  GICR,R31
	OUT  GICR,R30
	OUT  MCUCR,R30

;DISABLE WATCHDOG
	LDI  R31,0x18
	OUT  WDTCR,R31
	OUT  WDTCR,R30

;CLEAR R2-R14
	LDI  R24,13
	LDI  R26,2
	CLR  R27
__CLEAR_REG:
	ST   X+,R30
	DEC  R24
	BRNE __CLEAR_REG

;CLEAR SRAM
	LDI  R24,LOW(0x800)
	LDI  R25,HIGH(0x800)
	LDI  R26,0x60
__CLEAR_SRAM:
	ST   X+,R30
	SBIW R24,1
	BRNE __CLEAR_SRAM

;GLOBAL VARIABLES INITIALIZATION
	LDI  R30,LOW(__GLOBAL_INI_TBL*2)
	LDI  R31,HIGH(__GLOBAL_INI_TBL*2)
__GLOBAL_INI_NEXT:
	LPM  R24,Z+
	LPM  R25,Z+
	SBIW R24,0
	BREQ __GLOBAL_INI_END
	LPM  R26,Z+
	LPM  R27,Z+
	LPM  R0,Z+
	LPM  R1,Z+
	MOVW R22,R30
	MOVW R30,R0
__GLOBAL_INI_LOOP:
	LPM  R0,Z+
	ST   X+,R0
	SBIW R24,1
	BRNE __GLOBAL_INI_LOOP
	MOVW R30,R22
	RJMP __GLOBAL_INI_NEXT
__GLOBAL_INI_END:

;STACK POINTER INITIALIZATION
	LDI  R30,LOW(0x85F)
	OUT  SPL,R30
	LDI  R30,HIGH(0x85F)
	OUT  SPH,R30

;DATA STACK POINTER INITIALIZATION
	LDI  R28,LOW(0x260)
	LDI  R29,HIGH(0x260)

	JMP  _main

	.ESEG
	.ORG 0

	.DSEG
	.ORG 0x260
;       1 /*
;       2 		The^day^of^DooM
;       3 
;       4 	Create Date:	14.03.2009
;       5 	Last Update:	11.04.2009
;       6 	Full Channge:	12.04.2009 - For cvAvreval
;       7 	Last Update:	18.04.2009
;       8 
;       9 	Project->Configuration option:
;      10 		1) Кварц: 7.3728MHz
;      11 		3) Ниво на оптимизация: -Os
;      12 
;      13 	PA0/PA3 - motor 0
;      14 	PA4/PA7 - motor 1
;      15 	PB0/PB3 - motor 2
;      16 	PB4/PB7 - motor 3
;      17 	PC0/PC3 - motor 4
;      18 	PC4/PC7 - motor 5
;      19 */
;      20 
;      21 #include <delay.h>
;      22 #include <stdio.h> // sprintf
;      23 #include <mega32.h>
;      24 	#ifndef __SLEEP_DEFINED__
	#ifndef __SLEEP_DEFINED__
;      25 	#define __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
;      26 	.EQU __se_bit=0x80
	.EQU __se_bit=0x80
;      27 	.EQU __sm_mask=0x70
	.EQU __sm_mask=0x70
;      28 	.EQU __sm_powerdown=0x20
	.EQU __sm_powerdown=0x20
;      29 	.EQU __sm_powersave=0x30
	.EQU __sm_powersave=0x30
;      30 	.EQU __sm_standby=0x60
	.EQU __sm_standby=0x60
;      31 	.EQU __sm_ext_standby=0x70
	.EQU __sm_ext_standby=0x70
;      32 	.EQU __sm_adc_noise_red=0x10
	.EQU __sm_adc_noise_red=0x10
;      33 	.SET power_ctrl_reg=mcucr
	.SET power_ctrl_reg=mcucr
;      34 	#endif
	#endif
;      35 #include <string.h>
;      36 #include <math.h>
;      37 #include "video.h"

	.CSEG
_videoSendData:
;	data -> Y+0
_0x3:
	SBIC 0x10,6
	RJMP _0x3
_0x6:
	SBIS 0xB,5
	RJMP _0x6
	LD   R30,Y
	OUT  0xC,R30
	ADIW R28,1
	RET
;	x -> Y+2
;	y -> Y+1
;	c -> Y+0
_videoPutStrXY:
	ST   -Y,R17
;	x -> Y+4
;	y -> Y+3
;	*str -> Y+1
;	i -> R17
	LDI  R17,0
	LDI  R30,LOW(2)
	ST   -Y,R30
	CALL _videoSendData
	LDD  R30,Y+4
	ST   -Y,R30
	CALL _videoSendData
	LDD  R30,Y+3
	ST   -Y,R30
	CALL _videoSendData
_0xA:
	MOV  R30,R17
	LDD  R26,Y+1
	LDD  R27,Y+1+1
	LDI  R31,0
	ADD  R26,R30
	ADC  R27,R31
	LD   R30,X
	ST   -Y,R30
	CALL _videoSendData
	MOV  R30,R17
	SUBI R17,-1
	LDD  R26,Y+1
	LDD  R27,Y+1+1
	LDI  R31,0
	ADD  R26,R30
	ADC  R27,R31
	LD   R30,X
	CPI  R30,0
	BRNE _0xA
	LDD  R17,Y+0
	ADIW R28,5
	RET
_videoInit:
	LDI  R30,LOW(0)
	OUT  0xB,R30
	LDI  R30,LOW(8)
	OUT  0xA,R30
	LDI  R30,LOW(134)
	OUT  0x20,R30
	LDI  R30,LOW(0)
	OUT  0x20,R30
	LDI  R30,LOW(5)
	OUT  0x9,R30
	RET
;      38 #include "mouse.h"
;      39         .equ PORTD = 0x12
        .equ PORTD = 0x12
;      40         .equ DDRD  = 0x11
        .equ DDRD  = 0x11       
;      41         .MACRO DATALOW
        .MACRO DATALOW
;      42         sbi DDRD, 3
        sbi DDRD, 3
;      43         cbi PORTD, 3
        cbi PORTD, 3
;      44         .ENDM
        .ENDM
;      45         .MACRO RELDATA
        .MACRO RELDATA
;      46         cbi DDRD, 3
        cbi DDRD, 3
;      47         sbi PORTD, 3
        sbi PORTD, 3
;      48         .ENDM
        .ENDM
;      49         .MACRO CLKLOW
        .MACRO CLKLOW
;      50         sbi DDRD, 2
        sbi DDRD, 2
;      51         cbi PORTD, 2
        cbi PORTD, 2
;      52         .ENDM
        .ENDM
;      53         .MACRO RELCLK
        .MACRO RELCLK
;      54         cbi DDRD, 2
        cbi DDRD, 2
;      55         sbi PORTD, 2
        sbi PORTD, 2
;      56         .ENDM
        .ENDM
_mouse_send:
	CALL __SAVELOCR4
;	data -> Y+4
;	Bit -> R17
;	parity -> R16
;	i -> R19
	RELDATA
	RELCLK
	__DELAY_USW 553
	CLKLOW
	__DELAY_USW 553
	DATALOW
	__DELAY_USB 246
	RELCLK
	__DELAY_USB 98
_0xC:
	SBIC 0x10,2
	RJMP _0xC
	LDI  R19,LOW(0)
	LDI  R17,LOW(1)
	LDI  R16,LOW(1)
_0xF:
	CPI  R19,9
	BRSH _0x11
	CPI  R19,8
	BRNE _0x12
	CPI  R16,0
	BREQ _0x13
	RELDATA
	JMP  _0x14
_0x13:
	DATALOW
_0x14:
	JMP  _0x15
_0x12:
	MOV  R30,R17
	LDD  R26,Y+4
	AND  R30,R26
	BREQ _0x16
	RELDATA
	JMP  _0x17
_0x16:
	DATALOW
_0x17:
_0x15:
_0x18:
	SBIS 0x10,2
	RJMP _0x18
_0x1B:
	SBIC 0x10,2
	RJMP _0x1B
	LDI  R30,0
	SBIC 0x12,3
	LDI  R30,1
	EOR  R16,R30
	LSL  R17
	SUBI R19,-1
	JMP  _0xF
_0x11:
	RELDATA
	__DELAY_USB 123
_0x1E:
	SBIC 0x10,3
	RJMP _0x1E
_0x21:
	SBIC 0x10,2
	RJMP _0x21
_0x24:
	SBIS 0x10,3
	RJMP _0x27
	SBIC 0x10,2
	RJMP _0x26
_0x27:
	RJMP _0x24
_0x26:
	CLKLOW
	CALL __LOADLOCR4
	ADIW R28,5
	RET
_mouse_read:
	SBIW R28,10
	ST   -Y,R17
;	i -> R17
;	t -> Y+1
	RELCLK
	RELDATA
	__DELAY_USW 369
_0x29:
	SBIC 0x10,2
	RJMP _0x29
	LDI  R17,LOW(0)
	__DELAY_USB 12
_0x2C:
	SBIS 0x10,2
	RJMP _0x2C
_0x2F:
	CPI  R17,9
	BRSH _0x31
_0x32:
	SBIC 0x10,2
	RJMP _0x32
	MOV  R30,R17
	LDI  R31,0
	MOVW R26,R28
	ADIW R26,1
	ADD  R26,R30
	ADC  R27,R31
	LDI  R30,0
	SBIC 0x10,3
	LDI  R30,1
	ST   X,R30
	SUBI R17,-1
_0x35:
	SBIS 0x10,2
	RJMP _0x35
	RJMP _0x2F
_0x31:
_0x38:
	SBIS 0x10,2
	RJMP _0x38
	CLKLOW
	LDD  R30,Y+8
	LDI  R26,LOW(128)
	MUL  R30,R26
	MOVW R30,R0
	MOV  R22,R30
	LDD  R30,Y+7
	LDI  R26,LOW(64)
	MUL  R30,R26
	MOVW R30,R0
	ADD  R22,R30
	LDD  R30,Y+6
	LDI  R26,LOW(32)
	MUL  R30,R26
	MOVW R30,R0
	ADD  R22,R30
	LDD  R30,Y+5
	LDI  R26,LOW(16)
	MUL  R30,R26
	MOVW R30,R0
	ADD  R22,R30
	LDD  R30,Y+4
	LDI  R26,LOW(8)
	MUL  R30,R26
	MOVW R30,R0
	ADD  R22,R30
	LDD  R30,Y+3
	LDI  R26,LOW(4)
	MUL  R30,R26
	MOVW R30,R0
	ADD  R22,R30
	LDD  R30,Y+2
	LDI  R26,LOW(2)
	MUL  R30,R26
	MOVW R30,R0
	ADD  R30,R22
	LDD  R26,Y+1
	ADD  R30,R26
	LDD  R17,Y+0
	ADIW R28,11
	RET
_reset_mouse:
	LDI  R30,LOW(255)
	ST   -Y,R30
	CALL _mouse_send
	CALL _mouse_read
	CALL _mouse_read
	CALL _mouse_read
	LDI  R30,LOW(240)
	ST   -Y,R30
	CALL _mouse_send
	CALL _mouse_read
	LDI  R30,LOW(100)
	LDI  R31,HIGH(100)
	ST   -Y,R31
	ST   -Y,R30
	CALL _delay_ms
	RET
;	*x -> Y+9
;	*y -> Y+7
;	cursor_x -> R16,R17
;	cursor_y -> R18,R19
;	mouse_status -> R21
;	mouse_y -> R20
;	mouse_x -> Y+6

	.DSEG

	.CSEG
;	cursor_x -> Y+1
;	cursor_y -> Y+0
_mouseInit:
	IN   R30,0x11
	ANDI R30,LOW(0xF3)
	OUT  0x11,R30
	SBI  0x11,3
	SBI  0x12,3
	LDI  R30,LOW(500)
	LDI  R31,HIGH(500)
	ST   -Y,R31
	ST   -Y,R30
	CALL _delay_ms
	IN   R30,0x11
	ANDI R30,LOW(0xF3)
	OUT  0x11,R30
	SBI  0x11,2
	SBI  0x12,2
	LDI  R30,LOW(500)
	LDI  R31,HIGH(500)
	ST   -Y,R31
	ST   -Y,R30
	CALL _delay_ms
	IN   R30,0x11
	ANDI R30,LOW(0xF3)
	OUT  0x11,R30
	RELCLK
	RELDATA
	CALL _reset_mouse
	RET
;      57 
;      58 #define CS11    1
;      59 #define OCIE1A  4
;      60 /* PORTD */
;      61 #define PD7     7
;      62 #define PD6     6
;      63 #define PD5     5
;      64 #define PD4     4
;      65 #define PD3     3
;      66 #define PD2     2
;      67 #define PD1     1
;      68 #define PD0     0
;      69 
;      70 typedef struct {
;      71 	unsigned int step;
;      72 	unsigned char direction, state;
;      73 } BI_STEP_MOTOR, *BI_STEP_MOPTOR;
;      74 
;      75 volatile char arrStep[] = {

	.DSEG
_arrStep:
;      76 	0b00001000,
;      77 	0b00001010,
;      78 	0b00000010,
;      79 	0b00000110,
;      80 	0b00000100,
;      81 	0b00000101,
;      82 	0b00000001,
;      83 	0b00001001
;      84 };
	.BYTE 0x8
;      85 
;      86 volatile BI_STEP_MOTOR arrMotor[6];
_arrMotor:
	.BYTE 0x18
;      87 
;      88 typedef struct {
;      89         unsigned char motor, direction;
;      90 	unsigned int step;
;      91 } ROBKO_MOTOR_POSITION;
;      92 
;      93 //#define DEBUG
;      94 
;      95 //interrupt [TIM1_COMPA]
;      96 void timer1_compa_isr(void)
;      97 {

	.CSEG
_timer1_compa_isr:
;      98 	unsigned char i;
;      99 	unsigned char newPorta = 0, newPortb =0, newPortc = 0;
;     100 
;     101 	TCNT1 = 0;
	CALL __SAVELOCR4
;	i -> R17
;	newPorta -> R16
;	newPortb -> R19
;	newPortc -> R18
	LDI  R16,0
	LDI  R18,0
	LDI  R19,0
	LDI  R30,LOW(0)
	LDI  R31,HIGH(0)
	OUT  0x2C+1,R31
	OUT  0x2C,R30
;     102 
;     103 	for(i=0; i<sizeof(arrMotor)/sizeof(*arrMotor); i++) {
	LDI  R17,LOW(0)
_0x4D:
	CPI  R17,1
	BRLO PC+3
	JMP _0x4E
;     104 
;     105 		if(arrMotor[i].step) {
	LDI  R30,LOW(4)
	MUL  R30,R17
	MOVW R30,R0
	SUBI R30,LOW(-_arrMotor)
	SBCI R31,HIGH(-_arrMotor)
	MOVW R26,R30
	CALL __GETW1P
	SBIW R30,0
	BRNE PC+3
	JMP _0x4F
;     106 			switch(i) {
	MOV  R30,R17
;     107 			case 0: newPorta |= arrStep[arrMotor[i].state]; break;
	CPI  R30,0
	BRNE _0x53
	LDI  R30,LOW(4)
	MUL  R30,R17
	MOVW R30,R0
	__ADDW1MN _arrMotor,3
	LD   R30,Z
	LDI  R31,0
	SUBI R30,LOW(-_arrStep)
	SBCI R31,HIGH(-_arrStep)
	LD   R30,Z
	OR   R16,R30
	RJMP _0x52
;     108 			case 1: newPorta |= arrStep[arrMotor[i].state]<<4; break;
_0x53:
	CPI  R30,LOW(0x1)
	BRNE _0x54
	LDI  R30,LOW(4)
	MUL  R30,R17
	MOVW R30,R0
	__ADDW1MN _arrMotor,3
	LD   R30,Z
	LDI  R31,0
	SUBI R30,LOW(-_arrStep)
	SBCI R31,HIGH(-_arrStep)
	LD   R30,Z
	SWAP R30
	ANDI R30,0xF0
	OR   R16,R30
	RJMP _0x52
;     109 			case 2: newPortb |= arrStep[arrMotor[i].state]; break;
_0x54:
	CPI  R30,LOW(0x2)
	BRNE _0x55
	LDI  R30,LOW(4)
	MUL  R30,R17
	MOVW R30,R0
	__ADDW1MN _arrMotor,3
	LD   R30,Z
	LDI  R31,0
	SUBI R30,LOW(-_arrStep)
	SBCI R31,HIGH(-_arrStep)
	LD   R30,Z
	OR   R19,R30
	RJMP _0x52
;     110 			case 3: newPortb |= arrStep[arrMotor[i].state]<<4; break;
_0x55:
	CPI  R30,LOW(0x3)
	BRNE _0x56
	LDI  R30,LOW(4)
	MUL  R30,R17
	MOVW R30,R0
	__ADDW1MN _arrMotor,3
	LD   R30,Z
	LDI  R31,0
	SUBI R30,LOW(-_arrStep)
	SBCI R31,HIGH(-_arrStep)
	LD   R30,Z
	SWAP R30
	ANDI R30,0xF0
	OR   R19,R30
	RJMP _0x52
;     111 			case 4: newPortc |= arrStep[arrMotor[i].state]; break;
_0x56:
	CPI  R30,LOW(0x4)
	BRNE _0x57
	LDI  R30,LOW(4)
	MUL  R30,R17
	MOVW R30,R0
	__ADDW1MN _arrMotor,3
	LD   R30,Z
	LDI  R31,0
	SUBI R30,LOW(-_arrStep)
	SBCI R31,HIGH(-_arrStep)
	LD   R30,Z
	RJMP _0x133
;     112 			case 5: newPortc |= arrStep[arrMotor[i].state]<<4;
_0x57:
	CPI  R30,LOW(0x5)
	BRNE _0x52
	LDI  R30,LOW(4)
	MUL  R30,R17
	MOVW R30,R0
	__ADDW1MN _arrMotor,3
	LD   R30,Z
	LDI  R31,0
	SUBI R30,LOW(-_arrStep)
	SBCI R31,HIGH(-_arrStep)
	LD   R30,Z
	SWAP R30
	ANDI R30,0xF0
_0x133:
	OR   R18,R30
;     113 			}
_0x52:
;     114 
;     115 			if(arrMotor[i].direction) {
	LDI  R30,LOW(4)
	MUL  R30,R17
	MOVW R30,R0
	__ADDW1MN _arrMotor,2
	LD   R30,Z
	CPI  R30,0
	BREQ _0x59
;     116 				if(++arrMotor[i].state > sizeof(arrStep)-1) {
	LDI  R30,LOW(4)
	MUL  R30,R17
	MOVW R30,R0
	__ADDW1MN _arrMotor,3
	MOVW R26,R30
	LD   R30,X
	SUBI R30,-LOW(1)
	ST   X,R30
	CPI  R30,LOW(0x8)
	BRLO _0x5A
;     117 					arrMotor[i].state = 0;
	LDI  R30,LOW(4)
	MUL  R30,R17
	MOVW R30,R0
	__ADDW1MN _arrMotor,3
	LDI  R26,LOW(0)
	STD  Z+0,R26
;     118 					--arrMotor[i].step;
	LDI  R30,LOW(4)
	MUL  R30,R17
	MOVW R30,R0
	SUBI R30,LOW(-_arrMotor)
	SBCI R31,HIGH(-_arrMotor)
	MOVW R26,R30
	LD   R30,X+
	LD   R31,X+
	SBIW R30,1
	ST   -X,R31
	ST   -X,R30
;     119 				}
;     120 			} else {
_0x5A:
	RJMP _0x5B
_0x59:
;     121 				if((char)arrMotor[i].state-- < 1) {
	LDI  R30,LOW(4)
	MUL  R30,R17
	MOVW R30,R0
	__ADDW1MN _arrMotor,3
	MOVW R26,R30
	LD   R30,X
	SUBI R30,LOW(1)
	ST   X,R30
	SUBI R30,-LOW(1)
	CPI  R30,LOW(0x1)
	BRGE _0x5C
;     122 					arrMotor[i].state = sizeof(arrStep)-1;
	LDI  R30,LOW(4)
	MUL  R30,R17
	MOVW R30,R0
	__ADDW1MN _arrMotor,3
	LDI  R26,LOW(7)
	STD  Z+0,R26
;     123 					--arrMotor[i].step;
	LDI  R30,LOW(4)
	MUL  R30,R17
	MOVW R30,R0
	SUBI R30,LOW(-_arrMotor)
	SBCI R31,HIGH(-_arrMotor)
	MOVW R26,R30
	LD   R30,X+
	LD   R31,X+
	SBIW R30,1
	ST   -X,R31
	ST   -X,R30
;     124 				}
;     125 			}
_0x5C:
_0x5B:
;     126 		}
;     127 	}
_0x4F:
	SUBI R17,-1
	RJMP _0x4D
_0x4E:
;     128 
;     129 	PORTA = newPorta;
	OUT  0x1B,R16
;     130 	PORTB = newPortb;
	OUT  0x18,R19
;     131 	PORTC = newPortc;
	OUT  0x15,R18
;     132 }
	CALL __LOADLOCR4
	ADIW R28,4
	RET
;     133 
;     134 
;     135 int main(void)
;     136 {
_main:
;     137     int n = 0, m = 0, dx = 0, dy = 0;
;     138 	unsigned char x, y, mouse_status, motor, direction, step;
;     139 
;     140 	char buffer[22];
;     141 
;     142 	DDRA = DDRB = DDRC = 0xff;
	SBIW R28,30
	LDI  R24,2
	LDI  R26,LOW(28)
	LDI  R27,HIGH(28)
	LDI  R30,LOW(_0x5D*2)
	LDI  R31,HIGH(_0x5D*2)
	CALL __INITLOCB
;	n -> R16,R17
;	m -> R18,R19
;	dx -> R20,R21
;	dy -> Y+28
;	x -> Y+27
;	y -> Y+26
;	mouse_status -> Y+25
;	motor -> Y+24
;	direction -> Y+23
;	step -> Y+22
;	buffer -> Y+0
	LDI  R16,0
	LDI  R17,0
	LDI  R18,0
	LDI  R19,0
	LDI  R20,0
	LDI  R21,0
	LDI  R30,LOW(255)
	OUT  0x14,R30
	OUT  0x17,R30
	OUT  0x1A,R30
;     143 	memset(&arrMotor, 0, sizeof(arrMotor));
	LDI  R30,LOW(_arrMotor)
	LDI  R31,HIGH(_arrMotor)
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(0)
	ST   -Y,R30
	LDI  R30,LOW(24)
	LDI  R31,HIGH(24)
	ST   -Y,R31
	ST   -Y,R30
	CALL _memset
;     144 
;     145 #ifndef DEBUG
;     146 	// 1us = 1.085
;     147 	OCR1AH = 3333>>8;
	LDI  R30,LOW(13)
	OUT  0x2B,R30
;     148 	OCR1AL = (unsigned char)3333;
	LDI  R30,LOW(5)
	OUT  0x2A,R30
;     149 #else
;     150 	OCR1AH = 33>>8;
;     151 	OCR1AL = (unsigned char)33;
;     152 #endif
;     153 
;     154 	//TCNT1 = 0;
;     155 	//TIMSK = 1<<OCIE1A;
;     156 	//TCCR1B = 1<<CS11;
;     157 
;     158 #ifdef DEBUG
;     159 	arrMotor[0].step = 1111;
;     160 	arrMotor[0].direction = 1;
;     161 #endif
;     162 
;     163     mouseInit();
	CALL _mouseInit
;     164     videoInit();
	CALL _videoInit
;     165 
;     166 	#asm sei;
;     167 	#endasm
;     168     while(1) {
_0x5E:
;     169         if(++n>1000) {
	MOVW R30,R16
	ADIW R30,1
	MOVW R16,R30
	CPI  R30,LOW(0x3E9)
	LDI  R26,HIGH(0x3E9)
	CPC  R31,R26
	BRGE PC+3
	JMP _0x61
;     170             n = 0;
	__GETWRN 16,17,0
;     171             timer1_compa_isr();
	CALL _timer1_compa_isr
;     172 
;     173             if(++m>10) {
	MOVW R30,R18
	ADIW R30,1
	MOVW R18,R30
	SBIW R30,11
	BRGE PC+3
	JMP _0x62
;     174                 m = 0;
	__GETWRN 18,19,0
;     175 
;     176                 mouse_send(0xeb);                  //Request data from mouse
	LDI  R30,LOW(235)
	ST   -Y,R30
	CALL _mouse_send
;     177                 mouse_read();                      //Read the acknowledge byte
	CALL _mouse_read
;     178                 mouse_status = mouse_read();       //Read mouse status
	CALL _mouse_read
	STD  Y+25,R30
;     179                 dx = mouse_read();                 //Read mouse horizontal movement
	CALL _mouse_read
	MOV  R20,R30
	CLR  R21
;     180                 dy = mouse_read();                 //Read mouse vertical movement
	CALL _mouse_read
	LDI  R31,0
	STD  Y+28,R30
	STD  Y+28+1,R31
;     181 
;     182                 if(dx && (mouse_status & 0b00010000)) dx = -(256-dx);
	MOV  R0,R20
	OR   R0,R21
	BREQ _0x64
	LDD  R30,Y+25
	ANDI R30,LOW(0x10)
	BRNE _0x65
_0x64:
	RJMP _0x63
_0x65:
	LDI  R30,LOW(256)
	LDI  R31,HIGH(256)
	SUB  R30,R20
	SBC  R31,R21
	CALL __ANEGW1
	MOVW R20,R30
;     183                 if(dy && (mouse_status & 0b00100000)) dy = -(256-dy);
_0x63:
	LDD  R30,Y+28
	LDD  R31,Y+28+1
	SBIW R30,0
	BREQ _0x67
	LDD  R30,Y+25
	ANDI R30,LOW(0x20)
	BRNE _0x68
_0x67:
	RJMP _0x66
_0x68:
	LDD  R26,Y+28
	LDD  R27,Y+28+1
	LDI  R30,LOW(256)
	LDI  R31,HIGH(256)
	SUB  R30,R26
	SBC  R31,R27
	CALL __ANEGW1
	STD  Y+28,R30
	STD  Y+28+1,R31
;     184 
;     185                 sprintf(buffer, "Mouse dx = %d    ", dx); videoPutStrXY(3, 0, buffer);
_0x66:
	MOVW R30,R28
	ST   -Y,R31
	ST   -Y,R30
	__POINTW1FN _0,0
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R20
	CALL __CWD1
	CALL __PUTPARD1
	LDI  R24,4
	CALL _sprintf
	ADIW R28,8
	LDI  R30,LOW(3)
	ST   -Y,R30
	LDI  R30,LOW(0)
	ST   -Y,R30
	MOVW R30,R28
	ADIW R30,2
	ST   -Y,R31
	ST   -Y,R30
	CALL _videoPutStrXY
;     186                 sprintf(buffer, "Mouse dy = %d    ", dy); videoPutStrXY(3, 11, buffer);
	MOVW R30,R28
	ST   -Y,R31
	ST   -Y,R30
	__POINTW1FN _0,18
	ST   -Y,R31
	ST   -Y,R30
	LDD  R30,Y+32
	LDD  R31,Y+32+1
	CALL __CWD1
	CALL __PUTPARD1
	LDI  R24,4
	CALL _sprintf
	ADIW R28,8
	LDI  R30,LOW(3)
	ST   -Y,R30
	LDI  R30,LOW(11)
	ST   -Y,R30
	MOVW R30,R28
	ADIW R30,2
	ST   -Y,R31
	ST   -Y,R30
	CALL _videoPutStrXY
;     187 
;     188                 if(dx>=0) x = dx;
	SUBI R20,0
	SBCI R21,0
	BRLT _0x69
	MOVW R30,R20
	RJMP _0x134
;     189                 else x = -dx;
_0x69:
	MOVW R30,R20
	CALL __ANEGW1
_0x134:
	STD  Y+27,R30
;     190                 if(dy>=0) y = dy;
	LDD  R26,Y+28
	LDD  R27,Y+28+1
	SBIW R26,0
	BRLT _0x6B
	LDD  R30,Y+28
	RJMP _0x135
;     191                 else y = -dy;
_0x6B:
	LDD  R30,Y+28
	LDD  R31,Y+28+1
	CALL __ANEGW1
_0x135:
	STD  Y+26,R30
;     192 
;     193                 motor = direction = step = 0;
	LDI  R30,LOW(0)
	STD  Y+22,R30
	STD  Y+23,R30
	STD  Y+24,R30
;     194                 if(x > 77 && y > 77) {
	LDD  R26,Y+27
	CPI  R26,LOW(0x4E)
	BRLO _0x6E
	LDD  R26,Y+26
	CPI  R26,LOW(0x4E)
	BRSH _0x6F
_0x6E:
	RJMP _0x6D
_0x6F:
;     195                     if(dx>0 && dy<0) {
	CLR  R0
	CP   R0,R20
	CPC  R0,R21
	BRGE _0x71
	LDD  R26,Y+28
	LDD  R27,Y+28+1
	SBIW R26,0
	BRLT _0x72
_0x71:
	RJMP _0x70
_0x72:
;     196                         step = 8;
	LDI  R30,LOW(8)
	STD  Y+22,R30
;     197                         motor = 2;
	LDI  R30,LOW(2)
	STD  Y+24,R30
;     198                         direction = 1;
	LDI  R30,LOW(1)
	RJMP _0x136
;     199                     } else if(dx<0 && dy>0) {
_0x70:
	SUBI R20,0
	SBCI R21,0
	BRGE _0x75
	LDD  R26,Y+28
	LDD  R27,Y+28+1
	CALL __CPW02
	BRLT _0x76
_0x75:
	RJMP _0x74
_0x76:
;     200                         motor = 2;
	LDI  R30,LOW(2)
	STD  Y+24,R30
;     201                         direction = 0;
	LDI  R30,LOW(0)
_0x136:
	STD  Y+23,R30
;     202                     }
;     203 
;     204                     if(dx<0 && dy<0) {
_0x74:
	SUBI R20,0
	SBCI R21,0
	BRGE _0x78
	LDD  R26,Y+28
	LDD  R27,Y+28+1
	SBIW R26,0
	BRLT _0x79
_0x78:
	RJMP _0x77
_0x79:
;     205                         step = 8;
	LDI  R30,LOW(8)
	STD  Y+22,R30
;     206                         motor = 3;
	LDI  R30,LOW(3)
	STD  Y+24,R30
;     207                         direction = 1;
	LDI  R30,LOW(1)
	RJMP _0x137
;     208                     } else if(dx>0 && dy>0) {
_0x77:
	CLR  R0
	CP   R0,R20
	CPC  R0,R21
	BRGE _0x7C
	LDD  R26,Y+28
	LDD  R27,Y+28+1
	CALL __CPW02
	BRLT _0x7D
_0x7C:
	RJMP _0x7B
_0x7D:
;     209                         step = 8;
	LDI  R30,LOW(8)
	STD  Y+22,R30
;     210                         motor = 3;
	LDI  R30,LOW(3)
	STD  Y+24,R30
;     211                         direction = 0;
	LDI  R30,LOW(0)
_0x137:
	STD  Y+23,R30
;     212                     }
;     213                 } else {
_0x7B:
	RJMP _0x7E
_0x6D:
;     214                     if(x > 88) {
	LDD  R26,Y+27
	CPI  R26,LOW(0x59)
	BRLO _0x7F
;     215                         step = 8;
	LDI  R30,LOW(8)
	STD  Y+22,R30
;     216                         motor = 0;
	LDI  R30,LOW(0)
	STD  Y+24,R30
;     217                         if(dx<0) direction = 1;
	SUBI R20,0
	SBCI R21,0
	BRGE _0x80
	LDI  R30,LOW(1)
	RJMP _0x138
;     218                         else direction = 0;
_0x80:
	LDI  R30,LOW(0)
_0x138:
	STD  Y+23,R30
;     219                     } else if(y > 88) {
	RJMP _0x82
_0x7F:
	LDD  R26,Y+26
	CPI  R26,LOW(0x59)
	BRLO _0x83
;     220                         step = 8;
	LDI  R30,LOW(8)
	STD  Y+22,R30
;     221                         motor = 1;
	LDI  R30,LOW(1)
	STD  Y+24,R30
;     222                         if(dy < 0) direction = 1;
	LDD  R26,Y+28
	LDD  R27,Y+28+1
	SBIW R26,0
	BRGE _0x84
	RJMP _0x139
;     223                         else direction = 0;
_0x84:
	LDI  R30,LOW(0)
_0x139:
	STD  Y+23,R30
;     224                     }
;     225                 }
_0x83:
_0x82:
_0x7E:
;     226 
;     227                 sprintf(buffer, "Motor %d ", motor); videoPutStrXY(3, 22, buffer);
	MOVW R30,R28
	ST   -Y,R31
	ST   -Y,R30
	__POINTW1FN _0,36
	ST   -Y,R31
	ST   -Y,R30
	LDD  R30,Y+28
	CLR  R31
	CLR  R22
	CLR  R23
	CALL __PUTPARD1
	LDI  R24,4
	CALL _sprintf
	ADIW R28,8
	LDI  R30,LOW(3)
	ST   -Y,R30
	LDI  R30,LOW(22)
	ST   -Y,R30
	MOVW R30,R28
	ADIW R30,2
	ST   -Y,R31
	ST   -Y,R30
	CALL _videoPutStrXY
;     228                 sprintf(buffer, "Step %d ", step); videoPutStrXY(3, 33, buffer);
	MOVW R30,R28
	ST   -Y,R31
	ST   -Y,R30
	__POINTW1FN _0,46
	ST   -Y,R31
	ST   -Y,R30
	LDD  R30,Y+26
	CLR  R31
	CLR  R22
	CLR  R23
	CALL __PUTPARD1
	LDI  R24,4
	CALL _sprintf
	ADIW R28,8
	LDI  R30,LOW(3)
	ST   -Y,R30
	LDI  R30,LOW(33)
	ST   -Y,R30
	MOVW R30,R28
	ADIW R30,2
	ST   -Y,R31
	ST   -Y,R30
	CALL _videoPutStrXY
;     229                 sprintf(buffer, "Direction %d ", direction); videoPutStrXY(3, 44, buffer);
	MOVW R30,R28
	ST   -Y,R31
	ST   -Y,R30
	__POINTW1FN _0,55
	ST   -Y,R31
	ST   -Y,R30
	LDD  R30,Y+27
	CLR  R31
	CLR  R22
	CLR  R23
	CALL __PUTPARD1
	LDI  R24,4
	CALL _sprintf
	ADIW R28,8
	LDI  R30,LOW(3)
	ST   -Y,R30
	LDI  R30,LOW(44)
	ST   -Y,R30
	MOVW R30,R28
	ADIW R30,2
	ST   -Y,R31
	ST   -Y,R30
	CALL _videoPutStrXY
;     230 
;     231                 arrMotor[motor].step = step;
	LDD  R30,Y+24
	LDI  R31,0
	CALL __LSLW2
	SUBI R30,LOW(-_arrMotor)
	SBCI R31,HIGH(-_arrMotor)
	MOVW R26,R30
	LDD  R30,Y+22
	LDI  R31,0
	ST   X+,R30
	ST   X,R31
;     232                 arrMotor[motor].direction = direction;
	LDD  R26,Y+24
	LDI  R30,LOW(4)
	MUL  R30,R26
	MOVW R30,R0
	__ADDW1MN _arrMotor,2
	LDD  R26,Y+23
	STD  Z+0,R26
;     233 
;     234                 //arrMotor[motor].step += (direction == arrMotor[motor].direction) ? step : -step;
;     235                 //if(!arrMotor[motor].step) arrMotor[motor].direction = direction;
;     236             }
;     237         }
_0x62:
;     238 
;     239         /*
;     240         if(PIND & 1<<PD4) {
;     241             if(!arrMotor[i].step) arrMotor[i].step = 1;
;     242             arrMotor[i].direction = 0;
;     243         }
;     244 
;     245         if(PIND & 1<<PD5) {
;     246             delay_ms(255);
;     247 
;     248             if(++i > sizeof(arrMotor)/sizeof(*arrMotor) - 1) i = 0;
;     249 
;     250             while(PIND & 1<<PD5);
;     251         }
;     252 
;     253         if(PIND & 1<<PD6) {
;     254             if(!arrMotor[i].step) arrMotor[i].step = 1;
;     255             arrMotor[i].direction = 1;
;     256         }*/
;     257     }
_0x61:
	RJMP _0x5E
;     258 
;     259     return 0;
_0x86:
	RJMP _0x86
;     260 }

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
_getchar:
     sbis usr,rxc
     rjmp _getchar
     in   r30,udr
	RET
_putchar:
     sbis usr,udre
     rjmp _putchar
     ld   r30,y
     out  udr,r30
	ADIW R28,1
	RET
__put_G2:
	ST   -Y,R17
	ST   -Y,R16
	LDD  R26,Y+4
	LDD  R27,Y+4+1
	CALL __GETW1P
	SBIW R30,0
	BREQ _0x87
	LDD  R26,Y+2
	LDD  R27,Y+2+1
	CALL __GETW1P
	MOVW R16,R30
	SBIW R30,0
	BREQ _0x89
	__CPWRN 16,17,2
	BRLO _0x8A
	MOVW R30,R16
	SBIW R30,1
	MOVW R16,R30
	ST   X+,R30
	ST   X,R31
_0x89:
	LDD  R26,Y+4
	LDD  R27,Y+4+1
	LD   R30,X+
	LD   R31,X+
	ADIW R30,1
	ST   -X,R31
	ST   -X,R30
	SBIW R30,1
	LDD  R26,Y+6
	STD  Z+0,R26
_0x8A:
	RJMP _0x8B
_0x87:
	LDD  R30,Y+6
	ST   -Y,R30
	CALL _putchar
_0x8B:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,7
	RET
__print_G2:
	SBIW R28,6
	CALL __SAVELOCR6
	LDI  R17,0
_0x8C:
	LDD  R30,Y+18
	LDD  R31,Y+18+1
	ADIW R30,1
	STD  Y+18,R30
	STD  Y+18+1,R31
	SBIW R30,1
	LPM  R30,Z
	MOV  R18,R30
	CPI  R30,0
	BRNE PC+3
	JMP _0x8E
	MOV  R30,R17
	CPI  R30,0
	BRNE _0x92
	CPI  R18,37
	BRNE _0x93
	LDI  R17,LOW(1)
	RJMP _0x94
_0x93:
	ST   -Y,R18
	LDD  R30,Y+15
	LDD  R31,Y+15+1
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R28
	ADIW R30,15
	ST   -Y,R31
	ST   -Y,R30
	RCALL __put_G2
_0x94:
	RJMP _0x91
_0x92:
	CPI  R30,LOW(0x1)
	BRNE _0x95
	CPI  R18,37
	BRNE _0x96
	ST   -Y,R18
	LDD  R30,Y+15
	LDD  R31,Y+15+1
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R28
	ADIW R30,15
	ST   -Y,R31
	ST   -Y,R30
	RCALL __put_G2
	RJMP _0x13A
_0x96:
	LDI  R17,LOW(2)
	LDI  R20,LOW(0)
	LDI  R16,LOW(0)
	CPI  R18,45
	BRNE _0x97
	LDI  R16,LOW(1)
	RJMP _0x91
_0x97:
	CPI  R18,43
	BRNE _0x98
	LDI  R20,LOW(43)
	RJMP _0x91
_0x98:
	CPI  R18,32
	BRNE _0x99
	LDI  R20,LOW(32)
	RJMP _0x91
_0x99:
	RJMP _0x9A
_0x95:
	CPI  R30,LOW(0x2)
	BRNE _0x9B
_0x9A:
	LDI  R21,LOW(0)
	LDI  R17,LOW(3)
	CPI  R18,48
	BRNE _0x9C
	ORI  R16,LOW(128)
	RJMP _0x91
_0x9C:
	RJMP _0x9D
_0x9B:
	CPI  R30,LOW(0x3)
	BREQ PC+3
	JMP _0x91
_0x9D:
	CPI  R18,48
	BRLO _0xA0
	CPI  R18,58
	BRLO _0xA1
_0xA0:
	RJMP _0x9F
_0xA1:
	MOV  R26,R21
	LDI  R30,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	MOV  R21,R30
	MOV  R30,R18
	SUBI R30,LOW(48)
	ADD  R21,R30
	RJMP _0x91
_0x9F:
	MOV  R30,R18
	CPI  R30,LOW(0x63)
	BRNE _0xA5
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	SBIW R26,4
	STD  Y+16,R26
	STD  Y+16+1,R27
	ADIW R26,4
	LD   R30,X
	ST   -Y,R30
	LDD  R30,Y+15
	LDD  R31,Y+15+1
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R28
	ADIW R30,15
	ST   -Y,R31
	ST   -Y,R30
	RCALL __put_G2
	RJMP _0xA6
_0xA5:
	CPI  R30,LOW(0x73)
	BRNE _0xA8
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	SBIW R26,4
	STD  Y+16,R26
	STD  Y+16+1,R27
	ADIW R26,4
	CALL __GETW1P
	STD  Y+6,R30
	STD  Y+6+1,R31
	ST   -Y,R31
	ST   -Y,R30
	CALL _strlen
	MOV  R17,R30
	RJMP _0xA9
_0xA8:
	CPI  R30,LOW(0x70)
	BRNE _0xAB
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	SBIW R26,4
	STD  Y+16,R26
	STD  Y+16+1,R27
	ADIW R26,4
	CALL __GETW1P
	STD  Y+6,R30
	STD  Y+6+1,R31
	ST   -Y,R31
	ST   -Y,R30
	CALL _strlenf
	MOV  R17,R30
	ORI  R16,LOW(8)
_0xA9:
	ORI  R16,LOW(2)
	ANDI R16,LOW(127)
	LDI  R19,LOW(0)
	RJMP _0xAC
_0xAB:
	CPI  R30,LOW(0x64)
	BREQ _0xAF
	CPI  R30,LOW(0x69)
	BRNE _0xB0
_0xAF:
	ORI  R16,LOW(4)
	RJMP _0xB1
_0xB0:
	CPI  R30,LOW(0x75)
	BRNE _0xB2
_0xB1:
	LDI  R30,LOW(_tbl10_G2*2)
	LDI  R31,HIGH(_tbl10_G2*2)
	STD  Y+6,R30
	STD  Y+6+1,R31
	LDI  R17,LOW(5)
	RJMP _0xB3
_0xB2:
	CPI  R30,LOW(0x58)
	BRNE _0xB5
	ORI  R16,LOW(8)
	RJMP _0xB6
_0xB5:
	CPI  R30,LOW(0x78)
	BREQ PC+3
	JMP _0xE7
_0xB6:
	LDI  R30,LOW(_tbl16_G2*2)
	LDI  R31,HIGH(_tbl16_G2*2)
	STD  Y+6,R30
	STD  Y+6+1,R31
	LDI  R17,LOW(4)
_0xB3:
	SBRS R16,2
	RJMP _0xB8
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	SBIW R26,4
	STD  Y+16,R26
	STD  Y+16+1,R27
	ADIW R26,4
	CALL __GETW1P
	STD  Y+10,R30
	STD  Y+10+1,R31
	LDD  R26,Y+10
	LDD  R27,Y+10+1
	SBIW R26,0
	BRGE _0xB9
	CALL __ANEGW1
	STD  Y+10,R30
	STD  Y+10+1,R31
	LDI  R20,LOW(45)
_0xB9:
	CPI  R20,0
	BREQ _0xBA
	SUBI R17,-LOW(1)
	RJMP _0xBB
_0xBA:
	ANDI R16,LOW(251)
_0xBB:
	RJMP _0xBC
_0xB8:
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	SBIW R26,4
	STD  Y+16,R26
	STD  Y+16+1,R27
	ADIW R26,4
	CALL __GETW1P
	STD  Y+10,R30
	STD  Y+10+1,R31
_0xBC:
_0xAC:
	SBRC R16,0
	RJMP _0xBD
_0xBE:
	CP   R17,R21
	BRSH _0xC0
	SBRS R16,7
	RJMP _0xC1
	SBRS R16,2
	RJMP _0xC2
	ANDI R16,LOW(251)
	MOV  R18,R20
	SUBI R17,LOW(1)
	RJMP _0xC3
_0xC2:
	LDI  R18,LOW(48)
_0xC3:
	RJMP _0xC4
_0xC1:
	LDI  R18,LOW(32)
_0xC4:
	ST   -Y,R18
	LDD  R30,Y+15
	LDD  R31,Y+15+1
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R28
	ADIW R30,15
	ST   -Y,R31
	ST   -Y,R30
	RCALL __put_G2
	SUBI R21,LOW(1)
	RJMP _0xBE
_0xC0:
_0xBD:
	MOV  R19,R17
	SBRS R16,1
	RJMP _0xC5
_0xC6:
	CPI  R19,0
	BREQ _0xC8
	SBRS R16,3
	RJMP _0xC9
	LDD  R30,Y+6
	LDD  R31,Y+6+1
	ADIW R30,1
	STD  Y+6,R30
	STD  Y+6+1,R31
	SBIW R30,1
	LPM  R30,Z
	RJMP _0x13B
_0xC9:
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	LD   R30,X+
	STD  Y+6,R26
	STD  Y+6+1,R27
_0x13B:
	ST   -Y,R30
	LDD  R30,Y+15
	LDD  R31,Y+15+1
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R28
	ADIW R30,15
	ST   -Y,R31
	ST   -Y,R30
	RCALL __put_G2
	CPI  R21,0
	BREQ _0xCB
	SUBI R21,LOW(1)
_0xCB:
	SUBI R19,LOW(1)
	RJMP _0xC6
_0xC8:
	RJMP _0xCC
_0xC5:
_0xCE:
	LDI  R18,LOW(48)
	LDD  R30,Y+6
	LDD  R31,Y+6+1
	ADIW R30,2
	STD  Y+6,R30
	STD  Y+6+1,R31
	SBIW R30,2
	CALL __GETW1PF
	STD  Y+8,R30
	STD  Y+8+1,R31
_0xD0:
	LDD  R30,Y+8
	LDD  R31,Y+8+1
	LDD  R26,Y+10
	LDD  R27,Y+10+1
	CP   R26,R30
	CPC  R27,R31
	BRLO _0xD2
	SUBI R18,-LOW(1)
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	LDD  R30,Y+10
	LDD  R31,Y+10+1
	SUB  R30,R26
	SBC  R31,R27
	STD  Y+10,R30
	STD  Y+10+1,R31
	RJMP _0xD0
_0xD2:
	CPI  R18,58
	BRLO _0xD3
	SBRS R16,3
	RJMP _0xD4
	SUBI R18,-LOW(7)
	RJMP _0xD5
_0xD4:
	SUBI R18,-LOW(39)
_0xD5:
_0xD3:
	SBRC R16,4
	RJMP _0xD7
	CPI  R18,49
	BRSH _0xD9
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	SBIW R26,1
	BRNE _0xD8
_0xD9:
	RJMP _0x13C
_0xD8:
	CP   R21,R19
	BRLO _0xDD
	SBRS R16,0
	RJMP _0xDE
_0xDD:
	RJMP _0xDC
_0xDE:
	LDI  R18,LOW(32)
	SBRS R16,7
	RJMP _0xDF
	LDI  R18,LOW(48)
_0x13C:
	ORI  R16,LOW(16)
	SBRS R16,2
	RJMP _0xE0
	ANDI R16,LOW(251)
	ST   -Y,R20
	LDD  R30,Y+15
	LDD  R31,Y+15+1
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R28
	ADIW R30,15
	ST   -Y,R31
	ST   -Y,R30
	RCALL __put_G2
	CPI  R21,0
	BREQ _0xE1
	SUBI R21,LOW(1)
_0xE1:
_0xE0:
_0xDF:
_0xD7:
	ST   -Y,R18
	LDD  R30,Y+15
	LDD  R31,Y+15+1
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R28
	ADIW R30,15
	ST   -Y,R31
	ST   -Y,R30
	RCALL __put_G2
	CPI  R21,0
	BREQ _0xE2
	SUBI R21,LOW(1)
_0xE2:
_0xDC:
	SUBI R19,LOW(1)
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	SBIW R26,2
	BRLO _0xCF
	RJMP _0xCE
_0xCF:
_0xCC:
	SBRS R16,0
	RJMP _0xE3
_0xE4:
	CPI  R21,0
	BREQ _0xE6
	SUBI R21,LOW(1)
	LDI  R30,LOW(32)
	ST   -Y,R30
	LDD  R30,Y+15
	LDD  R31,Y+15+1
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R28
	ADIW R30,15
	ST   -Y,R31
	ST   -Y,R30
	RCALL __put_G2
	RJMP _0xE4
_0xE6:
_0xE3:
_0xE7:
_0xA6:
_0x13A:
	LDI  R17,LOW(0)
_0x91:
	RJMP _0x8C
_0x8E:
	CALL __LOADLOCR6
	ADIW R28,20
	RET
_sprintf:
	PUSH R15
	MOV  R15,R24
	SBIW R28,2
	ST   -Y,R17
	ST   -Y,R16
	MOVW R26,R28
	CALL __ADDW2R15
	MOVW R16,R26
	MOVW R26,R28
	ADIW R26,6
	CALL __ADDW2R15
	CALL __GETW1P
	STD  Y+2,R30
	STD  Y+2+1,R31
	MOVW R26,R28
	ADIW R26,4
	CALL __ADDW2R15
	CALL __GETW1P
	ST   -Y,R31
	ST   -Y,R30
	ST   -Y,R17
	ST   -Y,R16
	MOVW R30,R28
	ADIW R30,6
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(0)
	LDI  R31,HIGH(0)
	ST   -Y,R31
	ST   -Y,R30
	RCALL __print_G2
	LDD  R26,Y+2
	LDD  R27,Y+2+1
	LDI  R30,LOW(0)
	ST   X,R30
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,4
	POP  R15
	RET
    ld   r30,y+
    ld   r31,y+
    sbiw r30,0
    brpl __abs0
    com  r30
    com  r31
    adiw r30,1
__abs0:
    ret
    ld   r30,y+
    cpi  r30,0
    brpl __cabs0
    neg  r30
__cabs0:
    ret
    ld   r26,y+
    ld   r30,y+
    cp   r26,r30
    brlt __cmax0
    mov  r30,r26
__cmax0:
    ret
    ld   r26,y+
    ld   r30,y+
    cp   r30,r26
    brlt __cmin0
    mov  r30,r26
__cmin0:
    ret
    clr  r30
    ld   r26,y+
    cpi  r26,0
    breq __csign0
    brlt __csignm
    ldi  r30,1
    rjmp __csign0
__csignm:
    ldi  r30,-1
__csign0:
    ret
    ld   r26,y+
    ld   r27,y+
    ld   r30,y+
    ld   r31,y+
    cp   r26,r30
    cpc  r27,r31
    brlt __max0
    movw r30,r26
__max0:
    ret
    ld   r26,y+
    ld   r27,y+
    ld   r30,y+
    ld   r31,y+
    cp   r30,r26
    cpc  r31,r27
    brlt __min0
    movw r30,r26
__min0:
    ret
    ld   r26,y+
    ld   r27,y+
    sbiw r26,0
    breq __sign0
    brlt __signm
    ldi  r30,1
    rjmp __sign0
__signm:
    ldi   r30,-1
__sign0:
    ret
    ld    r30,y+
    ld    r31,y+
    ld    r22,y+
    ld    r23,y+
    sbiw  r30,0
    sbci  r22,0
    sbci  r23,0
    brpl  __labs0
    com   r30
    com   r31
    com   r22
    com   r23
    clr   r0
    adiw  r30,1
    adc   r22,r0
    adc   r23,r0
__labs0:
    ret
    ld    r26,y+
    ld    r27,y+
    ld    r24,y+
    ld    r25,y+
    ld    r30,y+
    ld    r31,y+
    ld    r22,y+
    ld    r23,y+
    cp    r26,r30
    cpc   r27,r31
    cpc   r24,r22
    cpc   r25,r23
    brlt  __lmax0
    movw  r30,r26
    movw  r22,r24
__lmax0:
    ret
    ld    r26,y+
    ld    r27,y+
    ld    r24,y+
    ld    r25,y+
    ld    r30,y+
    ld    r31,y+
    ld    r22,y+
    ld    r23,y+
    cp    r30,r26
    cpc   r31,r27
    cpc   r22,r24
    cpc   r23,r25
    brlt  __lmin0
    movw  r30,r26
    movw  r22,r24
__lmin0:
    ret
    clr  r30
    ld   r26,y+
    ld   r27,y+
    ld   r24,y+
    ld   r25,y+
    sbiw r26,0
    sbci r24,0
    sbci r25,0
    breq __lsign0
    brlt __lsignm
    ldi  r30,1
    rjmp __lsign0
__lsignm:
    ldi  r30,-1
__lsign0:
    ret
    ld   r26,y+
    ld   r27,y+
    clr	 r30
	clr	 r0
	ldi	 r22,0x80
	clt
__isqrt0:
	mov	 r1,r22
	lsr	 r1
	ror	 r0
	or	 r1,r30
	brts __isqrt1
	cp	 r26,r0
	cpc	 r27,r1
	brcs __isqrt2
__isqrt1:
	sub	 r26,r0
	sbc	 r27,r1
	or	 r30,r22
__isqrt2:
	bst	 r27,7
	lsl	 r26
	rol	 r27
	lsr	 r22
	brcc __isqrt0
	ret
    push r20
    push r21
    ld   r26,y+
    ld   r27,y+
    ld   r24,y+
    ld   r25,y+
    clr  r0
    clr  r1
    clr  r20
    clr  r21
    clr  r22
    ldi  r23,0x80
    clr  r30
    clr  r31
    clt
__lsqrt0:
    movw r20,r22
    lsr  r21
    ror  r20
    ror  r1
    ror  r0
    or   r20,r30
    or   r21,r31
    brts __lsqrt1
    cp   r26,r0
    cpc  r27,r1
    cpc  r24,r20
    cpc  r25,r21
    brcs __lsqrt2
__lsqrt1:
    sub  r26,r0
    sbc  r27,r1
    sbc  r24,r20
    sbc  r25,r21
    or   r30,r22
    or   r31,r23
__lsqrt2:
    bst  r25,7
    lsl  r26
    rol  r27
    rol  r24
    rol  r25
    lsr  r23
    ror  r22
    brcc __lsqrt0
    pop  r21
    pop  r20
    ret

_isdigit:
	ldi  r30,1
	ld   r31,y+
	cpi  r31,'0'
	brlo __isdigit0
	cpi  r31,'9'+1
	brlo __isdigit1
__isdigit0:
	clr  r30
__isdigit1:
	ret

_isspace:
	ldi  r30,1
	ld   r31,y+
	cpi  r31,' '
	breq __isspace1
	cpi  r31,9
	brlo __isspace0
	cpi  r31,14
	brlo __isspace1
__isspace0:
	clr  r30
__isspace1:
	ret

_isxdigit:
	ldi  r30,1
	ld   r31,y+
	subi r31,0x30
	brcs __isxdigit0
	cpi  r31,10
	brcs __isxdigit1
	andi r31,0x5f
	subi r31,7
	cpi  r31,10
	brcs __isxdigit0
	cpi  r31,16
	brcs __isxdigit1
__isxdigit0:
	clr  r30
__isxdigit1:
	ret

_memset:
	ldd  r27,y+1
	ld   r26,y
	adiw r26,0
	breq __memset1
	ldd  r31,y+4
	ldd  r30,y+3
	ldd  r22,y+2
__memset0:
	st   z+,r22
	sbiw r26,1
	brne __memset0
__memset1:
	ldd  r30,y+3
	ldd  r31,y+4
	adiw r28,5
	ret

_strlen:
	ld   r26,y+
	ld   r27,y+
	clr  r30
	clr  r31
__strlen0:
	ld   r22,x+
	tst  r22
	breq __strlen1
	adiw r30,1
	rjmp __strlen0
__strlen1:
	ret

_strlenf:
	clr  r26
	clr  r27
	ld   r30,y+
	ld   r31,y+
__strlenf0:
	lpm  r0,z+
	tst  r0
	breq __strlenf1
	adiw r26,1
	rjmp __strlenf0
__strlenf1:
	movw r30,r26
	ret

_delay_ms:
	ld   r30,y+
	ld   r31,y+
	adiw r30,0
	breq __delay_ms1
__delay_ms0:
	__DELAY_USW 0x733
	wdr
	sbiw r30,1
	brne __delay_ms0
__delay_ms1:
	ret

__ADDW2R15:
	CLR  R0
	ADD  R26,R15
	ADC  R27,R0
	RET

__ANEGW1:
	NEG  R31
	NEG  R30
	SBCI R31,0
	RET

__LSLW2:
	LSL  R30
	ROL  R31
	LSL  R30
	ROL  R31
	RET

__CWD1:
	MOV  R22,R31
	ADD  R22,R22
	SBC  R22,R22
	MOV  R23,R22
	RET

__GETW1P:
	LD   R30,X+
	LD   R31,X
	SBIW R26,1
	RET

__GETW1PF:
	LPM  R0,Z+
	LPM  R31,Z
	MOV  R30,R0
	RET

__PUTPARD1:
	ST   -Y,R23
	ST   -Y,R22
	ST   -Y,R31
	ST   -Y,R30
	RET

__CPW02:
	CLR  R0
	CP   R0,R26
	CPC  R0,R27
	RET

__SAVELOCR6:
	ST   -Y,R21
__SAVELOCR5:
	ST   -Y,R20
__SAVELOCR4:
	ST   -Y,R19
__SAVELOCR3:
	ST   -Y,R18
__SAVELOCR2:
	ST   -Y,R17
	ST   -Y,R16
	RET

__LOADLOCR6:
	LDD  R21,Y+5
__LOADLOCR5:
	LDD  R20,Y+4
__LOADLOCR4:
	LDD  R19,Y+3
__LOADLOCR3:
	LDD  R18,Y+2
__LOADLOCR2:
	LDD  R17,Y+1
	LD   R16,Y
	RET

__INITLOCB:
__INITLOCW:
	ADD R26,R28
	ADC R27,R29
__INITLOC0:
	LPM  R0,Z+
	ST   X+,R0
	DEC  R24
	BRNE __INITLOC0
	RET

;END OF CODE MARKER
__END_OF_CODE:
