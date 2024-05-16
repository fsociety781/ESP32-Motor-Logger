// #ifndef DX_ACC_H
// #define DX_ACC_H
#include <Wire.h>
#include <SPI.h>
#include "Adafruit_LIS3DH.h"
#include <Adafruit_Sensor.h>

Adafruit_LIS3DH lis = Adafruit_LIS3DH();

class Accelero 
{
public:
    struct Values
    {
        float x;
        float y;
        float z;
    };
    static void setup(void)
    {
         if (! lis.begin(0x19)) {   // change this to 0x19 for alternative i2c address
         Serial.println("Couldnt start");
         while (1) yield();
         }
         Serial.println("LIS3DH found!");
         lis.setDataRate(LIS3DH_DATARATE_25_HZ);
         Serial.print("Data rate set to: ");
    }
    static Values getValues(void)
    {
        sensors_event_t event; 
        lis.getEvent(&event);
        Values values;
        values.x = event.acceleration.x;
        values.y = event.acceleration.y;
        values.z = event.acceleration.z;
        return values;
    }
};
// #endif