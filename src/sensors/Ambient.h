#include "Adafruit_SHTC3.h"
#include <Wire.h>

Adafruit_SHTC3 shtc3 = Adafruit_SHTC3();

class Ambient
{
public:
    struct AmbientValues
    {
        float temperature;
        float humidity;
    };
    static void setup(void)
    {
         while (!Serial)
           delay(10);

        Serial.println("SHTC3 test");
        if (! shtc3.begin()) {
            Serial.println("Couldn't find SHTC3");
            while (1) delay(1);
        }
        Serial.println("Found SHTC3 sensor");
    }

    static AmbientValues getValues(void)
    {   
        sensors_event_t humidity, temp;
        shtc3.getEvent(&humidity, &temp);
        AmbientValues data;
        data.temperature = temp.temperature;
        data.humidity = humidity.relative_humidity;
        return data;
    }
};
