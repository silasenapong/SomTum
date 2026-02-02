#ifndef DHTZ_H
#define DHTZ_H

#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

class DHTZ
{
public:
    void DHTZInit();
    static float DHTZReadTemp();
    static float DHTZReadHumid();
};

#endif
