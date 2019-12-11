#include "Configs.h" // all defined variables

#include "Utils.h"
/*
String convertIntToBeautyString(int integer, int signCount)
*/

String convertIntToBeautyString(int integer, int signCount){
  String zeros = "";
  for (int i = 0; i < signCount; i++){
	zeros += "0";
  }
  String source = zeros + String(integer);
  return source.substring(source.length() - signCount, source.length());
}


//---Temperature DS18B20----
#include <OneWire.h>
OneWire ds(10);
double tempDS18B20 = 0;
byte addrTempSensor[8];
byte typeTempSensor;
unsigned long timeFromActivation = 0;
int intervalTempSensor = 1000;
bool isTempSensorActive = false;
bool goodStatusTempSensor = false;

#include "TemperatureDS18B20.h"
/*
bool initDS18B20(byte* addressTempDevice, byte& typeOfSensor);
void updateDS18B20(double& temperature, bool& isActive, unsigned long& prevActivation, const int& interval, const byte* addressTempDevice, const byte& typeOfSensor);
*/

//----


//---Barometr---
#include <Wire.h>
#include <SPI.h>
#include <MS5611.h>
MS5611 ms5611;
double referencePressure = 0;
double tempBarometr = 0;
double pressureBarometr = 0;
double relativeAltitudeBarometr = 0;

#include "BarometerController.h"
/*
double initBarometr();
void updateBarometr(double& tempBarometr, double& pressureBarometr, double& relativeAltitudeBarometr, const double& referencePressure);
*/
//-----------------------


//---SD---
#include <MsTimer2.h> //interrupt timer lib
#include <SD.h>

String logStringData = "";
String fileName = "data.csv";
String headToFile = String("h") + String(":") + String("m") + String(":") + String("s") + String(".") + String("ms") + String(FILE_SEPARATOR) +
					String("lat") + String(FILE_SEPARATOR) +
					String("lon") + String(FILE_SEPARATOR) +
					String("tempBarometr") + String(FILE_SEPARATOR) +
					String("pressureBarometr") + String(FILE_SEPARATOR) +
					String("altitudeBarometr") + String(FILE_SEPARATOR) +
					String("tempDS18B20") + String(FILE_SEPARATOR) +
					String("batteryStatus");


//-----------------------


//---Timer---

int h = 0, m = 0, s = 0, ms = 0;
#include "TimeSeparator.h"
/*
void updateTimeSeparator(int& h, int& m, int& s, int& ms);
*/

//-------------

//---RTC---
#include <ThreeWire.h>  
#include <RtcDS1302.h>

ThreeWire myWire(6,7,5); // IO, SCLK, CE (DATA/CLK/RST) 
//TODO it to CONFIGS.h
RtcDS1302<ThreeWire> Rtc(myWire); // start init RTC
bool RTCIsAvalable = false;
//---------

//---GPS---
#include <TinyGPS.h>
//long   lat,lon; // create variable for latitude and longitude object
float lat = 0,lon = 0;
TinyGPS gps; // create gps object

#include "GPSController.h"
/*
	void updateGPS(float& lat, float& lon);
*/
// TODO
void updateGPS(float& lat, float& lon){
	float newLat, newLon;
	while(GPS_SERIAL.available()){
		if( gps.encode( GPS_SERIAL.read() ) ) { 
			gps.f_get_position(&newLat, &newLon);
		}
	}
	lat = newLat;
	lon = newLon;
}

//----------


//---Battery---
float batteryStatus = 0.0;	
#include "BatteryChecker.h"
/*
	void updateBatteryStatus(float& battery);
*/
//-------------


//---RetrievalMode----
#include "CustomDateTime.h"
#include <RtcDateTime.h>
#include <EEPROM.h>

bool retrievalStatus = false;
const int eeAddress = 0;

RtcDateTime savedRtcDateTime;
DateTime criticalDateTime = {
	CRITICAL_DATE,
	CRITICAL_TIME
};
RtcDateTime criticalRtcDateTime = RtcDateTime(criticalDateTime.date, criticalDateTime.time);

#include "RetrievalDateTime.h"
// TODO
RtcDateTime initSavedDateTime(){
	DateTime savedDateTime;
	EEPROM.get(eeAddress, savedDateTime);

	Serial.print("Loaded previous DateTime: ");
	Serial.print(savedDateTime.date);
	Serial.print(" ");
	Serial.println(savedDateTime.time);
	return RtcDateTime(savedDateTime.date, savedDateTime.time);
}

String getStringData(const RtcDateTime& dt){
	String monthes[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	String month = monthes[dt.Month() - 1];
	String day = convertIntToBeautyString(dt.Day(), 2);
	String year = String(dt.Year());

	return month + " " + day + " " + year;
}

String getStringTime(const RtcDateTime& dt){
	String hour = convertIntToBeautyString(dt.Hour(), 2);
	String min = convertIntToBeautyString(dt.Minute(), 2);
	String sec = convertIntToBeautyString(dt.Second(), 2);
	return hour + ":" + min + ":" + sec;
}


DateTime getDateTimeForRecording(const DateTime& critical){
	RtcDateTime now = Rtc.GetDateTime();
	if (!now.IsValid()){
		Serial.println("RTC lost confidence in the DateTime!");
		RTCIsAvalable = false;
		return critical;
	} else {
		DateTime newDateTime = critical;
		String date = getStringData(now);
		String time = getStringTime(now);
		date.toCharArray(newDateTime.date, 12);
		time.toCharArray(newDateTime.time, 9);
		return newDateTime;
	}


}

#include "RetrievalSensors.h"

// TODO

void initRetrievalSensors(){
	pinMode(RELAY1, OUTPUT);
	//pinMode(LED_PIN_GREEN, OUTPUT);
}

void updateRetrievalSensors(const bool& retrievalStatus){
	digitalWrite(RELAY1, retrievalStatus);
}

void updateRetrievalStatus(const RtcDateTime& critical, const RtcDateTime& loaded, bool& retrievalStatus){
	if (!retrievalStatus){
		RtcDateTime now = Rtc.GetDateTime();

		uint32_t secondsDifference = TIME_DIFFERENCE_SECONDS;
		if ((now.TotalSeconds() - loaded.TotalSeconds() > secondsDifference) || (now > critical)){
			Serial.println("Now in retrieval mode");
			retrievalStatus = true;
			MsTimer2::stop();
		}
	}
}


//---------------------

//---Button---
#include <GyverButton.h>
GButton controllerButton(BTN_PIN);
bool buttonWasActivated = false;


#include "RetrievalButton.h"
// TODO

void initSavingButton(){
	pinMode(LED_PIN_RED, OUTPUT);
	pinMode(LED_PIN_GREEN, OUTPUT);
}

void updateSavingButton(bool& buttonStatus){
	controllerButton.tick();

	digitalWrite(LED_PIN_GREEN, buttonStatus);
	digitalWrite(LED_PIN_RED, !buttonStatus);

	// Serial.print(getDateTimeForRecording(criticalDateTime).date);
	// Serial.print(" ");
	// Serial.println(getDateTimeForRecording(criticalDateTime).time);

	if (!buttonStatus){
		if (controllerButton.isTriple()){
			buttonStatus = true;
			tone(PIEZO_PIN, PIEZO_SIGNAL, PIEZO_DURATION);
			EEPROM.put(eeAddress, getDateTimeForRecording(criticalDateTime));
			Serial.println("New data saved to EEPROM");
			savedRtcDateTime = initSavedDateTime();
			retrievalStatus = false;
			initAllSensors();
		}
	}

}
//-----------------


void initAllSensors(){
	savedRtcDateTime = initSavedDateTime(); delay(100);

	RTCIsAvalable = initRTC(); delay(100);

	initSavingButton();

	referencePressure = initBarometr();  delay(100);

	goodStatusTempSensor = initDS18B20(addrTempSensor, typeTempSensor); delay(100);

	fileName = initSDCard(headToFile); delay(100);

	initRecorderSDCard(); delay(100);

	initRetrievalSensors();
}

void updateNormalModeSensors(){
		updateTimeSeparator(h, m, s, ms);

		updateGPS();

		updateBarometr(tempBarometr, pressureBarometr, relativeAltitudeBarometr, referencePressure);

		updateDS18B20(tempDS18B20, isTempSensorActive, timeFromActivation, intervalTempSensor, addrTempSensor, typeTempSensor);

		updateBatteryStatus(batteryStatus);

}

void updateRetrievalModeSensors(){
	updateSavingButton(buttonWasActivated);

	updateRetrievalStatus(criticalRtcDateTime, savedRtcDateTime, retrievalStatus);

	updateRetrievalSensors(retrievalStatus);
}

void setup() {
	Serial.begin(9600);
	
	GPS_SERIAL.begin(9600);

	initAllSensors();

}

void loop() {

	updateRetrievalModeSensors();
	if (!retrievalStatus){
		updateNormalModeSensors();
	}

}

bool initRTC(){
	Rtc.Begin();
	RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
	if (!Rtc.IsDateTimeValid()) {
	// Common Causes:
	//    1) first time you ran and the device wasn't running yet
	//    2) the battery on the device is low or even missing

		Serial.println("RTC lost confidence in the DateTime!");
		Rtc.SetDateTime(compiled);
	}

	if (Rtc.GetIsWriteProtected()){
		Serial.println("RTC was write protected, enabling writing now");
		Rtc.SetIsWriteProtected(false);
	}

	if (!Rtc.GetIsRunning()) {
		Serial.println("RTC was not actively running, starting now");
		Rtc.SetIsRunning(true);
	}

	RtcDateTime now = Rtc.GetDateTime();
	
	if (!now.IsValid()){
		return false;
	}
	return true;
}



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










