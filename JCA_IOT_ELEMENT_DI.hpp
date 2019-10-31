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

#ifndef _JCA_IOT_ELEMENT_DI_H
#define _JCA_IOT_ELEMENT_DI_H

#include "JCA_IOT_ELEMENT.hpp"
#define JCA_IOT_ELEMENT_TYPE_DI 1

//Include extrenal

namespace JCA{ namespace IOT{ namespace ELEMENT{
   class cDI : public cRoot {
    public:
      
      ////------------------------------
      //// Template Child-Class 
      ////------------------------------
      //// 
      //// create Input
      //cInputBool In1("in1");
      //cInput In2("in2");
      //// 
      //// create Datapoint
      //cData Data1("data1");
      //cData Data2("data2");
      ////
      ////------------------------------
      
      cDI(const char* InName) : cRoot(InName, JCA_IOT_ELEMENT_TYPE_DI) {
         
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
      
      virtual void update(uint32_t DiffMillis){ ; };
      
   };
}}}

#endif