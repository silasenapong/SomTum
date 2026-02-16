#include <Arduino.h>
#include <DHTZ.hpp>

float t, h;

DHT dht(DHTPIN, DHTTYPE);

void DHTZ::DHTZInit()
{
    dht.begin();
}

float DHTZ::DHTZReadTemp()
{
    t = dht.readTemperature();
    // Serial.print("Temp :");
    // Serial.print(t);

    if (isnan(t))
    {
        Serial.println(F("Failed to read from DHT sensor!"));
        return -1;
    }

    return t;
}

float DHTZ::DHTZReadHumid()
{
    h = dht.readHumidity();
    // Serial.print("Humi :");
    // Serial.print(h);

    if (isnan(h))
    {
        Serial.println(F("Failed to read from DHT sensor!"));
        return -1;
    }

    return h;
}
