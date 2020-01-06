/* 
 * File:   pcf8563.c
 * Author: sglee
 *
 * Created on 2018-9-13, AM 11:39
 */

#include <xc.h> 

#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "pcf8563.h"
#include "i2c.h"



void setI2C(unsigned char reg, unsigned char data)
{
    I2C_Master_Start();         // Start condition
    
    I2C_Master_Write(RTC_ADDR); // 7 bit address + Write
    I2C_Master_Write(reg);      // Write data
    I2C_Master_Write(data);     // Write data
    
    I2C_Master_Stop();          // Stop condition
}

unsigned char getI2C(unsigned char reg)
{
    unsigned char data;
  
    I2C_Master_Start();                 // Start condition
    
    I2C_Master_Write(RTC_ADDR);         // 7 bit address + Write
    I2C_Master_Write(reg);
    
    I2C_Master_RepeatedStart();         // add, important!!!
    
    I2C_Master_Write(RTC_ADDR | 0x01);  // 7 bit address + Read
    data = I2C_Master_Read(0);          // Read + Acknowledge

    I2C_Master_Stop();                  // Stop condition

    return data;
}

void PCF8563_setDate(unsigned short year, unsigned char mon, unsigned char day)
{
    year = changeIntToHex(year % 100);
    mon  = changeIntToHex(mon) | ((year > 1999) ? 0x00 : 0x80);
    day  = changeIntToHex(day);

    setI2C(REG_YEAR, year);
    setI2C(REG_MON, mon);
    setI2C(REG_DAY, day);
}

char *PCF8563_getDate()
{
    unsigned char buf[3];
    unsigned char century = 0;
    unsigned short year;
    unsigned char mon, day;
    char msg[] = "    -  -  ";

    buf[0] = getI2C(REG_YEAR);
    buf[1] = getI2C(REG_MON);
    buf[2] = getI2C(REG_DAY);

    if (buf[1] & 0x80)
        century = 1;

    buf[0] = buf[0]&0xff;
    buf[1] = buf[1]&0x1f;
    buf[2] = buf[2]&0x3f;

    year = (century == 0) ? (1900 + changeHexToInt(buf[0])) : (2000 + changeHexToInt(buf[0]));
    mon = changeHexToInt(buf[1]);
    day = changeHexToInt(buf[2]);
    
    sprintf(msg, "%d-  -  ", year);
    
    msg[9] = day % 10  + '0';
    msg[8] = day / 10  + '0';
    msg[6] = mon % 10  + '0';
    msg[5] = mon / 10  + '0';
        
    return msg;
}

void PCF8563_setTime(unsigned char hour, unsigned char min, unsigned char sec)
{
    setI2C(REG_HOUR, changeIntToHex(hour));
    setI2C(REG_MIN, changeIntToHex(min));
    setI2C(REG_SEC, changeIntToHex(sec));
}

char *PCF8563_getTime()
{
    unsigned char hour, min, sec;
    char time[] = "  :  :  ";

    hour = changeHexToInt(getI2C(REG_HOUR) & 0x3f);
    min = changeHexToInt(getI2C(REG_MIN) & 0x7f);
    sec = changeHexToInt(getI2C(REG_SEC) & 0x7f);

    time[7] = sec % 10  + '0';
    time[6] = sec / 10  + '0';
    time[4] = min % 10  + '0';
    time[3] = min / 10  + '0';
    time[1] = hour % 10  + '0';
    time[0] = hour / 10  + '0';
    
    //sprintf(time, "%d:%d:%d", hour, min, sec);
    return time;
}

void init_pcf8563(void) 
{ 
    if((getI2C(0x0a)) != 0x08)  
    {  
        setI2C(REG_CTRL_STATUS_1, 0x00);  
        setI2C(REG_CTRL_STATUS_2, 0x00);  
    } 
} 
