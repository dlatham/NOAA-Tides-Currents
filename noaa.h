/* Tides and currents library for ESP8266 usage 
Station types
0 = wind only (met)
1 = Current only

This library requires a wifi client connection using the ESP8266 wifi client

*/

#include <ArduinoJson.h> //Required library for parsing JSON


#ifndef _NOAA_H
#define _NOAA_H

class NOAA {
	int stationid, type;
	float station[8];
public: 
	NOAA(int, int);
	bool update();
	float getSpeed(int);
	float getGust();
	float getDirection();
};

#endif