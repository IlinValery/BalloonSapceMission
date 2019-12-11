#pragma once


void writeDataToSD(String filename) {

  String data = ""; // new line

  data += convertIntToBeautyString(h, 2);
  data += String(":");
  data += convertIntToBeautyString(m, 2);
  data += String(":");
  data += convertIntToBeautyString(s, 2);
  data += String(".");
  data += convertIntToBeautyString(ms, 4);
  data += String(FILE_SEPARATOR);

  data += String(lat, 6);
  data += String(FILE_SEPARATOR);

  data += String(lon, 6);
  data += String(FILE_SEPARATOR);

  data += String(tempBarometr, 3);
  data += String(FILE_SEPARATOR);

  data += String(pressureBarometr);
  data += String(FILE_SEPARATOR);

  data += String(relativeAltitudeBarometr, 4);
  data += String(FILE_SEPARATOR);

  data += String(tempDS18B20, 4);
  data += String(FILE_SEPARATOR);

  data += String(batteryStatus, 4);

  File dataFile = SD.open(filename, FILE_WRITE);
  
  if (dataFile) {
	dataFile.println(data);
	dataFile.close();

	Serial.println(data);
  }
  else {
	// Сообщаем об ошибке, если все плохо
	Serial.println("error writing to file");
  }
}

void recorderHandler(){
  writeDataToSD(fileName);
}

void initRecorderSDCard() {
  MsTimer2::set(dtime, recorderHandler); // Здесь задаем период 1 секунда
  MsTimer2::start();//Стартуем, теперь таймер запущен
}



String getDateTimeForFile(const RtcDateTime& dt){
  
  String day, hour, min, sec;

  day = convertIntToBeautyString(dt.Day(), 2);
  hour = convertIntToBeautyString(dt.Hour(), 2);
  min = convertIntToBeautyString(dt.Minute(), 2);
  sec = convertIntToBeautyString(dt.Second(), 2);

  return day + hour + min + sec + ".csv";
}

int getFileCount( String dir ){

  File d = SD.open( dir );
  int count_files = 0;

  while( true )  {
	File entry =  d.openNextFile();
	if( !entry ) {
	  return count_files;
	}

	String file_name = entry.name();

	if( file_name.indexOf('~') != 0 ) {
	  count_files++;
	}
  }
}

String initSDCard(const String& headToFile) {
  pinMode(PIN_CHIP_SELECT, OUTPUT);
  if (!SD.begin(PIN_CHIP_SELECT)) {
	Serial.println("Card failed, or not present");
	// TODO message here
	return;
  }
  Serial.println("SD card initialized.");
  
  String fileName = "data.csv";

  // Modify filename
  if (RTCIsAvalable) {
	RtcDateTime now = Rtc.GetDateTime();
	if (!now.IsValid())
	{
		Serial.println("RTC lost confidence in the DateTime!");
		RTCIsAvalable = false;
	}
	fileName = getDateTimeForFile(now);
  } else {
	int filesCount = getFileCount("/");
	fileName = "000" + String(filesCount) + ".CSV";
	fileName = "DATA" + fileName.substring(fileName.length() - (3+4), fileName.length());
  }

  Serial.print("Created new file with name: ");
  Serial.println(fileName);
  

  // Make file with new filename
  File dataFile = SD.open(fileName, FILE_WRITE);
  dataFile.close();
  

  // Insert head to new file
  if (SD.exists(fileName)) {
	File dataFile = SD.open(fileName, FILE_WRITE);
	dataFile.println(headToFile);
	dataFile.close();
	Serial.println(headToFile);
	dataFile.close();
  }
  else {
	// Сообщаем об ошибке, если все плохо
	Serial.println("error opening file");
  }
  return fileName;
}
