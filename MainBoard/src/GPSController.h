#pragma once

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