/**********************************************
 * Info: 	Die Sammlung enthält alle Klassen des
 *          Element-Vektor Data
 * Version:
 * 	V1.0.0	Erstellt	30.10.2019	JCA
 *		-cData
 *    -cDataBool
 *    -cDataInt
 *    -cDataFloat
 **********************************************/

#ifndef _JCA_IOT_ELEMENT_DATA_H
#define _JCA_IOT_ELEMENT_DATA_H

#include "JCA_IOT_ELEMENT_define.h"

namespace JCA{ namespace IOT{ namespace ELEMENT{
   /**********************************************
   * Class:	JCA_IOT_ELEMENT_cData
   * Info: 	cData ist die Root-Klasse aller Datapunkte
   *          und enthält grundlegende Funktionen
   * Version:
   * 	V1.0.0	Erstellt	30.10.2019	JCA
   **********************************************/
   class cData{
    public:
      char Name[JCA_IOT_ELEMENT_NAME_LEN];
      unsigned char Type;
      unsigned char QC;
      
      cData(const char* InName, const unsigned char InType) const {
         strncpy(Name, InName, JCA_IOT_ELEMENT_NAME_LEN);
         Type = InType;
         QC = JCA_IOT_QC_GOOD;
      }
      
      bool isGood() const {
         return (QC && 0b10000000) != 0b00000000;
      }
   };
   
   /**********************************************
   * Class:	JCA_IOT_ELEMENT_cDataBool
   * Info: 	Bildet einen Digitalen Datenpunkt des Elements ab
   * Version:
   * 	V1.0.0	Erstellt	30.10.2019	JCA
   **********************************************/
   class cDataBool : public cData{
    public:
      bool Value;
      
      cDataBool(const char* InName) : cData(InName, JCA_IOT_ELEMENT_DATA_BOOL) {
         Value = false;
      }
      
      void config(const bool InValue) const {
         Value = InValue;
      }
   };
   
   /**********************************************
   * Class:	JCA_IOT_ELEMENT_cDataInt
   * Info: 	Bildet einen Ganzzahl Datenpunkt des Elements ab
   * Version:
   * 	V1.0.0	Erstellt	30.10.2019	JCA
   **********************************************/
   class cDataInt : public cData{
    public:
      int32_t Value;
      
      cDataInt(const char* InName) : cData(InName, JCA_IOT_ELEMENT_DATA_INT) {
         Value = 0;
      }
      
      void config(const int32_t InValue) const {
         Value = InValue;
      }
   };
   
   /**********************************************
   * Class:	JCA_IOT_ELEMENT_cDataFloat
   * Info: 	Bildet einen Gleitkommazahl Datenpunkt des Elements ab
   * Version:
   * 	V1.0.0	Erstellt	30.10.2019	JCA
   **********************************************/
   class cDataFloat : public cData{
    public:
      float Value;
      
      cDataFloat(const char* InName) : cData(InName, JCA_IOT_ELEMENT_DATA_FLOAT) {
         Value = 0.0;
      }
      
      void config(const float InValue) const {
         Value = InValue;
      }
   };
   
}}}
#endif