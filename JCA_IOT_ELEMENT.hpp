/**********************************************
 * Class:   JCA_IOT_ELEMENT
 * Info:    Die Klasse ist der Ursprung für alle JCA_IOT_IO
 *          Elemente und enthält die definition der
 *          Datenstrukturen sowie Grundfunktionen für Ein-
 *          und Ausaben, sowie die Schrnittstelle zur
 *          Kommunikation.
 * Version:
 *    V1.0.0   Erstellt   23.04.2019   JCA
 *      -tData
 *    -tInterface
 *    -tMesh
 *    -addParam
 *    -addValue
 *    -addMesh
 **********************************************/

#ifndef _JCA_IOT_ELEMENT_H
#define _JCA_IOT_ELEMENT_H

#include "JCA_IOT_ELEMENT_define.h"
#include "JCA_IOT_ELEMENT_Input.hpp"
#include "JCA_IOT_ELEMENT_Data.hpp"

//Include extrenal
#include <vector>
#include <ArduinoJson.h>

namespace JCA{ namespace IOT{ namespace ELEMENT{
   class cRoot {
    public:
      std::vector<cInput*> Input;
      std::vector<cData*> Data;
      char Name[JCA_IOT_ELEMENT_NAME_LEN];
      unsigned char Type;
      
      ////------------------------------
      //// Template Child-Class 
      ////------------------------------
      //// 
      //// create Input
      // cInput In1("in1");
      // cInput In2("in2");
      //// 
      //// create Datapoint
      // cData Data1("data1");
      // cData Data2("data2");
      ////
      ////------------------------------
      
      cRoot(const char* InName, const unsigned char InType){
         strncpy(Name, InName, JCA_IOT_ELEMENT_NAME_LEN);
         Type = InType;
         
         ////------------------------------
         //// Template Child-Class
         ////------------------------------
         //// 
         //// add Input to Vector
         // Input.push_back(in1);
         // Input.push_back(in2);
         //// add Data to Vector
         // Data.push_back(data1);
         // Data.push_back(data2);
         ////
         ////------------------------------
         
      }
      
      void config(JsonObject& jsonObj);
      virtual void update(uint32_t DiffMillis);
      
    private:
      bool addInputBool(const char* InName){
         cInputBool Dummy(InName);
         strncpy(Name, InName, JCA_IOT_ELEMENT_NAME_LEN);
      }
   };
}}}
