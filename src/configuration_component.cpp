#include <ESP8266WiFi.h>

const char *ssid = "environmental-data-collector.com";
const char *password = "password";

void setupWiFi() {
    WiFi.mode(WIFI_OFF);   //Prevents reconnection issue (taking too long to connect)
    delay(1000);
    WiFi.mode(WIFI_STA);    //Hides the viewing of ESP as wifi hotspot
    WiFi.begin(ssid, password);     //Connect to WiFi router
    Serial.println("");
    Serial.print("Connecting...");
    while (WiFi.status() != WL_CONNECTED) {     // Wait for connection
        delay(500);
        Serial.print(".");
    }
    //If connection successful show IP address in serial monitor
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());     //IP address assigned to the ESP
}

bool isWiFiConnected() {
    if (WiFi.status() != WL_CONNECTED) {
        return false;
    }
    return true;
}

bool canReconnect() {
    int count = 0;
    WiFi.mode(WIFI_OFF);   //Prevents reconnection issue (taking too long to connect)
    WiFi.mode(WIFI_STA);    //Hides the viewing of ESP as wifi hotspot
    WiFi.begin(ssid, password);     //Connect to WiFi router
    while ((WiFi.status() != WL_CONNECTED) or (count < 500)) {     // Wait for connection
        Serial.println("Connecting...");
        count++;
    }
    if (count == 500) {
        return false;
    } else {
        Serial.println("Connected");
        return true;
    }
}