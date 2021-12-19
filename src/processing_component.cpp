double getMean(double readings[], )
{
    int noOfReadings = sizeof(readings);
    double sum = 0;
    for (int i = 0; i < noOfReadings; i++)
    {
        sum += readings[i];
    }
    return round((sum / noOfReadings));
}

double getStandardDeviation(double readings[], double mean)
{
    int noOfReadings = sizeof(readings);
    double variance = 0;
    for (int i = 0; i < noOfReadings; i++)
    {
        variance += pow((readings[i] - mean), 2);
    }
    return round(sqrt(variance) / noOfReadings);
}

// Convert the data to CAP based XML string
void convertToXML(char *XMLString, double temperatureMean, double humidityMean, double pressureMean, double lightIntensityMean, double temperatureSD, double humiditySD, double pressureSD, double lightIntensitySD, String id, String timeStamp)
{
    String CAP = "<?xml version = \"1.0\" encoding = \"utf8\"?>" + "<message>" + "<id>%s</id>" + "<sender> EDT IoT</sender>" + "<timeStamp> %s</timeStamp>" + "<mean>" + "<type>%s</type><value>%s</value>" + "<type>%s</type><value>%s</value>" + "<type>%s</type><value>%s</value>" + "<type>%s</type><value>%s</value>" + "</mean>" + "<SD>" + "<type>%s</type><value>%s</value>" + "<type>%s</type><value>%s</value>" + "<type>%s</type><value>%s</value>" + "<type>%s</type><value>%s</value>" + "</SD>" + "</message>";

    sprintf(
        XMLString, CAP, id.c_str(), timeStamp.c_str(), "Temperature", temperatureMean.c_str(), "Humidity", humidityMean.c_str(), "Pressure", pressureMean.c_str(), "LightIntensity", lightIntensityMean.c_str(),
        "Temperature", temperatureSD.c_str(), "Humidity", humiditySD.c_str(), "Pressure", pressureSD.c_str(), "LightIntensity", lightIntensitySD.c_str()

    );
}