/**********************************************
 * Class:   JCA_IOT_HANDLER
 * Info:    Die Klasse stellt den Handler für
 *          alle IOT-Elemente dar und bietet die
 *          die Schnittstelle zum Mesh-Netzwerk
 * Version:
 *    V1.0.0   Erstellt    02.11.2019  JCA
 *    -add Properties
 *       -- Elements Vector
 *       -- CreateElement Map
 *       -- JDoc StaticJsonDocument
 *       -- Name, ErrorTest, QC, ConfigFile
 *    -add Methoden
 *       -- cHandler
 *       -- addElement
 *       -- config
 *       -- update
 **********************************************/

#ifndef _JCA_IOT_HANDLER_H
#define _JCA_IOT_HANDLER_H

//Include extrenal
#include <map>
#include <vector>
#include <ArduinoJson.h>
#include <FS.h>
#include "JCA_IOT_ELEMENT.hpp"

namespace JCA{ namespace IOT{
   
   class cHandler{
    public:
      std::map<String, std::function<void(JsonObject, std::vector<ELEMENT::cRoot*>&)> > CreateElement;
      std::vector<ELEMENT::cRoot*> Elements;
      char Name[JCA_IOT_ELEMENT_NAME_LEN];
      String ErrorText;
      String ConfigFileName;
      StaticJsonDocument<JCA_IOT_HANDLER_JSON_DOCSIZE> JDoc;
      unsigned char QC;
      
      cHandler(){
         QC = JCA_IOT_QC_CREAT;
      }
      
      void update(uint32_t DiffMillis) {
         #if DEBUGLEVEL >= 3
            Serial.println(F("START - cHandler.update()"));
            Serial.printf("  DiffMillis:%i\r\n",DiffMillis);
         #endif
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
            //update Data
            Elements[e]->update(DiffMillis);
         }
         #if DEBUGLEVEL >= 3
            Serial.println(F("DONE - cHandler.update()"));
         #endif
      }
      
      bool config(char* InConfigFile){
         #if DEBUGLEVEL >= 2
            Serial.println(F("START - cHandler.config()"));
         #endif
         ConfigFileName = InConfigFile;
         //------------------------
         // INIT
         //------------------------
         
         // Init Filesystem
         if (!SPIFFS.begin()) {
            QC = JCA_IOT_QC_CONFCREAT;
            ErrorText = F("Failed to config file");
            #if DEBUGLEVEL >= 1
               Serial.println(ErrorText);
            #endif
            return false;
         }
         // Open Configfile
         File ConfigFile = SPIFFS.open(ConfigFileName, "r");
         if(!ConfigFile){
            QC = JCA_IOT_QC_CONFCREAT;
            ErrorText = F("Failed to config file");
            #if DEBUGLEVEL >= 1
               Serial.println(ErrorText);
            #endif
            return false;
         }
         // Check Filesize
         size_t Size = ConfigFile.size();
         if(Size > JCA_IOT_HANDLER_FILE_MAXSIZE){
            QC = JCA_IOT_QC_CONFCREAT;
            ErrorText = F("Config file size is too large");
            #if DEBUGLEVEL >= 1
               Serial.println(ErrorText);
            #endif
            return false;
         }
         // create Json
         DeserializationError JError = deserializeJson(JDoc, ConfigFile);
         if(JError){
            QC = JCA_IOT_QC_CONFCREAT;
            ErrorText = F("deserialize FAILD: ");
            ErrorText += JError.c_str();
            #if DEBUGLEVEL >= 1
               Serial.println(ErrorText);
            #endif
            return false;
         }
         
         //------------------------
         // CONFIG
         //------------------------
         
         // global Settings
         #if DEBUGLEVEL >= 2
            Serial.println(F("  START - global Settings"));
         #endif
         JsonObject JConfig = JDoc.as<JsonObject>();
         if (JConfig.containsKey("name")){
            strncpy(Name, JConfig["name"], JCA_IOT_ELEMENT_NAME_LEN);
         }else{
            itoa(ESP.getChipId(),Name, JCA_IOT_ELEMENT_NAME_LEN);
         }
         /* TODO
         
         TTTTT  OOO  DDD    OOO 
           T   O   O D  D  O   O
           T   O   O D   D O   O
           T   O   O D   D O   O
           T   O   O D  D  O   O
           T    OOO  DDD    OOO 
         
         check for:
            - i2c
            - spi
            - onewire
            
         */
         #if DEBUGLEVEL >= 2
            Serial.print(F("    Node Name:"));
            Serial.println(Name);
            Serial.println(F("  DONE - global Settings"));
         #endif

         // create Elements
         #if DEBUGLEVEL >= 2
            Serial.println(F("  START - create Elements"));
         #endif
         for(JsonObject JElement : JConfig["elements"].as<JsonArray>()){
            #if DEBUGLEVEL >= 2
               if (JElement.containsKey("name")){
                  Serial.print(F("    Element Name:"));
                  Serial.println(JElement["name"].as<char*>());
               }
            #endif
            if (JElement.containsKey("type")){
               #if DEBUGLEVEL >= 2
                  Serial.print(F("    Element Type:"));
                  Serial.println(JElement["type"].as<char*>());
               #endif
               if(CreateElement.find(JElement["type"].as<String>()) == CreateElement.end() ){
                  QC = JCA_IOT_QC_CONFCREAT;
                  ErrorText = F("Unable to create Type ");
                  ErrorText += JElement["type"].as<String>();
                  #if DEBUGLEVEL >= 1
                     Serial.println(ErrorText);
                  #endif
               }else{
                  CreateElement[JElement["type"].as<String>()](JElement, Elements);
               }
            }
         }
         #if DEBUGLEVEL >= 2
            Serial.println(F("DONE - cHandler.config()"));
         #endif
         return true;
      }
   };
}}

#endif