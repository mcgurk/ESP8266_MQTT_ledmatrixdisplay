// ePaper Lolin 2.13 inch e-Paper 250x122 WeMos Mini D1
// BUSY D2 (* voi olla että voi säästää odottamalla aina vakioajan)
// RST D4 (* voi olla että voi säästää käyttämällä Wemos:n RST:tä)
// DC D3
// CS D8
// SCK/CLK D5
// MOSI/DIN D7
// GND GND
// 3.3V 3.3V
// ---
// DHT 3.3V, GND, D6
//
// https://github.com/ZinggJM/GxEPD2
// https://github.com/olikraus/U8g2_for_Adafruit_GFX
// https://github.com/olikraus/u8g2/wiki/fntlistall

#define ENABLE_GxEPD2_GFX 0

#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <U8g2_for_Adafruit_GFX.h>

#include "DHTesp.h"
DHTesp dht;

#if defined (ESP8266)
GxEPD2_BW<GxEPD2_213_B72, GxEPD2_213_B72::HEIGHT> display(GxEPD2_213_B72(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4)); // GDEH0213B72
#endif

#if defined(ESP32)
GxEPD2_BW<GxEPD2_213_B72, GxEPD2_213_B72::HEIGHT> display(GxEPD2_213_B72(/*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // GDEH0213B72
#endif

#if defined(__AVR)
#define MAX_DISPAY_BUFFER_SIZE 800 // 
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= MAX_DISPAY_BUFFER_SIZE / (EPD::WIDTH / 8) ? EPD::HEIGHT : MAX_DISPAY_BUFFER_SIZE / (EPD::WIDTH / 8))
#define MAX_HEIGHT_3C(EPD) (EPD::HEIGHT <= (MAX_DISPAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) ? EPD::HEIGHT : (MAX_DISPAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))
#define MAX_HEIGHT_3C(EPD) (EPD::HEIGHT <= (MAX_DISPAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) ? EPD::HEIGHT : (MAX_DISPAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))
#endif

//#include "GxEPD2_boards_added.h"
//#include "GxEPD2_more_boards_added.h" // private

#if !defined(__AVR) && !defined(_BOARD_GENERIC_STM32F103C_H_)
#include "bitmaps/Bitmaps128x250.h" // 2.13" b/w
#else
#include "bitmaps/Bitmaps128x250.h" // 2.13" b/w
#endif

U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;

const char HelloWorld[] = "Tervehdys Möttönen!";

void setup()
{
  pinMode(D0, OUTPUT);
  digitalWrite(D0, LOW);
  pinMode(D1, OUTPUT);
  digitalWrite(D1, HIGH);
  Serial.begin(115200);
  Serial.println();
  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)\tHeatIndex (C)\t(F)");
  String thisBoard= ARDUINO_BOARD;
  Serial.println(thisBoard);

  dht.setup(D6, DHTesp::DHT22); // Connect DHT sensor to GPIO 17

  //Serial.begin(115200);
  Serial.println();
  Serial.println("setup");
  delay(100);
  display.init(115200);
  u8g2Fonts.begin(display); // connect u8g2 procedures to Adafruit GFX
  // first update should be full refresh
  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);

  u8g2Fonts.setFontMode(1);                 // use u8g2 transparent mode (this is default)
  u8g2Fonts.setFontDirection(0);            // left to right (this is default)
  u8g2Fonts.setForegroundColor(GxEPD_BLACK);         // apply Adafruit GFX color
  u8g2Fonts.setBackgroundColor(GxEPD_WHITE);         // apply Adafruit GFX color
  u8g2Fonts.setFont(u8g2_font_helvR14_tf);  // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
  //u8g2Fonts.setFont(u8g2_font_profont29_mr);  // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
  
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    //display.setCursor(20, 20);
    //display.print(HelloWorld);
    u8g2Fonts.setCursor(10, 30); // start writing at this position
    u8g2Fonts.print(HelloWorld);
  }
  while (display.nextPage());
  delay(1000);
  /*helloWorld();
  delay(1000);
  // partial refresh mode can be used to full screen,
  // effective if display panel hasFastPartialUpdate
  helloFullScreenPartialMode();
  delay(1000);
  helloArduino();
  delay(1000);
  helloEpaper();
  delay(1000);
  //helloValue(123.9, 1);
  //delay(1000);
  showFont("FreeMonoBold9pt7b", &FreeMonoBold9pt7b);
  delay(1000);
  drawBitmaps();
  if (display.epd2.hasPartialUpdate)
  {
    showPartialUpdate();
    delay(1000);
  } // else // on GDEW0154Z04 only full update available, doesn't look nice
  //drawCornerTest();
  //showBox(16, 16, 48, 32, false);
  //showBox(16, 56, 48, 32, true);
  display.powerOff();
  deepSleepTest();*/
  
  Serial.println("setup done");
}

void loop()
{
  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.print(dht.toFahrenheit(temperature), 1);
  Serial.print("\t\t");
  Serial.print(dht.computeHeatIndex(temperature, humidity, false), 1);
  Serial.print("\t\t");
  Serial.println(dht.computeHeatIndex(dht.toFahrenheit(temperature), humidity, true), 1);

  char cstr[16];
  sprintf(cstr, "%2.2f°C,%2.2frh", temperature, humidity);

  helloWorld(cstr);
  //helloFullScreenPartialMode();
  delay(2000);
}

// note for partial update window and setPartialWindow() method:
// partial update window size and position is on byte boundary in physical x direction
// the size is increased in setPartialWindow() if x or w are not multiple of 8 for even rotation, y or h for odd rotation
// see also comment in GxEPD2_BW.h, GxEPD2_3C.h or GxEPD2_GFX.h for method setPartialWindow()


void helloWorld(char cstr[])
{
  //Serial.println("helloWorld");
  display.setPartialWindow(0, 0, display.width(), display.height());
  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(cstr, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center bounding box by transposition of origin:
  //uint16_t x = ((display.width() - tbw) / 2) - tbx;
  //uint16_t y = ((display.height() - tbh) / 2) - tby;
  ////display.setFullWindow();

  u8g2Fonts.setFontMode(1);                 // use u8g2 transparent mode (this is default)
  u8g2Fonts.setFontDirection(0);            // left to right (this is default)
  u8g2Fonts.setForegroundColor(GxEPD_BLACK);         // apply Adafruit GFX color
  u8g2Fonts.setBackgroundColor(GxEPD_WHITE);         // apply Adafruit GFX color
  //u8g2Fonts.setFont(u8g2_font_helvR14_tf);  // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
  u8g2Fonts.setFont(u8g2_font_profont29_mf);  // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
  //uint16_t x = (display.width() - 160) / 2;
  uint16_t x = 10;
  uint16_t y = display.height() / 2;
  
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    //display.setCursor(x, y);
    //display.print(cstr);
    u8g2Fonts.setCursor(x, y); // start writing at this position
    //u8g2Fonts.print("Hello World!");
    u8g2Fonts.print(cstr);
  }
  while (display.nextPage());
  //Serial.println("helloWorld done");
}

void helloFullScreenPartialMode()
{
  Serial.println("helloFullScreenPartialMode");
  const char fullscreen[] = "full screen update";
  const char fpm[] = "fast partial mode";
  const char spm[] = "slow partial mode";
  const char npm[] = "no partial mode";
  display.setPartialWindow(0, 0, display.width(), display.height());
  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  const char* updatemode;
  if (display.epd2.hasFastPartialUpdate)
  {
    updatemode = fpm;
  }
  else if (display.epd2.hasPartialUpdate)
  {
    updatemode = spm;
  }
  else
  {
    updatemode = npm;
  }
  // do this outside of the loop
  int16_t tbx, tby; uint16_t tbw, tbh;
  // center update text
  display.getTextBounds(fullscreen, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t utx = ((display.width() - tbw) / 2) - tbx;
  uint16_t uty = ((display.height() / 4) - tbh / 2) - tby;
  // center update mode
  display.getTextBounds(updatemode, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t umx = ((display.width() - tbw) / 2) - tbx;
  uint16_t umy = ((display.height() * 3 / 4) - tbh / 2) - tby;
  // center HelloWorld
  display.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t hwx = ((display.width() - tbw) / 2) - tbx;
  uint16_t hwy = ((display.height() - tbh) / 2) - tby;
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(hwx, hwy);
    display.print(HelloWorld);
    display.setCursor(utx, uty);
    display.print(fullscreen);
    display.setCursor(umx, umy);
    display.print(updatemode);
  }
  while (display.nextPage());
  Serial.println("helloFullScreenPartialMode done");
}
