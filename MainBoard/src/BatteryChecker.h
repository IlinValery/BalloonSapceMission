#pragma once

#define BATTERY_PIN A8
#define BATTERY_KOEF 102.3

void updateBatteryStatus(float& battery){
	battery = analogRead(BATTERY_PIN)/BATTERY_KOEF;
}