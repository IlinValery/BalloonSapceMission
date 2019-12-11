#define LED_PIN_RED 2
#define LED_PIN_GREEN 3  

#define PIEZO_PIN 4
#define PIEZO_SIGNAL 1000

#define BRIDGE_PIN 12

float signalLevel = 5;
int durationOfSequence = 2000;

void setup() {
 pinMode(LED_PIN_GREEN, OUTPUT);
 pinMode(LED_PIN_RED, OUTPUT);
 Serial.begin(57600);
}
void loop() {

  // check signal level here

  
  if (signalLevel > 4) {
    if (digitalRead(BRIDGE_PIN)) {
      playSignal(durationOfSequence);
      Serial.println(digitalRead(BRIDGE_PIN));
    } else {
      stopSignal(); 
    }
      
  } else {
    
  }
}

void playSignal(const int& durationOfSequence) {
  long currentTime = millis();
  tone(PIEZO_PIN, PIEZO_SIGNAL, durationOfSequence/2);
  while (millis() - currentTime < durationOfSequence){
    if (millis() - currentTime < durationOfSequence/2){
      digitalWrite(LED_PIN_RED, HIGH);
      digitalWrite(LED_PIN_GREEN, LOW);
    } else {
      digitalWrite(LED_PIN_RED, LOW);
      digitalWrite(LED_PIN_GREEN, HIGH);      
    }
  }
}

void stopSignal(){
  digitalWrite(LED_PIN_RED, LOW);
  digitalWrite(LED_PIN_GREEN, LOW);
}
