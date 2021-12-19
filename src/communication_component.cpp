#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include "configuration_component.h"

bool sendDataToServer(char *XMLString)
{
    HTTPClient http;
    if (!isWiFiConnected())
    {
        Serial.println("Please check the WiFi connection");
        return false;
    }

    const char *APIEndPointURL = "https://environmental-data-collector.com/insert-data";
    http.begin(APIEndPointURL); //Specify request destination

    int httpCode = http.POST(XMLString); //Send the request
    if (httpCode != HTTP_CODE_OK)
    {
        http.errorToString(httpCode).c_str();
        http.end();
        return false;
    }

    String payload = http.getString(); //Get the response payload, if successful
    http.end();
    return true;
}