/* -----------------------------------
 * Test Interrupts
 * -----------------------------------*/
#define DEBUGLEVEL 2

/* -----------------------------------
 * ISR - Interrupt Service Routine
 * -----------------------------------*/
#define INTERRUPT_PIN D2
volatile uint32_t DiffTime[40];
volatile uint32_t TimeRise;
volatile uint8_t DiffIndex;
volatile bool outLed = true;
volatile uint8_t _pin, _bit, _port;
ICACHE_RAM_ATTR void detectChange(){
   uint32_t ActTime = millis();
   outLed = (*portInputRegister(_port) & _bit) == 0;
   if(outLed){
      if(DiffIndex < 40){
         DiffTime[DiffIndex] = ActTime - TimeRise;
      }
      DiffIndex++;
   }else{
      TimeRise = ActTime;
   }
   #if DEBUGLEVEL > 1
      Serial.print("ISR Index-COunt:");
      Serial.println(DiffIndex);
   #endif
}


/* -----------------------------------
 * Setup
 * -----------------------------------*/
void setup() {
  _pin = INTERRUPT_PIN;
  _bit = digitalPinToBitMask(_pin);
  _port = digitalPinToPort(_pin);
  
  #if DEBUGLEVEL > 0
    Serial.begin(74880);
  #endif
  pinMode(BUILTIN_LED, OUTPUT);
  
  #if DEBUGLEVEL > 0
  Serial.println("--------------------------------");
  Serial.println("Init Interrupt - PIN");
  #endif
  pinMode(_pin, INPUT_PULLUP);
  
  #if DEBUGLEVEL > 0
  Serial.println("Init Interrupt - ISR Change");
  #endif
  attachInterrupt(digitalPinToInterrupt(_pin), detectChange, CHANGE);
  
  #if DEBUGLEVEL > 0
    Serial.println("--------------------------------");
    Serial.println("Init Done");
  #endif
}

void loop() {
   digitalWrite(BUILTIN_LED, outLed);
   if(DiffIndex >= 39){
      #if DEBUGLEVEL > 1
         Serial.println("--------------------------------");
         Serial.println("Data Read done:");
         for(uint8_t i = 0; i < 40; i++){
            Serial.println(DiffTime[i]);
         }
      #endif
      DiffIndex = 0;
   }
}