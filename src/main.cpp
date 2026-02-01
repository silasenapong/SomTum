#include <Arduino.h>
#include <DHT.h>

#define DHTTYPE DHT11

#define DHTPIN 2
#define LEDPIN 8

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
    Serial.begin(9600);
    dht.begin();
}

void loop()
{
    float humi = dht.readHumidity();
    float tempC = dht.readTemperature();

    if (isnan(humi) || isnan(tempC))
    {
        Serial.println("Error reading data!");
        return;
    }

    Serial.print("Temp : ");
    Serial.print(tempC);
    Serial.print(" °C\n");

    Serial.print("Humi : ");
    Serial.print(humi);
    Serial.print(" %\n");

    if (tempC > 30)
    {
        digitalWrite(LEDPIN, HIGH);
    }
    else
    {
        digitalWrite(LEDPIN, LOW);
    }

    delay(2000);
}
