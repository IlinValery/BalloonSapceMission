#pragma once

void updateTimeSeparator(int& h, int& m, int& s, int& ms) {
	unsigned long finished = millis();
	unsigned long elapsed = finished;
	unsigned long over;
	h = int(elapsed / 3600000);
	over = elapsed % 3600000;
	m = int(over / 60000);
	over = over % 60000;
	s = int(over / 1000);
	ms = over % 1000;
}