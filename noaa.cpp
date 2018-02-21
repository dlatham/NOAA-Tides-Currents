/* NOAA API parser */
#include "NOAA.h"
#include <ESP8266WiFi.h>

NOAA::NOAA(int id, int station_type){
	stationid = id;
	type = station_type;
	int i=0;
	while(i<8,i++){
		station[i] = 0.0;
	}

}

bool NOAA::update(){
	
	WiFiClientSecure client;


	//WiFi.printDiag(Serial); // Debug wifi connection problems
	//if (type==0){ // wind only station
		String url = "/api/datagetter?range=1&product=wind&time_zone=lst&units=english&format=json&station=";
		url += stationid;

	//CHECK THE MEMORY USAGE
	Serial.print("Free memory: ");
	Serial.println(ESP.getFreeHeap());


	Serial.print("Requesting URL: ");
  	Serial.println(url);

  	client.setTimeout(10000);
  	if (!client.connect("tidesandcurrents.noaa.gov", 443)) {
    	Serial.println(F("Connection failed"));
    	return 0;
  	}

  	Serial.println(F("Connected!"));

  
  	// This will send the request to the server
  	client.print("GET " + url + " HTTP/1.1\r\n" +
               "Host: tidesandcurrents.noaa.gov\r\n" + 
               "Connection: close\r\n\r\n");
  	unsigned long timeout = millis();
  	while (client.available() == 0) {
    	if (millis() - timeout > 5000) {
      	Serial.println(">>> Client Timeout !");
      	client.stop();
      	return 0;
    	}
  	}

  	// Check HTTP status
  	char status[32] = {0};
  	client.readBytesUntil('\r', status, sizeof(status));
  	if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
    	Serial.print(F("Unexpected response: "));
    	Serial.println(status);
    	return 0;
  	}

  	// Skip HTTP headers
  	char endOfHeaders[] = "\r\n\r\n";
  	if (!client.find(endOfHeaders)) {
    	Serial.println(F("Invalid response"));
    	return 0;
  	}

  	// Allocate JsonBuffer
  	// Use arduinojson.org/assistant to compute the capacity.
  	const size_t bufferSize = JSON_ARRAY_SIZE(8) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + 8*JSON_OBJECT_SIZE(6) + 580;
	DynamicJsonBuffer jsonBuffer(bufferSize);


  	// Parse JSON object
  	JsonObject& root = jsonBuffer.parseObject(client);
  	if (!root.success()) {
    	Serial.println(F("Parsing failed!"));
    	return 0;
  	}

  	//CHECK THE MEMORY USAGE
  	Serial.print("Free memory: ");
	Serial.println(ESP.getFreeHeap());

  	// Extract values
  	Serial.println(F("Response:"));
  	JsonArray& data = root["data"];

  	//CHECK THE MEMORY USAGE
	Serial.println(ESP.getFreeHeap());

  	JsonObject& data0 = data[0];
	station[0] = data0["s"]; // "11.86"
	Serial.println(station[0]);
	station[6] = data0["d"]; // "271.00"
	Serial.println(station[6]);
	station[7] = data0["g"]; // "14.77"
	Serial.println(station[7]);

	JsonObject& data1 = data[1];
	station[1] = data1["s"]; // "11.86"
	Serial.println(station[1]);


  	// Disconnect
  	client.stop();
  	return true;
	

}


float NOAA::getSpeed(int t){
	//t = the requested timeframe 0 = current, 1 = -6min, 2 = -12min ... 5 = -30min
	return station[t];

}

float NOAA::getGust(){
	return station[7];
}

float NOAA::getDirection(){
	return station[6];
}





