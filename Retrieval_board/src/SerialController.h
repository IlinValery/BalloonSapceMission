void initSerial(){
  MegaSerial.begin(9600);
}

void updateSerial(bool& retrievalStatus){
    if (MegaSerial.available() > 0) {
        retrievalStatus = true;
      } else {
        retrievalStatus = false;
      }
}