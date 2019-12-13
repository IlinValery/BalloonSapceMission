#pragma once

//---Status Signals ----
// LEDs are 43 41 39 37, 12g-13r
#define PIEZO_PIN 8
#define PIEZO_SIGNAL 700
#define PIEZO_DURATION 500
#define LED_PIN_RED 13
#define LED_PIN_GREEN 12  

#define LED_STATUS1 43
#define LED_STATUS2 41
#define LED_STATUS3 39
#define LED_STATUS_ORIENTATION 37
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

#define NANO_SERIAL Serial1
#define DEBUG_NANO true

#define GPS_SERIAL Serial2


#define RELAY1 23
#define BTN_PIN 9 

#define CRITICAL_DATE "Dec 14 2019"
#define CRITICAL_TIME "16:30:00"
#define TIME_DIFFERENCE_SECONDS 4*60*60 // in seconds
