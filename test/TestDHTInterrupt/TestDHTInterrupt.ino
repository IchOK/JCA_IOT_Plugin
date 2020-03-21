#include <DHT_Interrupt.h>

DHT22 sensor;  // change to DHT11 if needed!
volatile float humidity = 0;
volatile float temperature = 0;
volatile uint8_t error = 0;
volatile int8_t result = 0;
volatile uint32_t LastTime;

// this callback will be called from an interrupt
// it should be short and carry the ICACHE_RAM_ATTR attribute
void ICACHE_RAM_ATTR handleData(float h, float t) {
  humidity = h;
  temperature = t;
  result = 1;
}

// this callback will be called from an interrupt
// it should be short and carry the ICACHE_RAM_ATTR attribute
void ICACHE_RAM_ATTR handleError(uint8_t e) {
  error = e;
  result = -1;
}

void setup() {
  Serial.begin(74880);
  sensor.setup(D2);
  sensor.onData(handleData);
  sensor.onError(handleError);
}

void loop() {
  if (result > 0) {
    Serial.printf("Humid: %g%%\n", humidity);
    Serial.printf("Temp: %gdegC\n", temperature);
  } else if (result < 0) {
    Serial.printf("Error: %s\n", sensor.getError());
  }
  result = 0;
  sensor.read();
}