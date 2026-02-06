#include <Arduino.h>
#include "DHTZ.hpp"
// #include "MQ135.hpp"

#define DelayMs 5000

DHTZ DHTSensor;
// MQ135 MQ;

void setup()
{
    Serial.begin(9600);
    // Serial.println("Date|Time|AirQ|Temp|Humi");

    DHTSensor.DHTZInit();
}

void loop()
{

    Serial.print(DHTSensor.DHTZReadTemp());
    Serial.print(",");
    Serial.println(DHTSensor.DHTZReadHumid());

    delay(DelayMs);
}
