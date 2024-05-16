#include <Arduino.h>
#include "AppPref.h"
#include <SPI.h>
#include <SD.h>
#include <RTClib.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

RTC_DS3231 rtc;
DateTime lastDateTime;
String waktu;
char days[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

File dataFile;

void ReadAccelerometerTask(void *pvParameters);
void ReadCurrentTask(void *pvParameters);


void initFile(const char *fileName, const char *header) {
  if (!SD.exists(fileName)) {
    if (dataFile = SD.open(fileName, FILE_WRITE)) {
      dataFile.println(header);
      Serial.println("File created");
      dataFile.close();
    }
  }
}

void initSDCard() {
  if (!SD.begin(5)) {
    Serial.println("Card failed, or not present");
    while (1)
      vTaskDelay(10 / portTICK_PERIOD_MS);
  }

  initFile("/Acelero.csv", "DateTime,AccelerometerX,AccelerometerY,AccelerometerZ");
  initFile("/Current.csv", "DateTime,Current");
}


void setup() {
  Serial.begin(115200);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1)
      vTaskDelay(10 / portTICK_PERIOD_MS);
  }

  if (rtc.lostPower()) {
    // Serial.println("RTC lost power, let's set the time!");
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  Serial.println("RTC OK");

  initSDCard();

  Accelero::setup();
  // Ambient::setup();
  // Current::setup();
  ADS1115_AMP::setup();
  // MotorControl::setup();
  // MotorTemp::setup();
  Serial.println("Setup done");

  xTaskCreatePinnedToCore(
      ReadAccelerometerTask,
      "AccelerometerTask",
      8192,
      NULL,
      1,
      NULL,
      0
  );

  xTaskCreatePinnedToCore(
      ReadCurrentTask,
      "CurrentTask",
      8192,
      NULL,
      1,
      NULL,
      1
  );
}

void ReadAccelerometerTask(void *pvParameters) {
  (void)pvParameters;

  while (1) {
    Accelero::Values acceleroValues = Accelero::getValues();
    DateTime now = rtc.now();
    char time[50];
    sprintf(time, "%d/%d/%d %02d:%02d:%02d ", now.day(), now.month(), now.year(), now.hour(), now.minute(), now.second());
    waktu = time;
     
    dataFile = SD.open("/Acelero.csv", FILE_APPEND);
    if (dataFile) {
      dataFile.print(waktu);
      dataFile.print(",");
       dataFile.print(acceleroValues.x);
      dataFile.print(",");
      dataFile.print(acceleroValues.y);
      dataFile.print(",");
      dataFile.print(acceleroValues.z);
      dataFile.println();
      dataFile.close();
      // lastMillis = millis();
    }

    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

void ReadCurrentTask(void *pvParameters) {
  (void)pvParameters;

  while (1) {
    ADS1115_AMP::ADS1115_AMPValues currentValues = ADS1115_AMP::getValues();
    DateTime now = rtc.now();
    char time[50];
    sprintf(time, "%d/%d/%d %02d:%02d:%02d ", now.day(), now.month(), now.year(), now.hour(), now.minute(), now.second());
    waktu = time;
    
    Serial.print("ARUS: ");
    Serial.println(currentValues.AMP);

    if (now != lastDateTime) {
      dataFile = SD.open("/Current.csv", FILE_APPEND);
      if (dataFile) {
        dataFile.print(waktu);
        dataFile.print(",");
        dataFile.print(currentValues.AMP);
        dataFile.println();
        dataFile.close();
        lastDateTime = now;
      }
    }

    // vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void loop() {

}