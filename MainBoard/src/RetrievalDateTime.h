#pragma once


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