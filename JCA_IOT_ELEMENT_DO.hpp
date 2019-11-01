/**********************************************
 * Class:   JCA_IOT_ELEMENT_DO
 * Info:    Abgeleitete Klasse von JCA_IOT_ELEMENT
 *          Initialisiert einen GPIO und schreibt
 *          diesen.
 * Version:
 *    V1.0.0   Erstellt    01.11.2019  JCA
 *    -add Properties
 *       -- Pin
 *       -- InputValue
 *       -- DataOutput
 *    -add Methoden
 *       -- cDO
 *       -- update
 **********************************************/

#ifndef _JCA_IOT_ELEMENT_DO_H
#define _JCA_IOT_ELEMENT_DO_H

#include "JCA_IOT_ELEMENT.hpp"
#define JCA_IOT_ELEMENT_TYPE_DO 2

//Include extrenal

namespace JCA{ namespace IOT{ namespace ELEMENT{
   class cDO : public cRoot {
    public:
      unsigned char Pin;
      cInputBool  InputValue;
      cDataBool   DataOutput;
      
      cDO(const char* InName, const unsigned char InPin) : cRoot(InName, JCA_IOT_ELEMENT_TYPE_DI) {
         
         // init Input
         InputValue.init("Input");
         // add Input to Vector
         Input.push_back((cInput*)(&InputValue));
         
         // init Data
         DataOutput.init("Value");
         // add Data to Vector
         Data.push_back((cData*)(&DataOutput));
         
         // init DO
         Pin = InPin;
         pinMode(Pin, OUTPUT);
      }
      
      virtual void update(uint32_t DiffMillis) {
         
         //Inputs will be updated by the global Handler
         DataOutput.Value = InputValue.Value;
         digitalWrite(Pin, DataOutput.Value);
      }
      
      
   };
}}}

#endif