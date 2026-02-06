#include <Arduino.h>
#include "DHTZ.h"

#define DelayMs 1000

DHTZ DHTSensor;

void setup()
{
    Serial.begin(9600);
    Serial.println("Temp|Humi");

    DHTSensor.DHTZInit();
}

void loop()
{

    Serial.print(DHTSensor.DHTZReadTemp());
    Serial.print(",");
    Serial.println(DHTSensor.DHTZReadHumid());

    delay(DelayMs);
}
