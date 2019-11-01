/**********************************************
 * Class:   JCA_IOT_ELEMENT_DI
 * Info:    Abgeleitete Klasse von JCA_IOT_ELEMENT
 *          Initialisiert einen GPIO und liest
 *          diesen ein.
 * Version:
 *    V1.0.0   Erstellt    01.11.2019  JCA
 *    -add Properties
 *       -- Pin
 *       -- Pullup
 *       -- DataInput
 *    -add Methoden
 *       -- cDI
 *       -- update
 **********************************************/

#ifndef _JCA_IOT_ELEMENT_DI_H
#define _JCA_IOT_ELEMENT_DI_H

#include "JCA_IOT_ELEMENT.hpp"
#define JCA_IOT_ELEMENT_TYPE_DI 1

//Include extrenal

namespace JCA{ namespace IOT{ namespace ELEMENT{
   class cDI : public cRoot {
    public:
      unsigned char Pin;
      bool Pullup;
      cDataBool   DataInput;
      
      cDI(const char* InName, const unsigned char InPin, const bool InPullup) : cRoot(InName, JCA_IOT_ELEMENT_TYPE_DI) {
         
         // init Data
         DataInput.init("Value");
         // add Data to Vector
         Data.push_back((cData*)(&DataInput));
         
         // init DI
         Pin = InPin;
         Pullup = InPullup;
         if (Pullup){
            pinMode(Pin, INPUT_PULLUP);
         } else {
            pinMode(Pin, INPUT);
         }
      }
      
      virtual void update(uint32_t DiffMillis) {
         
         //Inputs will be updated by the global Handler
         DataInput.Value = digitalRead(Pin);
      }
      
      
   };
}}}

#endif