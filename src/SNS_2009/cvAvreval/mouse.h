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

#define DATA_IN  PIND.3  //data line from mouse
#define CLK_IN   PIND.2  //clock line from mouse
#define DATA_OUT PORTD.3 //data line from mouse
#define LOW      0
#define HIGH     1            

#define MOUSE_SPEED 2
#define CURSOR_X_MAX 127
#define CURSOR_X_MIN 0
#define CURSOR_Y_MAX 99
#define CURSOR_Y_MIN 0
                                                                          
#asm    
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
  while(CLK_IN!=LOW) { }        // wait for CLK to go low 
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
      while(CLK_IN!=HIGH) {}                    // wait for CLK to go high. mouse read data at rising edge.
      while(CLK_IN!=LOW)  {}                    // wait for CLK to go low. 
      parity=parity ^ DATA_OUT;                 // update parity bit.
      Bit=Bit<<1;                               // Prepare mask
      i++;
    }
  #asm("RELDATA");                              // Release Data line. This is the stop bit. 
  delay_us(50);                                 // give time for transient to settle.
  while(DATA_IN!=LOW) {}                        // wait for Data line to go low               
  while(CLK_IN!=LOW)  {}                        // wait for CLK to go low. Start of ACK from mouse.
  while(DATA_IN==LOW || CLK_IN==LOW) {}         // } of ACK from mouse.               
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
  while(CLK_IN==HIGH)  {} // wait for falling edge. we read data at falling edge of CLK
  i=0;  
  delay_us(5);
  
  while(CLK_IN==LOW)   {} // wait for CLK to go high.
  while(i<=8)
    {
      while(CLK_IN==HIGH) {} // wait for falling edge of CLK   
        t[i]=DATA_IN;        // store each bit
        i++;       
      while(CLK_IN==LOW) {}
    }   
  while(CLK_IN==LOW) {}  
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