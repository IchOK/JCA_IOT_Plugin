/* -----------------------------------
 * Temp Sensoren
 * -----------------------------------*/
#define DEBUGLEVEL 0

/* -----------------------------------
 * Temp Sensoren
 * -----------------------------------*/
#include <DS18B20.h>

#define LOW_ALARM 20  // Minimal Value (Flash Display)
#define HIGH_ALARM 25 // Maximum Value (Flash Display)
#define RESOLUTION 12 // Sensor-Resolution in Bit

DS18B20 ds(D5);
uint8_t addLong[] = {40, 176, 109, 4, 0, 0, 128, 102}; //Wasser
uint8_t addShort[] = {40, 28, 51, 38, 0, 0, 128, 7};   //Land
uint8_t selected;
float TempWater;
float TempLand;

/* -----------------------------------
 * Display
 * -----------------------------------*/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
uint8_t displayIO;
boolean displayInvert = false;
uint8_t displayState = 0;
uint32_t displayLoopTime;

/* -----------------------------------
 * Trend
 *  24h at 128 pixels -> 675 s/px
 * -----------------------------------*/
#define PIXELTIME 675000000 // Time in us
#define TRENDMAX 24
#define TRENDMIN 19
uint8_t TrendDataWater[SCREEN_WIDTH];
uint8_t TrendDataLand[SCREEN_WIDTH];
float ActTrendDataWater = 0.0;
float ActTrendDataLand = 0.0;
uint32_t ActTrendLoop = 0;
float localTrend;
uint32_t TimeTrend;
uint32_t TimeTmp;

void setup() {
  #if DEBUGLEVEL > 0
    Serial.begin(74880);
  #endif
  
  #if DEBUGLEVEL > 0
  Serial.println("--------------------------------");
  Serial.println("Init Sensor - Set Resolution");
  #endif

  if (ds.select(addLong)) {
    ds.setResolution(RESOLUTION);
    #if DEBUGLEVEL > 0
      Serial.println("             - Long Sensor done");
    #endif
  }
  
  //Read short Sensor
  if (ds.select(addShort)) {
    ds.setResolution(RESOLUTION);
    #if DEBUGLEVEL > 0
      Serial.println("             - Short Sensor done");
    #endif
  }

  #if DEBUGLEVEL > 0
    Serial.println("--------------------------------");
    Serial.println("Init Display");
  #endif
  
  displayIO = display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  if(displayIO) {
    #if DEBUGLEVEL > 0
      Serial.println(F("SSD1306 allocation done"));
    #endif
    delay(2000);
    display.clearDisplay();
    display.setTextColor(WHITE);
  }
  #if DEBUGLEVEL > 0
    else{
      Serial.println(F("SSD1306 allocation FAILD"));
    }
  #endif


  #if DEBUGLEVEL > 0
    Serial.println("--------------------------------");
    Serial.println("Init Trending");
  #endif
   TimeTrend = micros();
   displayLoopTime = TimeTrend;
}

void loop() {
  boolean toggle = false;
  #if DEBUGLEVEL > 2
    uint32_t TimeStart;
    uint32_t TimeConvert;
    uint32_t TimeShow;
    TimeStart = micros();
  #endif

  #if DEBUGLEVEL > 1
    Serial.println("--------------------------------");
    Serial.println("Read - Start");
  #endif
  
  //Read long Sensor
  if (ds.select(addLong)) {
    TempWater = ds.getTempC();
    #if DEBUGLEVEL > 1
      Serial.print("Long Sensor : ");
      Serial.print(TempWater);
      Serial.println(" °C");
    #endif
  }
  
  //Read short Sensor
  if (ds.select(addShort)) {
    TempLand = ds.getTempC();
    #if DEBUGLEVEL > 1
      Serial.print("Short Sensor: ");
      Serial.print(TempLand);
      Serial.println(" °C");
    #endif
  }
  #if DEBUGLEVEL > 2
    TimeConvert = micros();
    Serial.print("Read - Ende (");
    Serial.print(TimeConvert - TimeStart);
    Serial.println("us");
  #endif
  
  #if DEBUGLEVEL > 1
    Serial.println("--------------------------------");
    Serial.println("Trending - Start");
  #endif
  
   //Move Trending to next Pixel 
   TimeTmp = micros();
   if ( (TimeTmp - TimeTrend) >= PIXELTIME){
      for (uint8_t i = 0; i < SCREEN_WIDTH-1; i++){
         TrendDataWater[i] = TrendDataWater[i+1];
         TrendDataLand[i] = TrendDataLand[i+1];
      }
      ActTrendDataWater = 0.0;
      ActTrendDataLand = 0.0;
      ActTrendLoop = 0;
      TimeTrend += PIXELTIME;
   }
   
   //Calc Meanvalue
   ActTrendDataWater = ActTrendDataWater + TempWater;
   ActTrendDataLand = ActTrendDataLand + TempLand;
   ActTrendLoop++;
   
   localTrend = (ActTrendDataWater / (float)ActTrendLoop);
   if (localTrend >= TRENDMAX){
      TrendDataWater[SCREEN_WIDTH-1] = SCREEN_HEIGHT;
   }else if(localTrend <= TRENDMIN){
      TrendDataWater[SCREEN_WIDTH-1] = 0;
   }else{
      TrendDataWater[SCREEN_WIDTH-1] = (uint8_t)( (localTrend - TRENDMIN) / (TRENDMAX - TRENDMIN) * SCREEN_HEIGHT);
   }
  #if DEBUGLEVEL > 2
    Serial.print("Water = ");
    Serial.print(localTrend);
    Serial.print(" °C [");
    Serial.print(TrendDataWater[SCREEN_WIDTH-1]);
    Serial.println("px]");
  #endif

   localTrend = (ActTrendDataLand / (float)ActTrendLoop);
   if (localTrend > TRENDMAX){
      TrendDataLand[SCREEN_WIDTH-1] = SCREEN_HEIGHT;
   }else if(localTrend < TRENDMIN){
      TrendDataLand[SCREEN_WIDTH-1] = 0;
   }else{
      TrendDataLand[SCREEN_WIDTH-1] = (uint8_t)( (localTrend - TRENDMIN) / (TRENDMAX - TRENDMIN) * SCREEN_HEIGHT);
   }
  #if DEBUGLEVEL > 2
    Serial.print("Land = ");
    Serial.print(localTrend);
    Serial.print(" °C [");
    Serial.print(TrendDataLand[SCREEN_WIDTH-1]);
    Serial.println("px]");
  #endif
     
  #if DEBUGLEVEL > 2
    TimeConvert = micros();
    Serial.print("Trending - Ende");
  #endif

  #if DEBUGLEVEL > 1
    Serial.println("Display - Start");
  #endif
  display.clearDisplay();
  
  TimeTmp = micros();
  if ( (TimeTmp - displayLoopTime) > 3000000){
     displayLoopTime = TimeTmp;
     displayState++;
     if(displayState > 2){
        displayState = 0;
     }
  }
  
  switch (displayState){
     case 0:
        // display temperature
        display.setTextSize(1);
        display.setCursor(0,0);
        display.print("Wasser: ");
        display.setTextSize(2);
        display.setCursor(0,10);
        display.print(TempWater);
        display.print(" ");
        display.setTextSize(1);
        display.cp437(true);
        display.write(167);
        display.setTextSize(2);
        display.print("C");
        if (TempWater < LOW_ALARM){
          display.print(" -");
          toggle = true;
        }
        if (TempWater > HIGH_ALARM){
          display.print(" +");
          toggle = true;
        }

        display.setTextSize(1);
        display.setCursor(0, 35);
        display.print("Land: ");
        display.setTextSize(2);
        display.setCursor(0, 45);
        display.print(TempLand);
        display.print(" ");
        display.setTextSize(1);
        display.cp437(true);
        display.write(167);
        display.setTextSize(2);
        display.print("C");
        if (TempLand < LOW_ALARM){
          display.print(" -");
          toggle = true;
        }
        if (TempLand > HIGH_ALARM){
          display.print(" +");
          toggle = true;
        }
        
        if (toggle) {
          displayInvert = !displayInvert;    
          display.dim(false);
        }else{
          displayInvert = false;
          display.dim(true);
        }
        display.invertDisplay(displayInvert);
        break;
     
     case 1:
        //display Trend Water
        display.setTextSize(1);
        display.setCursor(2,2);
        display.print(TRENDMAX);
        display.setCursor(2,SCREEN_HEIGHT - 10);
        display.print(TRENDMIN);

        display.setTextSize(2);
        display.setCursor(30,25);
        display.print("Water");

        for (uint8_t i = 1; i < SCREEN_WIDTH; i++){
           display.drawFastVLine(i-1, SCREEN_HEIGHT-TrendDataWater[i], TrendDataWater[i], SSD1306_INVERSE);
        }
        break;

     case 2:
        //display Trend Land
        display.setTextSize(1);
        display.setCursor(2,2);
        display.print(TRENDMAX);
        display.setCursor(2,SCREEN_HEIGHT - 10);
        display.print(TRENDMIN);

        display.setTextSize(2);
        display.setCursor(30,25);
        display.print("Land");

        for (uint8_t i = 1; i < SCREEN_WIDTH; i++){
           display.drawFastVLine(i-1, SCREEN_HEIGHT-TrendDataLand[i], TrendDataLand[i], SSD1306_INVERSE);
        }
        break;

     default:
        displayState = 0;
  }
      
  display.display();
  #if DEBUGLEVEL > 2
    TimeShow = micros();
    Serial.print("Display - Ende (");
    Serial.print(TimeShow - TimeConvert);
    Serial.println("us");
  #endif
}