/**********************************************
 * Info: 	Die Sammlung enthält alle Klassen des
 *          Element-Vektor Input
 * Version:
 * 	V1.0.0	Erstellt	30.10.2019	JCA
 *		-cInput
 *    -cInputBool
 **********************************************/

#ifndef _JCA_IOT_ELEMENT_INPUT_H
#define _JCA_IOT_ELEMENT_INPUT_H

#include "JCA_IOT_ELEMENT_define.h"

namespace JCA{ namespace IOT{ namespace ELEMENT{
   /**********************************************
   * Class:	JCA_IOT_ELEMENT_cInput
   * Info: 	cInput ist die Root-Klasse aller Inputs
   *          und enthält grundlegende Funktionen
   * Version:
   * 	V1.0.0	Erstellt	30.10.2019	JCA
   **********************************************/
   class cInput{
    public:
      char Name[JCA_IOT_ELEMENT_NAME_LEN];
      unsigned char ElementIndex;
      unsigned char TagIndex;
      unsigned char Type;
      unsigned char QC;
      
      cInput(const char* InName, const unsigned char InType) {
         strncpy(Name, InName, JCA_IOT_ELEMENT_NAME_LEN);
         Type = InType;
         QC = JCA_IOT_QC_CREAT;
      }
      
      void config(const unsigned char Element, const unsigned char Tag) {
         ElementIndex = Element;
         TagIndex = Tag;
         QC = JCA_IOT_QC_GOOD;
      }
      
      bool isGood() {
         return (QC && 0b10000000) != 0b00000000;
      }
   };
   
   /**********************************************
   * Class:	JCA_IOT_ELEMENT_cInputBool
   * Info: 	Bildet einen Digitalen Eingang des Elements ab
   * Version:
   * 	V1.0.0	Erstellt	30.10.2019	JCA
   **********************************************/
   class cInputBool : public cInput{
    public:
      bool Value;
      
//      cInputBool(const char* InName) : cInput(InName, JCA_IOT_ELEMENT_DATA_BOOL) {
      cInputBool() : cInput("", JCA_IOT_ELEMENT_DATA_BOOL) {
         Value = false;
      }
   };
   
   /**********************************************
   * Class:	JCA_IOT_ELEMENT_cInputInt
   * Info: 	Bildet einen Ganzzahl Eingang des Elements ab
   * Version:
   * 	V1.0.0	Erstellt	30.10.2019	JCA
   **********************************************/
   class cInputInt : public cInput{
    public:
      int32_t Value;
      
      cInputInt(const char* InName) : cInput(InName, JCA_IOT_ELEMENT_DATA_INT) {
         Value = 0;
      }
   };
   
   /**********************************************
   * Class:	JCA_IOT_ELEMENT_cInputFloat
   * Info: 	Bildet einen Gleitkommazahl Eingang des Elements ab
   * Version:
   * 	V1.0.0	Erstellt	30.10.2019	JCA
   **********************************************/
   class cInputFloat : public cInput{
    public:
      float Value;
      
      cInputFloat(const char* InName) : cInput(InName, JCA_IOT_ELEMENT_DATA_FLOAT) {
         Value = 0.0;
      }
   };
   
}}}
#endif