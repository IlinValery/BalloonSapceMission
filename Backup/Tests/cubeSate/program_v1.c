#include <Wire.h>
#include <SPI.h>

#define dtime 1000 //период записи данных на карту
//---DS18b20----
#include <OneWire.h>
OneWire ds(8); // Объект OneWire
int temperature = 0; // Глобальная переменная для хранения значение температуры с датчика DS18B20
long lastUpdateTime = 0; // Переменная для хранения времени последнего считывания с датчика
const int TEMP_UPDATE_TIME = 1000; // Определяем периодичность проверок
//----

//---DHT11---
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float h_dht11, t_dht11;
//------

//---Barometr---
#include <MS5611.h>
MS5611 ms5611;
double referencePressure;
double TempBarometr;
long Pressure;
float absoluteAltitude, relativeAltitude;
//-----------------------

//---Axel_gyro MPU6050
#include "I2Cdev.h"
#include "MPU6050.h"
MPU6050 accelgyro;
int16_t ax, ay, az;
int16_t gx, gy, gz;
#define OUTPUT_READABLE_ACCELGYRO
bool blinkState = false;
//-----------------------


//---SD---
#include <SD.h>
const int PIN_CHIP_SELECT = 53;
// Строка с данными, которые мы поместим в файл:
String logStringData = "";
//-----------------------


//---Timer---
unsigned long start, finished, elapsed;
#include <MsTimer2.h>//Библиотека прерываний по таймеру2
long totalSek = 0; //Здесь мы будем хранить секунды с момента запуска программы
//Этого счетчика хватит на 68 лет :)  (это просто размышления...68 лет нам не нужно)
float h, m, s, ms;
//-------------


String fileName = "data.csv";
String headToFile = "h\tm\ts\tTempBarometr\tPressure\tt_dht11\th_dht11\ttemp_other";
void setup() {
  Serial.begin(9600);
  fileName = init_sd(headToFile); delay(100);
  init_barometr();  delay(100);
  init_alexgyro();  delay(100);
  init_timer(); delay(100);
  dht.begin(); delay(500);

  work_timer();
  work_barometr();
  //write_data_to_card();
}

void loop() {
  work_timer();
  work_barometr();
  work_dht11();
  detectTemperature(); // Определяем температуру от датчика DS18b20
  //  work_axelgyro();
}

void work_dht11() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  h_dht11 = dht.readHumidity();
  // Read temperature as Celsius (the default)
  t_dht11 = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h_dht11) || isnan(t_dht11)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
}



void to_Timer()//Функция Таймер, он считает и пишет на экран время которое прошло
{
  write_data_to_card(fileName);
}

void init_timer() {
  MsTimer2::set(dtime, to_Timer); // Здесь задаем период 1 секунда
  MsTimer2::start();//Стартуем, теперь таймер запущен
}

void work_timer() {
  finished = millis(); // сохраняет время остановки, чтобы вычислить прошедшее время.
  // объявляем переменные

  unsigned long over;

  // время
  elapsed = finished;

  h = int(elapsed / 3600000);
  over = elapsed % 3600000;
  m = int(over / 60000);
  over = over % 60000;
  s = int(over / 1000);
  ms = over % 1000;
}

void write_data_to_card(String filename) {
  logStringData = "";

  logStringData += String(h, 0);
  logStringData += String("\t");
  logStringData += String(m, 0);
  logStringData += String("\t");
  logStringData += String(s, 0);

  //  logStringData += String(referencePressure);
  logStringData += String("\t");
  logStringData += String(TempBarometr);
  logStringData += String("\t");
  logStringData += String(Pressure);
  logStringData += String("\t");

  //---DHT11----
  logStringData += String(t_dht11);
  logStringData += String("\t");
  logStringData += String(h_dht11);
  logStringData += String("\t");

  //---DS18b20---
  logStringData += String(temperature);
  logStringData += String("\t");


  //  logStringData += String(ax);
  //  logStringData += String("  ");
  //  logStringData += String(ay);
  //  logStringData += String("  ");
  //  logStringData += String(az);
  //  logStringData += String("  ");
  //
  //  logStringData += String(gx);
  //  logStringData += String("  ");
  //  logStringData += String(gy);
  //  logStringData += String("  ");
  //  logStringData += String(gz);
  //  logStringData += String("  ");

  //Serial.println(logStringData);
  File dataFile = SD.open(filename, FILE_WRITE);
  // Если все хорошо, то записываем строку:
  if (dataFile) {
    dataFile.println(logStringData);
    dataFile.close();
    // Публикуем в мониторе порта для отладки
    Serial.println(logStringData);
  }
  else {
    // Сообщаем об ошибке, если все плохо
    Serial.println("error opening file");
  }
}

int getFileCount( String dir )
{
  File d = SD.open( dir );
  int count_files = 0;
  while( true )
  {
    File entry =  d.openNextFile();
    if( !entry )
    {
      // no more files. Let's return the number of files.
      return count_files;
    }
    String file_name = entry.name();  //Get file name so that we can check 
              //if it's a duplicate
    if( file_name.indexOf('~') != 0 )   //Igrnore filenames with a ~. It's a mac thing.
    {         //Just don't have file names that have a ~ in them
      count_files++;
    }
  }
}

String init_sd(String headToFile) {
  pinMode(PIN_CHIP_SELECT, OUTPUT);
  if (!SD.begin(PIN_CHIP_SELECT)) {
    Serial.println("Card failed, or not present");
    // Если что-то пошло не так, завершаем работу:
    return;
  }
  Serial.println("card initialized.");
  

  // Detect how much file on SDcard now

  int filesCount = getFileCount("/");

  String fileName = "000" + String(filesCount) + ".CSV";
  fileName = "DATA" + fileName.substring(fileName.length() - (3+4), fileName.length());
  Serial.println(fileName);
  // Make file with 
  File dataFile = SD.open(fileName, FILE_WRITE);
  dataFile.close();
  
  if (SD.exists(fileName)) {
    File dataFile = SD.open(fileName, FILE_WRITE);
    dataFile.println(headToFile);
    dataFile.close();
    // Публикуем в мониторе порта для отладки
    Serial.println(headToFile);
    dataFile.close();
  }
  else {
    // Сообщаем об ошибке, если все плохо
    Serial.println("error opening file");
  }
  return fileName;
}

void work_axelgyro() {
  // read raw accel/gyro measurements from device
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // these methods (and a few others) are also available
  //accelgyro.getAcceleration(&ax, &ay, &az);
  //accelgyro.getRotation(&gx, &gy, &gz);

#ifdef OUTPUT_READABLE_ACCELGYRO
  // display tab-separated accel/gyro x/y/z values
  Serial.print("a/g:\t");
  Serial.print(ax / 100); Serial.print("\t");
  Serial.print(ay / 100); Serial.print("\t");
  Serial.print(az / 100); Serial.print("\t");
  Serial.print(gx); Serial.print("\t");
  Serial.print(gy); Serial.print("\t");
  Serial.println(gz);
#endif

#ifdef OUTPUT_BINARY_ACCELGYRO
  Serial.write((uint8_t)(ax >> 8)); Serial.write((uint8_t)(ax & 0xFF));
  Serial.write((uint8_t)(ay >> 8)); Serial.write((uint8_t)(ay & 0xFF));
  Serial.write((uint8_t)(az >> 8)); Serial.write((uint8_t)(az & 0xFF));
  Serial.write((uint8_t)(gx >> 8)); Serial.write((uint8_t)(gx & 0xFF));
  Serial.write((uint8_t)(gy >> 8)); Serial.write((uint8_t)(gy & 0xFF));
  Serial.write((uint8_t)(gz >> 8)); Serial.write((uint8_t)(gz & 0xFF));
#endif
}

void init_alexgyro() {
  // join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif

  // initialize serial communication
  // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
  // it's really up to you depending on your project

  // initialize device
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  // use the code below to change accel/gyro offset values
  /*
    Serial.println("Updating internal sensor offsets...");
    // -76  -2359 1688  0 0 0
    Serial.print(accelgyro.getXAccelOffset()); Serial.print("\t"); // -76
    Serial.print(accelgyro.getYAccelOffset()); Serial.print("\t"); // -2359
    Serial.print(accelgyro.getZAccelOffset()); Serial.print("\t"); // 1688
    Serial.print(accelgyro.getXGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getYGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getZGyroOffset()); Serial.print("\t"); // 0
    Serial.print("\n");
    accelgyro.setXGyroOffset(220);
    accelgyro.setYGyroOffset(76);
    accelgyro.setZGyroOffset(-85);
    Serial.print(accelgyro.getXAccelOffset()); Serial.print("\t"); // -76
    Serial.print(accelgyro.getYAccelOffset()); Serial.print("\t"); // -2359
    Serial.print(accelgyro.getZAccelOffset()); Serial.print("\t"); // 1688
    Serial.print(accelgyro.getXGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getYGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getZGyroOffset()); Serial.print("\t"); // 0
    Serial.print("\n");
  */
}

void work_barometr() {
  TempBarometr = ms5611.readTemperature();
  Pressure = ms5611.readPressure();
  // Calculate altitude
  absoluteAltitude = ms5611.getAltitude(Pressure);
  relativeAltitude = ms5611.getAltitude(Pressure, referencePressure);

  //  Serial.print("TempBar = ");
  //  Serial.print(TempBarometr);
  //  Serial.print(" *C ");
  //
  //  Serial.print("Pressure = ");
  //  Serial.print(Pressure);
  //  Serial.print(" Pa  ");
  //
  //  Serial.print(" absoluteAltitude = ");
  //  Serial.print(absoluteAltitude);
  //  Serial.print(" m, relativeAltitude = ");
  //  Serial.print(relativeAltitude);
  //  Serial.println(" m");

}

void init_barometr() {
  // Initialize MS5611 sensor
  Serial.println("Initialize MS5611 Sensor");
  while (!ms5611.begin())
  {
    Serial.println("Could not find a valid MS5611 sensor, check wiring!");
    delay(500);
  }
  // Get reference pressure for relative altitude
  referencePressure = ms5611.readPressure();
  // Check settings
  Serial.print("Oversampling: ");
  Serial.println(ms5611.getOversampling());
}

int detectTemperature() {
  byte data[2];
  ds.reset();
  ds.write(0xCC);
  ds.write(0x44);
  if (millis() - lastUpdateTime > TEMP_UPDATE_TIME)
  {
    lastUpdateTime = millis();
    ds.reset();
    ds.write(0xCC);
    ds.write(0xBE);
    data[0] = ds.read();
    data[1] = ds.read();
    // Формируем значение
    temperature = (data[1] << 8) + data[0]; temperature = temperature >> 4;
  }
}