/*
*========================================================================================================
*
* File                : test_18b20.c
* Hardware Environment:	OpenPIC16F877A && 18b20 && 1602 && 5v voltage && 4M crystal oscillator
 && 
* Build Environment   : MPLAB IDE  
* Version             : V8.76
* By                  : Zhou Jie
*
*
*========================================================================================================
*/
#include<pic18.h>
#include<pic18f4520.h>

#define rs RC4
#define rw RC5
#define e  RC6 
#define  uch unsigned char                   
# define DQ RB0                               
# define DQ_DIR TRISB0                         
# define DQ_HIGH() DQ_DIR =1                  
# define DQ_LOW() DQ = 0; DQ_DIR = 0          
 unsigned char  TLV=0 ;                      
 unsigned char  THV=0;                         
 unsigned char TZ=0;                          
 unsigned char TXI=0;                          
 unsigned int wd;                             

unsigned char shi;                           
unsigned char ge;                            
unsigned char shifen;                         
unsigned char baifen;                       
unsigned char qianfen;                     
unsigned char wanfen;                        


void delay(char x,char y) 
{
  char z;
  do{
      z=y;
      do{;}while(--z);
     }while(--x);
 }

void LCD_delay()
 {
   int i;
   for(i=0;i<100;i++);
 } 

void LCE_PORT_init()
 {
   
    TRISC=0X00;               
    TRISD=0X00;                
 }

void lcd_enable()
 {
   rs=0;                   
   rw=0;                    
   e=0;                     
   LCD_delay();                 
   e=1;                     
 }

void lcd_init()
 {
    PORTD=0X1;               
    lcd_enable();
    PORTD=0X38;               
    lcd_enable();
    PORTD=0X0e;                
    lcd_enable();
    PORTD=0X06;                
    lcd_enable();
    PORTD=0X80;                
    lcd_enable();
 }

void write(char x)
 {
  PORTD=x;                 
  rs=1;                     
  rw=0;                     
  e=0;                    
  LCD_delay();             
  e=1;                      
 } 

void display()
{unsigned char i;
     PORTD=0X80; 
     lcd_enable();
     write(shi+0x30);
     write(ge+0x30);
     write('.');
     write(shifen+0x30);
     write(baifen+0x30);
     write(qianfen+0x30);
     write(wanfen+0x30);
	 for(i=0;i<25;i++)write(' ');
		PORTD=0XC0;            
        lcd_enable();         
 	for(i=0;i<16;i++)write(' ');

}

void init()
{
  ADCON1=0X0f;                                
  TRISB=0X00;                                
  TRISD=0X00;                                
}


reset(void)
{
  char presence=1;
  while(presence)
  { 
    DQ_LOW() ;                               
    delay(2,70);                             
    DQ_HIGH();                           
    delay(2,8);                               
    if(DQ==1) presence=1;                   
    else presence=0;                          
    delay(2,60);                             
   }
  }


void write_byte(uch val)
{
 uch i;
 uch temp;
 for(i=8;i>0;i--)
 {
   temp=val&0x01;                           
   DQ_LOW(); 
   NOP();
   NOP();
   NOP();
   NOP();
   NOP();                                    
   if(temp==1)  DQ_HIGH();                 
   delay(2,7);                             
   DQ_HIGH(); 
   NOP();
   NOP();
   val=val>>1;                            
  }
}

//------------------------------------------------

uch read_byte(void)
{
 uch i;
 uch value=0;                                
 static bit j;
 for(i=8;i>0;i--)
 {
   value>>=1; 
   DQ_LOW();
   NOP();
   NOP();
   NOP();
   NOP();
   NOP();
   NOP();                                   
   DQ_HIGH();                              
   NOP(); 
   NOP();
   NOP();  
   NOP(); 
   NOP();                                
   j=DQ;
   if(j) value|=0x80;
   delay(2,7);                           
  }
  return(value);
}


void get_temp()
{ 
int i;
DQ_HIGH();
reset();                                  
write_byte(0XCC);                         
write_byte(0X44);                         
for(i=20;i>0;i--)
    {
        
        display();                    
    }
reset();                               
write_byte(0XCC);                    
write_byte(0XBE);                  
TLV=read_byte();                
THV=read_byte();                    
DQ_HIGH();                              
TZ=(TLV>>4)|(THV<<4)&0X3f;            
TXI=TLV<<4;                               
if(TZ>100) TZ/100;            
ge=TZ%10;
shi=TZ/10;
wd=0;  
if (TXI & 0x80) wd=wd+5000;
if (TXI & 0x40) wd=wd+2500;
if (TXI & 0x20) wd=wd+1250;
if (TXI & 0x10) wd=wd+625;                           
shifen=wd/1000;                                              
baifen=(wd%1000)/100;                  
qianfen=(wd%100)/10;                   
wanfen=wd%10;                          
NOP();
}

void main()
{    PORTD=0X1;                 
    lcd_enable();
   init();                            
   LCE_PORT_init(); 
   lcd_init();
   while(1)
     {
       get_temp();                 
       display();                     
     }
}
   
