#pragma once

bool initDS18B20(byte* addressTempDevice, byte& typeOfSensor) {
	if ( !ds.search(addressTempDevice)) {
		return false;
	}

	if (OneWire::crc8(addressTempDevice, 7) != addressTempDevice[7]) {
		return false;
	}

	switch (addressTempDevice[0]) {

		case 0x10: // DS18S20
			typeOfSensor = 1;
			break;

		case 0x28: // DS18B20
			typeOfSensor = 0;
			break;

		case 0x22: // DS1822
			typeOfSensor = 0;
			break;

		default: // Not a sensor
			return false;

	}

	{ // success init:
		Serial.println("Initization DS18B20 complete!");
		return true;
	}
}


void updateDS18B20(double& temperature, bool& isActive, unsigned long& prevActivation, const int& interval, const byte* addressTempDevice, const byte& typeOfSensor) {
	if (isActive == false){

		ds.reset();
		ds.select(addressTempDevice);
		ds.write(0x44); // power for collect data about temp

		isActive = true;
		prevActivation = millis();
	} else {
		if (millis() - prevActivation > interval) {
			
			byte present = ds.reset();
			ds.select(addressTempDevice);
			ds.write(0xBE);

			byte data[12];
			for (byte i = 0; i < 9; i++) { // нам необходимо 9 байт
				data[i] = ds.read();
			}

			int16_t raw = (data[1] << 8) | data[0];

			if (typeOfSensor) {
				raw = raw << 3; // разрешение 9 бит по умолчанию
				
				if (data[7] == 0x10) {
					raw = (raw & 0xFFF0) + 12 - data[6];
				}
			} else {
				byte cfg = (data[4] & 0x60);
				if (cfg == 0x00) raw = raw & ~7;
				else if (cfg == 0x20) raw = raw & ~3;
				else if (cfg == 0x40) raw = raw & ~1;
			}

			temperature = (double)raw / 16.0;
			isActive = false;
		} else {
			return;
		}
	}
}