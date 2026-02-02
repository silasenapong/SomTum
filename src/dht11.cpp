/*
#include <DHT.h>

#define DHTTYPE DHT11
#define DHTPIN 2

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
    Serial.begin(9600);
    Serial.println("Temp|Humi");

    pinMode(LEDPIN, OUTPUT);
    dht.begin();
}

void loop()
{
    float humi = dht.readHumidity();
    float tempC = dht.readTemperature();

    if (isnan(humi) || isnan(tempC))
    {
        Serial.println("nan,nan");
        return;
    }

    Serial.print(tempC);
    Serial.print("|");
    Serial.println(humi);

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
*/
