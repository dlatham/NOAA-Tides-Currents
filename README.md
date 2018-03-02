# NOAA Tides and Currents

An ESP8266 library for Arduino IDE to retrieve and parse NOAA tides and currents API data

The library uses the esp8266 wifi client to connect to NOAA's tides and currents API and retreive wind and tide data for a specified station.

### How to use

1. Add this library to your Arduino libraries folder.

2. Include the library into your main sketch

```
#INCLUDE <NOAA.h>
```

3. Declare a new station using the appropriate station ID from NOAA's tides and currents site (https://tidesandcurrents.noaa.gov/). The required parameters are NOAA(int stationID, uint8_t type) where station ID is a 5 or 7 digit unique identifier provided on NOAA's site and the type should currently be set to 1 for a wind (meteorological) station. Future versions will support other station types.

```
NOAA presidio = NOAA(9414290,1);
```

4. Ensure you have a wifi connection set up in order to allow the library to make use of the ESP8266Wifi client for HTTPS GET requests. I like to use the wifi manager library to manage my connection.

```
WiFiManager wifiManager;
  wifiManager.setAPCallback(configModeCallback); // The function to call before the wifi manager config is launched
  wifiManager.autoConnect("Your-Preffered-AP-Name", "Your-Wifi-Password"); 
```


### Function reference

**bool update()**
Create a wifi client and connect the NOAA servers to request current winds and tide data from the station. Returns true for success or false for connection failure.


**float getSpeed(int t)**
Returns wind speed for time __t__ in knots. Currently update() gets wind speed readings at 6 min intervals for the last 7 time periods (t = 0...7).


**float getGust()**
Returns the current wind gust for the station in knots.


**float getDirection()**
Returns the current wind direction in degrees.

