// install manually (can't find from library manager):
// ESP8266: https://github.com/me-no-dev/ESPAsyncTCP
// (ESP32: https://github.com/me-no-dev/AsyncTCP)
// https://github.com/me-no-dev/ESPAsyncWebServer

// https://github.com/olikraus/u8g2/issues/105
// https://github.com/olikraus/u8g2/raw/master/tools/font/fony/Fony-wip.zip
// https://github.com/olikraus/u8g2/raw/master/tools/font/bdfconv/bdfconv.exe
// .\bdfconv C:\temp\3x5.bdf -f 1 -n oma_3x5_tn -o c:\temp\3x5.c

const uint8_t oma_3x5_tn[101] = {
  11,0,2,3,2,3,1,4,4,3,5,0,4,9,0,9,
  0,0,0,0,0,0,72,48,7,55,51,148,138,0,49,6,
  54,115,83,6,50,6,55,51,99,57,51,6,55,51,35,3,
  52,7,55,51,145,68,38,53,6,55,51,228,72,54,6,55,
  51,36,69,55,8,55,51,99,42,140,0,56,6,55,51,84,
  69,57,6,55,51,20,73,58,6,109,43,81,0,0,0,0,
  4,255,255,0,0};

#include <ESP8266WiFi.h>
#include <coredecls.h>                  // settimeofday_cb()
#include <TZ.h>

#define IOTappstory
//#define TLS

//#define DONTUPDATE //for testing purposes only!!!

#define VERSION "V3.0.6"

#define MYTZ TZ_Europe_Helsinki

#ifdef IOTappstory
//#define APPNAME "LED-matrix_MQTT"
#define COMPDATE __DATE__ __TIME__
#define MODEBUTTON 0
#include <IOTAppStory.h> // IOTAppStory-ESP by SensorsIot, https://github.com/iotappstory/ESP-Library, https://iotappstory.com/wiki/arduino-esp
IOTAppStory IAS(COMPDATE,MODEBUTTON);
#endif

//MAX7219
#include <U8g2lib.h>
U8G2_MAX7219_32X8_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ D5, /* data=*/ D7, /* cs=*/ D8, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ U8X8_PIN_NONE);
// https://github.com/olikraus/u8g2/wiki/fntlistall

//MQTT (PubSubClient) + SPIFFS 
#include <PubSubClient.h> // from library manager (https://pubsubclient.knolleary.net/)
#include <FS.h>
#ifdef TLS
WiFiClientSecure espClient;     // <-- Change #1: Secure connection to MQTT Server
#else
WiFiClient espClient;
#endif
PubSubClient client(espClient);
uint8_t mqttEnabled = 0;

uint8_t clockMode = 1;
uint8_t timeStatus = 0;
uint8_t mqttTimeoutEnabled = 0;
uint32_t lastMqttMessage = 0;
uint32_t mqttTimeoutLimit = 1000UL*3600UL*2UL; // ms
//uint32_t mqttTimeoutLimit = 5000; // ms
uint8_t mqttTimeoutTriggered = 0;
uint32_t millisAtStart;

#define TOPIC "ledmatrix"
char mqtt_server[100+1];
char mqtt_username[30+1];
char mqtt_passwd[30+1];
char mqtt_clientname[30+1];
char mqtt_topic[20+30+1];
char mqtt_statustopic[20+30+20+1];
char mqtt_statetopic[20+30+20+1];
char mqtt_message[15+20+30+10+50]; //time+topic+clientname+version+status
File f;

volatile uint32_t frame = 0;
uint8_t scrollerMode = 1;

time_t now() {
  return time(nullptr);
}

String getFormattedTime(const tm* tm) {
  int hours = tm->tm_hour;
  String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);
  int minutes = tm->tm_min;
  String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);
  int seconds = tm->tm_sec;
  String secondStr = seconds < 10 ? "0" + String(seconds) : String(seconds);
  return hoursStr + ":" + minuteStr + ":" + secondStr;
  //return hoursStr + ":" + minuteStr;
}


uint8_t pollScroller() {
  /*ledMatrix.clearOffscreen();
  ledMatrix.scrollTextLeft();
  ledMatrix.drawText();
  ledMatrix.commit();*/
  return true;
}


void callback(char* topic, byte* payload, unsigned int length) {
  lastMqttMessage = millis();
  mqttTimeoutTriggered = 0;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: \"");
  char token = payload[0];
  if (token != 'r') { //print payload (if now "raw")
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
  }
  Serial.println("\"");
  memmove(payload, payload+1, length-1); payload[length-1] = '\0'; //chop token and make char str
  if (token == 's' || token == 'S') { // "string"
    clockMode = 0;
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_helvB08_tr);
    u8g2.drawStr(0, 8, (char*)payload);
    u8g2.sendBuffer();
  }
  if (token == 't' || token == 'T') { // "temperature"
    clockMode = 0;
    char* pEnd;
    float f = strtof ((char*)payload, &pEnd);
    //float f = atof((char*)payload);
    Serial.println(f);
    char buf[10] = "--.-\0";
    if (pEnd != ((char*)payload)) sprintf(buf, "%4.1f", fabs(f));
    /*if (pEnd != ((char*)payload)) {
      sprintf(buf, "%4.1f", fabs(f));
    } else {
      sprintf(buf, "--.-");      
    }*/
    Serial.println(buf);
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_helvB08_tr); //8x5 bold
    if (f < 10 && f > -10) {
      u8g2.drawStr(7, 8, (char*)buf);
      if (f < 0) { u8g2.drawLine(6,3,8,3); u8g2.drawLine(6,4,8,4); }
    } else {
      u8g2.drawStr(4, 8, (char*)buf);       
      if (f < 0) { u8g2.drawLine(0,3,2,3); u8g2.drawLine(0,4,2,4); }
    }
    #define CX 24
    u8g2.drawBox(CX+1,0,2,2);
    u8g2.drawStr(CX+3, 8, "C");
    u8g2.sendBuffer();
  }
  if (token == 'i' || token == 'I') { // intensity
    uint8_t br = payload[0];
    uint8_t i;
    if (br > 47 && br < 58) i = br-'0';
    if (br > 64 && br < 71) i = br-'A'+10;
    if (br > 96 && br < 103) i = br-'a'+10;
    u8g2.setContrast(i << 4);
  }
  if (token == 'c' || token == 'C') { // clock
    clockMode = 1;
  }
  if (token == 'p' || token == 'P') { // ping
    mqttStatus("ping");
  }
  #ifdef IOTappstory
  if (token == 'a' || token == 'A') { // IOTappstory call home
    IAS.callHome();
  }
  #endif
  if (token == 'r' || token == 'R') { // "raw"
    clockMode = 0;
    u8g2.clearBuffer();
    u8g2.drawBitmap(0, 0, 4, 8, payload);
    u8g2.sendBuffer();
  }
  Serial.println();
  if (token == 'S' || token == 'I' || token == 'T' || token == 'R' || token == 'P' || token == 'C') mqttTimeoutEnabled = 1;
  if (token == 's' || token == 'i' || token == 't' || token == 'r' || token == 'p' || token == 'c') mqttTimeoutEnabled = 0;
}

void mqttStatus(char* statusmessage) {
  if(client.connected() && mqttEnabled) {
    time_t t = now();
    char *date = ctime(&t);
    sprintf(mqtt_message, "%i - %s - topic: %s, %s - %s", t, date, mqtt_topic, VERSION, statusmessage);
    client.publish(mqtt_statustopic, mqtt_message, true); //retain
  }
}

void reconnect() {
  //u8g2.clearBuffer();  
  //u8g2.setFont(u8g2_font_ncenB08_tr);
  //u8g2.drawStr(0, 8, "MQTT");
  u8g2.drawPixel(0,7);
  u8g2.sendBuffer();
  Serial.print("Attempting MQTT connection...");
  if (mqtt_clientname[0] == '\0') mqtt_clientname[0] = 'a';
  // Attempt to connect
  uint8_t result;
  //don't enable authentication if username is empty or only one character
  if (mqtt_username[0] != '\0' && mqtt_username[1] != '\0')
    //result = client.connect(mqtt_clientname, mqtt_username, mqtt_passwd);
    result = client.connect(mqtt_clientname, mqtt_username, mqtt_passwd, mqtt_statetopic, 0, true, "offline"); //LWT
  else
    result = client.connect(mqtt_clientname);
  if (result) {
    Serial.println("connected to MQTT-server");
    // ... and resubscribe
    client.subscribe(mqtt_topic);
    mqttStatus("connected to MQTT-server");
    client.publish(mqtt_statetopic, "online", true); //retain
    u8g2.setDrawColor(0);
    u8g2.drawPixel(0,7);
    u8g2.sendBuffer();
    u8g2.setDrawColor(1);
  } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println(" try again in 2 seconds");
    uint32_t s=millis();
    while ((millis()-s) < 2000) {
      poll();
    }
  }
}


uint8_t loadSetting(char *filename, char *data, uint16_t maxsize) {
  poll();
  Serial.print("Opening "); Serial.println(filename);
  f = SPIFFS.open(filename, "r");
  f.setTimeout(30);
  if (!f) {
    Serial.println("File doesn't exist.");
    return 0;
  } else {
    uint16_t cnt = f.readBytesUntil('\n', data, maxsize);
    Serial.print(cnt); Serial.print(" \""); Serial.print(data); Serial.println("\"");
  }
  f.close();
  return 1;
}

void printHelp() {
  String message = String(now()) + " - topic: " + mqtt_topic + ", version:" + VERSION;
  Serial.println(message);
  Serial.println();
  Serial.println("1 = Give SSID, 2 = Give Wifi-password, 3 = Save SSID and Wifi-password");
  #ifndef IOTappstory
  Serial.println("4 = Give and save MQTT-server, 6 = Give and save MQTT-client name");
  Serial.println("7 = Give and save MQTT-username, 8 = Give and save MQTT-password");
  #endif
  #ifdef IOTappstory
  Serial.println("A = Call home / update firmware (IOTappstory)");
  #endif
  Serial.println("9 = Reset, any other = exit");
  Serial.println();
  Serial.println("MQTT commands:");
  Serial.println("- s/S = string");
  Serial.println("- i/I = intensity (0...f)");
  Serial.println("- c/C = clock");
  Serial.println("- t/T = temperature");
  Serial.println("- r/R = raw (4 bytes / line, 8 lines)");
  Serial.println("- p/P = ping (reply in \".../status\"-topic)");
  #ifdef IOTappstory
  Serial.println("- a/A = call home / update firmware(IOTappstory)");
  #endif
  Serial.println("(e.g. sHi! or ia or c or r12342234323442345234623472348234)");
  Serial.println("(Uppercase letter starts/resets 2h mqtt-timeout timer)");
  Serial.println();
  Serial.println("-If there is mqtt-timeout timer enabled and no mqtt messages in 2 hours, right bottom pixel blinks.");
  Serial.println("-IOTappstory calls home (checks updates) every 3 hours");
  Serial.println();
  Serial.println("Connections:");
  Serial.println("- 3.3V -> VCC (red)");
  Serial.println("- GND -> GND (brown)");
  Serial.println("- Din -> D7 (orange) (GPIO 13 HSPID)");
  Serial.println("- CS -> D8 (yellow) (GPIO 15 HSPICS)");
  Serial.println("- CLK -> D5 (green) (GPIO 14 HSPICLK)");
}

void time_is_set() {
  timeStatus = 1;
  Serial.println();
  Serial.println("***settimeofday called! (Got NTP time!)");
  Serial.printf("***timezone (settimeofday):  %s\n", getenv("TZ") ? : "(none)");
  time_t epoch_now = now();
  Serial.print("***ctime (settimeofday): ");
  Serial.print(ctime(&epoch_now));
  mqttStatus("NTP-time updated");
}

//----------------------------------------------------------------------------------
//                                    Setup
// ----------------------------------------------------------------------------------
void setup(){
  settimeofday_cb(time_is_set);
  //implement NTP update of timekeeping (with automatic hourly updates)
  configTime(MYTZ, "pool.ntp.org", "time.nist.gov"); //set after IAS.begin()!!!
  // https://github.com/esp8266/Arduino/blob/master/libraries/esp8266/examples/NTP-TZ-DST/NTP-TZ-DST.ino
  
  u8g2.begin();
  u8g2.setContrast(0 << 4);
  u8g2.clearBuffer();  
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 8, "Setup");
  u8g2.sendBuffer();
  
  #ifdef IOTappstory
  IAS.onConfigMode([]() {
    u8g2.clearBuffer();  
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0, 8, "Config");
    u8g2.sendBuffer();    
  });
    IAS.onModeButtonShortPress([]() {
    Serial.println(F(" If mode button is released, I will enter in firmware update mode."));
    Serial.println(F("*-------------------------------------------------------------------------*"));
    clockMode = 0;
    u8g2.clearBuffer();  
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0, 8, "call");
    u8g2.sendBuffer();
  });
  IAS.onModeButtonLongPress([]() {
    Serial.println(F(" If mode button is released, I will enter in configuration mode."));
    Serial.println(F("*-------------------------------------------------------------------------*"));
    clockMode = 0;
    u8g2.clearBuffer();  
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0, 8, "conf");
    u8g2.sendBuffer();
  });
  IAS.onModeButtonVeryLongPress([]() {
    Serial.println(F(" If mode button is released, I won't do anything unless you program me to."));
    Serial.println(F("*-------------------------------------------------------------------------*"));
    clockMode = 1;
    /* TIP! You can use this callback to put your app on it's own configuration mode */
  });
  IAS.onFirmwareUpdateProgress([](int written, int total){
    clockMode = 0;
    Serial.print(".");
    u8g2.clearBuffer();  
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0, 8, "Firmware");
    u8g2.sendBuffer();
    Serial.print("Progess: "); Serial.print(written); Serial.print("/"); Serial.println(total);
    char buf[100];
    sprintf(buf, "Updating firmware: %i/%i", written, total);
    mqttStatus(buf);
  });
  IAS.onFirmwareUpdateCheck([](){
    Serial.println("***FirmwareUpdateCheck");
    mqttStatus("Firmware check");
  });
  IAS.preSetDeviceName("ledmatrixESP"); // preset deviceName this is also your MDNS responder: http://iasblink.local
  IAS.preSetAutoConfig(false); //go autoconfig-mode only with button
  #ifdef DONTUPDATE
  IAS.preSetAutoUpdate(false);
  #endif
    
  char *ms = "mqttserver", *mu = "username", *mp = "passwd", *mc = "clientname";
  IAS.addField(ms, "MQTT server", 100);
  IAS.addField(mu, "MQTT username", 30);
  IAS.addField(mp, "MQTT passwd", 30);
  IAS.addField(mc, "MQTT clientname (e.g. livingroom)", 30);

  IAS.begin('L'); // Optional parameter: What to do with EEPROM on First boot of the app? 'F' Fully erase | 'P' Partial erase(default) | 'L' Leave intact
  //configTime(MYTZ, "pool.ntp.org", "time.nist.gov"); //set after IAS.begin()!!!
  IAS.setCallHome(true); // Set to true to enable calling home frequently (disabled by default)
  IAS.setCallHomeInterval(60*60*3); // Call home interval in seconds, use 60s only for development. Please change it to at least 2 hours in production
  
  strcpy(mqtt_server, ms);
  strcpy(mqtt_username, mu);
  strcpy(mqtt_passwd, mp);
  strcpy(mqtt_clientname, mc);
  sprintf(mqtt_topic, "%s/%s", TOPIC, mqtt_clientname);
  //sprintf(mqtt_statustopic, "%s%s/status", TOPIC, mqtt_clientname);
  //sprintf(mqtt_statustopic, "%s", TOPIC);
  sprintf(mqtt_statustopic, "%s/%s/log", TOPIC, mqtt_clientname);
  sprintf(mqtt_statetopic, "%s/%s/state", TOPIC, mqtt_clientname);
  /*Serial.print(F("ms: \"")); Serial.print(ms); Serial.println(F("\"")); 
  Serial.print(F("mqtt_server: \"")); Serial.print(mqtt_server); Serial.println(F("\"")); 
  Serial.print(F("mqtt_topic: \"")); Serial.print(mqtt_topic); Serial.println(F("\"")); 
  Serial.print(F("mqtt_username: \"")); Serial.print(mqtt_username); Serial.println(F("\"")); */
  
  #else
  Serial.begin(115200);
  /*ledMatrix.clearOffscreen();
  ledMatrix.commit();
  ledMatrix.setText("Jarnon kello");*/
  millisAtStart = millis();
  SPIFFS.begin();
  Dir dir = SPIFFS.openDir("/");
  Serial.println(); Serial.println("---SPIFFS-directory---");
  while (dir.next()) {
    Serial.print(dir.fileName()); Serial.print(" ");
    f = dir.openFile("r");
    Serial.println(f.size());
    f.close();
    poll();
  }
  WiFi.begin();
  Serial.println(); Serial.println("Loading settings");
  uint8_t result = loadSetting("/mqtt_server.txt", mqtt_server, 100);
  if (result == 1 && mqtt_server[0] != '\0') mqttEnabled = 1;
  loadSetting("/mqtt_clientname.txt", mqtt_clientname, 30);
  sprintf(mqtt_topic, "%s/%s", TOPIC, mqtt_clientname);
  //sprintf(mqtt_statustopic, "%s%s/status", TOPIC, mqtt_clientname);
  //sprintf(mqtt_statustopic, "%s", TOPIC);
  sprintf(mqtt_statustopic, "%s/%s/log", TOPIC, mqtt_clientname);
  sprintf(mqtt_statestopic, "%s/%s/state", TOPIC, mqtt_clientname);
  loadSetting("/mqtt_username.txt", mqtt_username, 30);
  loadSetting("/mqtt_passwd.txt", mqtt_passwd, 30);
  Serial.println();
  uint32_t s = millis();
  while(WiFi.status() != WL_CONNECTED) {
    if (millis() - s > 1000) { Serial.print("."); s = millis(); }
    poll();
  }
  Serial.println("Wifi connected.");
  #endif

  #ifdef TLS
  espClient.setInsecure(); // connection will be encrypted but it will accept any server certificate without checking.
  #endif
  
  Serial.setTimeout(20000);
  printHelp();
  
  // if we don't have wifi-connection, wait 30s and reboot
  if (WiFi.status() != WL_CONNECTED) {
    u8g2.clearBuffer();  
    u8g2.setFont(u8g2_font_micro_tr);
    u8g2.drawStr(0, 8, "WIFI err");
    u8g2.sendBuffer();
    uint32_t s = millis();
    while((millis() - s) < 1000*30) {
      if (poll()) {
        while(1) poll(); //if something goes on in serial, disable 30s timer and listen serial
      }
    }
    ESP.restart();
  }

  //don't enable MQTT if server is empty or only one character
  if (mqtt_server[0] != '\0' && mqtt_server[1] != '\0') mqttEnabled = 1;
  if (mqttEnabled) {
    #ifdef TLS
    client.setServer(mqtt_server, 8883);
    #else
    client.setServer(mqtt_server, 1883);
    #endif
    client.setCallback(callback);
    lastMqttMessage = millis();
  }

  u8g2.clearBuffer();  
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 8, "Loop");
  u8g2.sendBuffer();

  /*Serial.printf("***timezone (after setup): %s\n", getenv("TZ") ? : "(none)");
  time_t epoch_now = now();
  Serial.print("***ctime (after setup): ");
  Serial.print(ctime(&epoch_now));*/
 
}



void loop() {

  if (mqttEnabled) {
    if (!client.connected()) {
      //Serial.println("reconnect!!!"); //DEBUG
      reconnect();
      poll();
    }
    client.loop();
  }

  if (clockMode) {
    if (timeStatus) {
      scrollerMode = 0;
      u8g2.clearBuffer();
      u8g2.setFont(oma_3x5_tn);
      time_t epoch_now = now();
      tm *local = localtime(&epoch_now);
      u8g2.drawStr(2, 10, getFormattedTime(local).c_str());
      u8g2.sendBuffer();
    } else {
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_micro_tr);
      u8g2.drawStr(0, 8, "xx:xx:xx");
      u8g2.sendBuffer();
    }
  }

  //if too long from last mqtt-message
  if (mqttTimeoutEnabled) {
    if (millis()-lastMqttMessage > mqttTimeoutLimit) {
      mqttTimeoutTriggered = 1;
    }
    if (mqttTimeoutTriggered) {
      /*ledMatrix.writePixel(31, 7, (millis() >> 6) & 1); // *************************
      ledMatrix.commit();*/
      delay(200);
    }
  }

  poll();
}

uint8_t poll() {
  static uint32_t lastFrame = 0;
  yield();
  frame = (millis() - millisAtStart) >> 5;
  if (scrollerMode && lastFrame != frame) {
    pollScroller();
    lastFrame = frame;
  }
  #ifdef IOTappstory
  IAS.loop();                                // this routine handles the calling home on the configured itnerval as well as reaction of the Flash button. If short press: update of skethc, long press: Configuration
  #endif
  return pollSerial();
}

#define flush_inbuf delay(100); while(Serial.available()) { Serial.read(); yield(); }

char readCharFromInput(uint8_t wait) {
  if (wait) while(Serial.available() == 0) { yield(); } //wait for next char
  if(!Serial.available()) return 0; //exit if nothing in buffer
  char c = Serial.read();
  flush_inbuf; //flush input buffer
  return c;
}
#define readNextChar() readCharFromInput(0)
#define waitAndReadNextChar() readCharFromInput(1)

char ssid[32+1];
char ssid_passwd[64+1];

void readBytes(char *data, uint16_t maxchars) {
  uint16_t cnt = Serial.readBytesUntil('\n', data, maxchars);
  uint16_t i = 0;
  while (data[i]) { //get rid of extra carriage returns and line feeds (CR = '\r' 0x0D, LF = '\n' 0x0A)
    if (data[i] == '\r' || data[i] == '\n') data[i] = '\0';
    i++;
  }
  flush_inbuf;
}

void saveSetting(char *filename, char *data) {
  Serial.print("Saving to "); Serial.println(filename);
  Serial.print("Data: "); Serial.println(data);
  uint16_t i = 0;
  f = SPIFFS.open(filename, "w");
  if (!f) {
    Serial.println("Error - cannot create file");
  } else {
    f.print(data);
    Serial.println("Saved");
  }
  f.close();
}
  
uint8_t pollSerial() {
  if (!Serial.available()) return 0;
  u8g2.clearBuffer();  
  u8g2.setFont(u8g2_font_micro_tr);
  u8g2.drawStr(0, 8, "Serial");
  u8g2.sendBuffer();
  char token;
  token = readNextChar();
  switch(token) { 
    case '1':
      Serial.println("Give SSID");
      readBytes(ssid, 32);
      Serial.print("SSID: ");Serial.println(ssid);
      break;
    case '2':
      Serial.println("Give password");
      readBytes(ssid_passwd, 64);
      Serial.print("Password: ");Serial.println(ssid_passwd);
      break;
    case '3':
      Serial.println("Save SSID and Wifi-password");
      Serial.print("SSID: ");Serial.println(ssid);
      Serial.print("Password: ");Serial.println(ssid_passwd);
      Serial.println("Are you sure (y/n)?");
      token = waitAndReadNextChar();
      if (token == 'y' || token == 'Y') {
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, ssid_passwd);
        Serial.println("SSID and Wifi-password saved");
        #ifdef IOTappstory
        IAS.preSetWifi(ssid, ssid_passwd);
        ESP.restart();
        #endif
      } else Serial.println("SSID and Wifi-password NOT SAVED");
      break;
    case '4':
      Serial.println("Give MQTT-server");
      readBytes(mqtt_server, 100);
      saveSetting("/mqtt_server.txt", mqtt_server);
      break;
    case '6':
      Serial.println("Give MQTT-client name");
      readBytes(mqtt_clientname, 50);
      saveSetting("/mqtt_clientname.txt", mqtt_clientname);
      break;
    case '7':
      Serial.println("Give MQTT-username");
      readBytes(mqtt_username, 50);
      saveSetting("/mqtt_username.txt", mqtt_username);
      break;
    case '8':
      Serial.println("Give MQTT-password");
      readBytes(mqtt_passwd, 50);
      saveSetting("/mqtt_passwd.txt", mqtt_passwd);
      break;
    #ifdef IOTappstory
    case 'A':
      IAS.callHome();
      break;
    #endif
    case '9':
      ESP.restart();
      break;
    default:
      printHelp();
      flush_inbuf;
      break;
  }
  Serial.println();
  return 1;
}
