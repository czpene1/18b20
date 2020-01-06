/*
 * File:   main.c
 * Author: sglee
 *
 * Created on 2018-9-12, PM 2:11
 */

#include <xc.h>

#include <stdio.h>
#include <stdlib.h>
#include "lcd.h"

#include "main.h"
#include "i2c.h"
#include "pcf8563.h"

char *yeardate;
char *curtime;

void main(void) 
{
    TRISD = 0x00;  // Configure PORTD as Output.
    TRISC = 0x00;  // Configure PORTC as Output.
    PORTC = 0x00;
    PORTD = 0x00;
    
  
    Lcd_Init();
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("Display test");
    __delay_ms(2000);
    
    init_i2c(_XTAL_FREQ, 100000);   // Initialize I2C Master with 100KHz clock
    init_pcf8563();
    
    PCF8563_setDate(2020, 01, 01);
    PCF8563_setTime(23, 20, 55);
    Lcd_Clear();
    
    while(1)
    {
        yeardate = PCF8563_getDate();
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String(yeardate);
        curtime = PCF8563_getTime();
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String(curtime);
        __delay_ms(999);
    }
    
    return;
}
