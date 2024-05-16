#include <ADS1115_lite.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

ADS1115_lite ads1(ADS1115_DEFAULT_ADDRESS); //Initializes wire library, sets private configuration variables to ADS1115 default(2.048V, 128SPS, Differential mode between  AIN0 and AIN1.  The Address parameter is not required if you want default

      //  unsigned long starttime;
      // unsigned long endtime ;
      int16_t convert;

class ADS1115_AMP
{
public:
    struct ADS1115_AMPValues
    {
        float AMP;
        float ADC;
    };

    static void setup(void)
    {
        ads1.setGain(ADS1115_REG_CONFIG_PGA_6_144V);
        ads1.setSampleRate(ADS1115_REG_CONFIG_DR_8SPS);
        ads1.setMux(ADS1115_REG_CONFIG_MUX_SINGLE_0);
    }

    static ADS1115_AMPValues getValues(void)
    {
        ads1.triggerConversion();

        const int numSamples = 10;
        float arusTotal = 0.0;
        float A1;
        float averagedArus;

        for (int i = 0; i < numSamples; ++i) {
            int16_t convert = ads1.getConversion();
            float resolution = 6144.00 / 32752.00;
            A1 = (convert * resolution) / 1000.00;
            float arus = (A1 - 2.553) / 0.1;

            if (arus < 0.05) {
                arus = 0.0;
            }

            arusTotal += arus;
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }

        averagedArus = arusTotal / numSamples;

        ADS1115_AMPValues data;
        data.ADC = A1;
        data.AMP = averagedArus;
        return data;
    }
};