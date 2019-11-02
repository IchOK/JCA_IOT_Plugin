#include "FS.h"

#include <JCA_IOT_ELEMENT.hpp>
#include <JCA_IOT_ELEMENT_DI.hpp>
#include <JCA_IOT_ELEMENT_DO.hpp>

using namespace JCA::IOT::ELEMENT;
uint32_t StoreMillis;
std::vector<cRoot*> Elements;
//cDI TestElementDi("TestElementDI",1,true);
//cDO TestElementDo("TestElementDO",2);

//Json-Dummy String
char DummyJson[] = "";
StaticJsonDocument<1024> JDoc;

void setup() {
/*  Elements.push_back(new cDI("TestElementDI",D5,true));
  Elements.push_back(new cDO("TestElementDO",D0));

  //Initialisierung DO hardcoded
  Elements[1]->Input[0]->ElementIndex = 0;
  Elements[1]->Input[0]->DataIndex = 0;
*/ 
  //-----------------------
  // Generator
  //-----------------------
  // read File
  Serial.begin(74880);
  Serial.println("Mounting FS...");
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }
  Serial.println("Open File...");
  File ConfigFile = SPIFFS.open("/config.json", "r");
  if(!ConfigFile){
    Serial.println("Failed to config file");
    return;
  }else{
    size_t Size = ConfigFile.size();
    if(Size > 1024){
      Serial.println("Config file size is too large");
      return;
    }else{
      // create Json
      Serial.println("Create JSON...");
      DeserializationError JError = deserializeJson(JDoc, ConfigFile);
//      DeserializationError JError = deserializeJson(JDoc, "{\"Test\":\"Wert\"}");
      if(JError){
        Serial.print("deserialize FAILD: ");
        Serial.println(JError.c_str());
      }else{
        Serial.println(JDoc["Name"].as<char*>());
      }
      JsonObject JConfig = JDoc.as<JsonObject>();
      Serial.println("Create Elements...");
      for(JsonObject JElement : JConfig["elements"].as<JsonArray>()){
        if(strcmp(JElement["type"].as<char*>(), "DI") == 0){
          Serial.print("DI: ");
          if (JElement.containsKey("name") && JElement.containsKey("pin")){
            char InName[JCA_IOT_ELEMENT_NAME_LEN];
            unsigned char InPin;
            bool InPullup = false;
            
            InPin = JElement["pin"].as<unsigned char>();
            strcpy(InName, JElement["name"].as<char*>());
            Serial.print(InName);
            Serial.print(" [");
            Serial.print(InPin);
            Serial.print("] ");
            
            if (JElement.containsKey("pullup")){
              InPullup = JElement["pullup"].as<bool>();
              Serial.print("Pullup:");
              Serial.print(InPullup);
              Serial.print(" ");
            }
            Elements.push_back(new cDI(InName, InPin, InPullup));
            Elements.back()->config(JElement);
            Serial.println("- DONE");
          }else{
            Serial.println("Keys missing - FAILD");
          }
        }
      if(strcmp(JElement["type"].as<char*>(), "DO") == 0){
          Serial.print("DO: ");
          if (JElement.containsKey("name") && JElement.containsKey("pin")){
            char InName[JCA_IOT_ELEMENT_NAME_LEN];
            unsigned char InPin;
            
            InPin = JElement["pin"].as<unsigned char>();
            strcpy(InName, JElement["name"].as<char*>());
            Serial.print(InName);
            Serial.print(" [");
            Serial.print(InPin);
            Serial.print("] ");
            
            Elements.push_back(new cDO(InName, InPin));
            Elements.back()->config(JElement);
            Serial.println("- DONE");
          }else{
            Serial.println("Keys missing - FAILD");
          }
        }
      }
    }
  }
  
  DeserializationError JError = deserializeJson(JDoc, DummyJson);
  JsonArray JArray = JDoc.as<JsonArray>();
  for(JsonObject JConfig : JArray){
    Elements[1]->config(JConfig);
  }
  StoreMillis = millis();
}

void loop() {
  uint32_t ActMillis = millis();
  uint32_t DiffMillis = ActMillis - StoreMillis;
//  delay(500);
  StoreMillis = ActMillis;
//  Elements[1]->setInputBool(0,!(Elements[1]->getInputBool(0)));
  for (int e = 0; e < Elements.size(); e++){
    //update Inputs
    for (int i = 0; i < Elements[e]->Input.size(); i++){
      switch(Elements[e]->Input[i]->Type){
        case JCA_IOT_ELEMENT_DATA_BOOL:
          Elements[e]->setInputBool(i, Elements[Elements[e]->Input[i]->ElementIndex]->getDataBool(Elements[e]->Input[i]->DataIndex));
          break;
        case JCA_IOT_ELEMENT_DATA_INT:
          Elements[e]->setInputInt(i, Elements[Elements[e]->Input[i]->ElementIndex]->getDataInt(Elements[e]->Input[i]->DataIndex));
          break;
        case JCA_IOT_ELEMENT_DATA_FLOAT:
          Elements[e]->setInputFloat(i, Elements[Elements[e]->Input[i]->ElementIndex]->getDataFloat(Elements[e]->Input[i]->DataIndex));
          break;
      }
    }
    Elements[e]->update(DiffMillis);
  }
}
