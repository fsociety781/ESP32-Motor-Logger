#include <Arduino.h>
#include "ACS712.h"

ACS712 sensor(ACS712_20A, 25);

class Current
{
public:
    struct CurrentValue 
    {
        float current;
    };
    static void setup(void)
    {
        sensor.calibrate();
    };
    static CurrentValue getValue(void)
    {
        CurrentValue data;
        data.current = sensor.getCurrentDC() / 5.875;//5.875 - 2000 atau 2703
        return data;
    };
};