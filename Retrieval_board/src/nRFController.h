#pragma once

int getSignalLevel(const byte * sequence){
  byte countNotZero = 0;
  for (int i = 0; i < SEQUENCE_COUNT; i++){
    if (sequence[i]) {
      countNotZero++;
    }
  }
  return countNotZero;
}

void radioUp(){
  radio.powerUp(); //начать работу
  radio.startListening();  //начинаем слушать эфир, мы приёмный модуль

}

void radioDown(){
    radio.stopListening();
    radio.powerDown();
}

void initRF(byte* sequence){
  
  // fill the sequenceRecieved
  for (int i = 0; i < SEQUENCE_COUNT; i++){
    sequence[i] = 0;
  }

  radio.begin(); //активировать модуль
  radio.setAutoAck(1);         //режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0,15);     //(время между попыткой достучаться, число попыток)
  radio.enableAckPayload();    //разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);     //размер пакета, в байтах

  radio.openReadingPipe(1,address[0]);      //хотим слушать трубу 0
  radio.setChannel(0x60);  //выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_MAX); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_250KBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!
  
  
}




void insertToSequence(const byte& value, byte& currentElement,  byte* sequence){
  sequence[currentElement] = value;
  currentElement = (currentElement + 1) % SEQUENCE_COUNT;
}

void printSequence(const byte* sequence){
  for (int i = 0; i < SEQUENCE_COUNT; i++){
    Serial.print(sequence[i]);
    if (i < SEQUENCE_COUNT-1) {
      Serial.print(" ");
    } else {
      Serial.print("\n");
    }
  }  
}




void updateRF(unsigned long& prevCheck) {
  byte pipeNo, gotByte;                          
  while( radio.available(&pipeNo)){    // слушаем эфир со всех труб
    radio.read( &gotByte, sizeof(gotByte) );         // чиатем входящий сигнал
    insertToSequence(gotByte, currentElement, sequenceRecieved);
    prevCheck = millis();
  }  

  if (millis() - prevCheck > MAX_INTERVAL_BETWEEN_SEQ) {
    insertToSequence(0, currentElement, sequenceRecieved);
    prevCheck = millis();
  }
}