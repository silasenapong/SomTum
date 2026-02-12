// DHT11 or DHT22 temp humid
// MQ135 Air quality
// GP2Y1010AU0F PM2.5

#include <Arduino.h>
#include "DHTZ.hpp"
#include "MQ135.hpp"

#define DelayMs 1000

DHTZ DHTSensor;
MQ135 MQ(A0);

void setup()
{
    Serial.begin(9600);
    // Serial.println("Date|Time|Temp|Humi|AirQ|PM2.5");

    DHTSensor.DHTZInit();
}

void loop()
{
    float temp = DHTSensor.DHTZReadTemp();
    float humi = DHTSensor.DHTZReadHumid();
    float ppm = MQ.getCorrectedPPM(temp, humi);

    Serial.print(ppm);
    Serial.print(",");
    Serial.print(temp);
    Serial.print(",");
    Serial.println(humi);

    delay(DelayMs);
}
