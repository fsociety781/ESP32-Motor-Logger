#include <Wire.h>
#include <Adafruit_MLX90614.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

class MotorTemp
{
public:
    struct MTemp
    {
        float ambTemp;
        float ambHum;
    };

    static void setup(void)
    {
       while (!Serial);
       Serial.println("Adafruit MLX90614 test");

       if (!mlx.begin()) {
            Serial.println("Error connecting to MLX sensor. Check wiring.");
            while (1);
       };
      Serial.print("Emissivity = "); Serial.println(mlx.readEmissivity());
      Serial.println("================================================");
    }

    static MTemp getValues(void)
    {
       MTemp data;
       data.ambTemp = mlx.readAmbientTempC();
       data.ambHum = mlx.readAmbientTempF();
       return data;
    }
};