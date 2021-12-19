#include <Wire.h>
#include <Adafruit_BMP085.h>
#include "DHT.h"

#define DHTTYPE DHT11
#define DHTPIN 3
#define LDRPIN A0
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;

void initializeSensors()
{
    if (bmp.begin())
    {
        Serial.println("BMP180 successfully initialized");
    }
    else
    {
        Serial.println("BMP180 initialization failed");
        while (1)
            ; // Pause forever.
    }
}

float getPressure()
{
    float pressure = bmp.readPressure();
    if (isnan(pressure))
    {
        Serial.println("Pressure reading is not a number");
        return 0;
    }
    return pressure;
}

float getTemperature()
{
    float temperature = dht.readTemperature();
    if (isnan(temperature))
    {
        Serial.println("Temperature reading is not a number");
        return 0;
    }
    return temperature;
}

float getHumidity()
{
    float humidity = dht.readHumidity();
    if (isnan(humidity))
    {
        Serial.println("Humidity reading is not a number");
        return 0;
    }
    return humidity;
}

float getLightIntensity()
{
    // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V)
    float lightIntensity = analogRead(LDRPIN) * (5.0 / 1023.0);
    return lightIntensity;
}