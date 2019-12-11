#pragma once

void initRetrievalSensors(){
	pinMode(RELAY1, OUTPUT);
	//pinMode(LED_PIN_GREEN, OUTPUT);
}

void updateRetrievalSensors(const bool& retrievalStatus){
	digitalWrite(RELAY1, !retrievalStatus);
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