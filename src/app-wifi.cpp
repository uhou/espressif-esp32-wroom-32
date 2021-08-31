#include <WiFi.h>

const char* ssid = "Xiaomi_37E4";
const char* password =  "12345678@abcd";


void connectToNetwork() {

    WiFi.disconnect(true, true);
	WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
		delay(2000);
		Serial.println("Establishing connection to WiFi..");
	}
	Serial.println("Connected to network.");    
}