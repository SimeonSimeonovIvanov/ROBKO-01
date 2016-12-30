/*
        The^day^of^DooM

    Create date:	27.10.2007
    Last Update:	01.11.2007
*/

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
 