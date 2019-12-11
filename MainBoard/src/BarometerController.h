#pragma once

double initBarometr() {
	if (ms5611.begin()){
		Serial.println("Initization BAROMETER complete!");
		return referencePressure = ms5611.readPressure();
	}
}

void updateBarometr(double& tempBarometr, double& pressureBarometr, double& relativeAltitudeBarometr, const double& referencePressure) {
  tempBarometr = ms5611.readTemperature();
  pressureBarometr = ms5611.readPressure();
  relativeAltitudeBarometr = ms5611.getAltitude(pressureBarometr, referencePressure);
}