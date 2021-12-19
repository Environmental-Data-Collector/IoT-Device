#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <queue>

#include "ssensor_reading_component.h"
#include "processing_component.h"
#include "communication_component.h"
#include "configuration_component.h"

#include "time.h"
using namespace std;

String timeStamp;
int code = 0;
char XMLString[2500];
String id_;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

double tempurateMean = 0;
double humidityMean = 0;
double pressureMean = 0;
double lightIntensityMean = 0;
double tempurateSD = 0;
double humiditySD = 0;
double pressureSD = 0;
double lightIntensitySD = 0;

// Buffers
queue<String> ids;
queue<String> timeStamps;
queue<int> codes;
queue<double> tempurateMeanBuffer;
queue<double> humidityMeanBuffer;
queue<double> pressureMeanBuffer;
queue<double> lightIntensityMeanBuffer;
queue<double> tempurateSDBuffer;
queue<double> humiditySDBuffer;
queue<double> pressureSDBuffer;
queue<double> lightIntensitySDBuffer;

// Executes after the values are sent to the server
void removeFirstElement()
{
  codes.pop();
  ids.pop();
  timeStamps.pop();
  tempurateMeanBuffe.pop();
  humidityMeanBuffer.pop();
  pressureMean.pop();
  lightIntensityMean.pop();
  tempurateSDBuffer.pop();
  humiditySDBuffer.pop();
  pressureSDBuffer.pop();
  lightIntensitySDBuffer.pop();
}

// Executes when there is a connection failure
void addAsLastElement()
{
  codes.push(code);
  ids.push(id_);
  timeStamps.push(timeStamp);
  tempurateMeanBuffer.push(tempurateMean);
  humidityMeanBuffer.push(humidityMean);
  pressureMeanBuffer.push(pressureMean);
  lightIntensityMeanBuffer.push(lightIntensityMean);
  tempurateSDBuffer.push(tempurateSD);
  humiditySDBuffer.push(humiditySD);
  pressureSDBuffer.push(pressureSD);
  lightIntensitySDBuffer.push(lightIntensitySD);
}

void getTime(char *timestamp)
{
  struct tm info;
  char arr[32];
  time_t temp = time(NULL);
  sprintf(timestamp, strftime(arr, sizeof(arr), "%Y-%m-%dT%H:%M:%S%z", localtime(&temp)));
}

void setup()
{
  Serial.begin(115200);
  setupWiFi();
  initializeSensors();
  configTime(19800, 0, "pool.ntp.org"); //Get Time from NTP service using NODEMCU 1.0 (ESP- 12E)
}

void loop()
{
  while (!codes.empty())
  {
    if (isWiFiConnected())
    {
      char CAP[2500];
      convertToXML(CAP, tempurateMeanBuffer.front(), humidityMeanBuffer.front(), pressureMeanBuffer.front(), lightIntensityMeanBuffer.front(), tempurateSDBuffer.front(), humiditySDBuffer.front(), pressureSDBuffer.front(), lightIntensitySDBuffer.front(), (String)ids.front(), (String)timeStamps.front());
      bool sent =
          if (sendDataToServer(CAP, codes.front()))
      {
        removeFirstElement();
        Serial.println("Successfully sent to the server");
      }
      else
      {
        Serial.println("Sending failed");
        break;
      }
    }
    else
    {
      Serial.println("Unable to connect to WiFi");
      break;
    }
  }

  int iterationCount = 0;
  double currentTemperatureBuffer[noOfIterations];
  double currentHumidityBuffer[noOfIterations];
  double currentPressureBuffer[noOfIterations];
  double currentLightIntensityBuffer[noOfIterations];

  while (iterationCount < 900)
  {
    currentTemperatureBuffer[iterationCount] = round(getTemperature());
    currentHumidityBuffer[iterationCount] = round(getHumidity());
    currentPressureBuffer[iterationCount] = round(getPressure());
    currentLightIntensityBuffer[iterationCount] = round(getLightIntensity());
    delay(1000); // Sensor reading are pushed to the buffer with a 1000ms gap
    iterationCount++;
  }

  tempurateMean = getMean(currentTemperatureBuffer);
  humidityMeanBuffer = getMean(currentHumidityBuffer);
  pressureMean = getMean(currentPressureBuffer);
  lightIntensityMean = getMean(currentLightIntensityBuffer);
  tempurateSD = getStandardDeviation(currentTemperatureBuffer, tempurateMean);
  humiditySD = getStandardDeviation(currentHumidityBuffer, humidityMeanBuffer);
  pressureSD = getStandardDeviation(currentPressureBuffer, pressureMean);
  lightIntensitySD = getStandardDeviation(currentLightIntensityBuffer, lightIntensityMean);

  id_ = String(code);
  timeStamp = String(getTime(timeStamp));
  convertToXML(XMLString, tempurateMean, humidityMeanBuffer, pressureMean, lightIntensityMean, tempurateSD, humiditySD, pressureSD, lightIntensitySD, id_, timeStamp);

  if (!isWiFiConnected())
  {
    if (canReconnect())
    {
      if (!sendDataToServer(XMLString))
      {
        addAsLastElement();
      }
    }
    else
    {
      addAsLastElement();
    }
  }
  else
  {
    if (!sendDataToServer(XMLString) {
      addAsLastElement();
    } else {
      Serial.println("Successfully sent to the server");
    }
  }
  code++;
}