#define DEBUGLEVEL 2

#include "FS.h"

#include "JCA_IOT.h"

using namespace JCA::IOT;
uint32_t StoreMillis;
//std::vector<cRoot*> Elements;

//Json-Dummy String
//StaticJsonDocument<1024> JDoc;

cHandler IotHandler;
void setup() {
  bool RetIO;
  #if DEBUGLEVEL > 0
    Serial.begin(115200);
    Serial.println("add Elements...");
  #endif
  //Add possible Elements
  ELEMENT::beginDI(IotHandler);
  ELEMENT::beginDO(IotHandler);
  
  //Konfig Handler
  RetIO = IotHandler.config("/config.json");

  StoreMillis = millis();
}

void loop() {
  uint32_t ActMillis = millis();
  uint32_t DiffMillis = ActMillis - StoreMillis;
  #if DEBUGLEVEL >= 3
    delay(2000);
  #endif
  StoreMillis = ActMillis;
  IotHandler.update(DiffMillis);
}
