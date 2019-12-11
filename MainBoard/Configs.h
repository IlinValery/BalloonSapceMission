#pragma once

//---Status Signals ----
// LEDs are 43 41 39 37, 12g-13r
#define PIEZO_PIN 8
#define PIEZO_SIGNAL 700
#define PIEZO_DURATION 500
#define LED_PIN_RED 13
#define LED_PIN_GREEN 12  

#define LED_STATUS_GPS 43
#define LED_STATUS_DS18B20 41
#define LED_STATUS_BAROMETER 39
#define LED_STATUS_SD 37
//

//RTC
#define DATA_PIN 6
#define CLK_PIN 7
#define RST_PIN 5

//---SD card---
#define dtime 5000 // delay between records
#define PIN_CHIP_SELECT 53
#define FILE_SEPARATOR "\t"
//-------------

#define GPS_SERIAL Serial2


#define RELAY1 23
#define BTN_PIN 9 

#define CRITICAL_DATE "Dec 11 2019"
#define CRITICAL_TIME "16:00:00"
#define TIME_DIFFERENCE_SECONDS 60*2 //4*60*60 // in seconds
