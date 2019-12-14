#include <DS18B20.h>

#define LOW_ALARM 20
#define HIGH_ALARM 25
#define RESOLUTION 12

DS18B20 ds(2);
uint8_t addLong[] = {40, 176, 109, 4, 0, 0, 128, 102};
uint8_t addShort[] = {40, 28, 51, 38, 0, 0, 128, 7};
uint8_t selected;

void setup() {
  Serial.begin(74880);
  Serial.println("--------------------------------");
  Serial.println("Init - Set Resolution");
  if (ds.select(addLong)) {
    ds.setResolution(RESOLUTION);
    Serial.println("- Long Sensor done");
  }
  
  //Read short Sensor
  if (ds.select(addShort)) {
    ds.setResolution(RESOLUTION);
    Serial.println("- Short Sensor done");
  }
}

void loop() {
  Serial.println("--------------------------------");
  Serial.println("Read - Start");
  //Read long Sensor
  if (ds.select(addLong)) {
    Serial.print("Long Sensor : ");
    Serial.print(ds.getTempC());
    Serial.println(" °C");
  }
  
  //Read short Sensor
  if (ds.select(addShort)) {
    Serial.print("Short Sensor: ");
    Serial.print(ds.getTempC());
    Serial.println(" °C");
  }
  Serial.println("Read - Ende");
  delay(2000);
}
