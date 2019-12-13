#include "Configs.h"

//--retrievalPlay--
float signalLevel = 5;
unsigned long prevStartPlay = 0;
#include "src/SignalController.h"
//----------------------------

//--serialMega-----
int incomingByte = 0;   // variable to keep bytes
bool retrievalStatus = false;
#include <SoftwareSerial.h>

SoftwareSerial MegaSerial(SERIAL_RX, SERIAL_TX); // RX, TX

#include "src/SerialController.h"
//--------------------------------


//---nRF--------------------------------
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9,10); // "создать" модуль на пинах 9 и 10 Для Уно

byte address[][6] = {"1Node","2Node","3Node","4Node","5Node","6Node"};


byte sequenceRecieved[SEQUENCE_COUNT];
byte currentElement = 0;

unsigned long prevRecieveMoment = 0;

#include "src/nRFController.h"
//----------------------------------------



void setup() {
 initSignal();

 Serial.begin(9600);

 initRF(sequenceRecieved);
 initSerial();

}
void loop() {
  updateSerial(retrievalStatus);


  updateRF(prevRecieveMoment);
  
  
  if (retrievalStatus) {
    radioUp();
    // Serial.println(getSignalLevel(sequenceRecieved));
    if (getSignalLevel(sequenceRecieved) > int(SEQUENCE_COUNT/3)){
      playSignal(prevStartPlay, getSignalLevel(sequenceRecieved));
    } else {
      stopSignal(); 
    }
  } else {
    radioDown();
  }

}
