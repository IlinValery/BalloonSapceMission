#pragma once

void initSignal(){
    pinMode(LEDS_PIN, OUTPUT);
}

void playSignal(unsigned long& prevStartPlay, const int& signalLevel) {
  float duration = MAX_DURATION_PLAY_SOUND / map(signalLevel, 0, SEQUENCE_COUNT, 10, 1);
  if (millis() - prevStartPlay < duration){
    Serial.println(millis() - prevStartPlay);
    if (millis() - prevStartPlay < duration/2){
      digitalWrite(LEDS_PIN, HIGH);
    } else {
      digitalWrite(LEDS_PIN, LOW);   
    }
  } else {
    prevStartPlay = millis();
    tone(PIEZO_PIN, PIEZO_SIGNAL, duration/2);
  }
}

void stopSignal(){
  digitalWrite(LEDS_PIN, LOW);
}
