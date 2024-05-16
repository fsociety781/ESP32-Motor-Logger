#include <Arduino.h>
const int photoSensorPin = 34; // Pin untuk fotodetektor/fototransistor
volatile unsigned long pulseCount = 0;
unsigned long lastTime = 0;
unsigned long lastPulseCount = 0;

void IRAM_ATTR pulseCounter() {
  pulseCount++;
}

class MotorControl
{
public:
    struct RpmValue 
    {
        double RPM;
    };
    static void setup(void)
    {
        pinMode(photoSensorPin, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(photoSensorPin), pulseCounter, RISING);
    };
    static RpmValue getValue(void)
    {
        unsigned long currentTime = millis();
        
        if (currentTime - lastTime >= 1000) {
          unsigned long elapsedTime = currentTime - lastTime;
          unsigned long deltaPulse = pulseCount - lastPulseCount;
          double rpm = (deltaPulse / 4.0) / (elapsedTime / 60000.0);
          
          RpmValue data;
          data.RPM = rpm;

          lastTime = currentTime;
          lastPulseCount = pulseCount;
         return data; // Tambahkan pernyataan return
        }else {
            RpmValue data;
            data.RPM = 0; // Nilai default jika tidak ada perubahan RPM
            return data;
        };
    };
};

