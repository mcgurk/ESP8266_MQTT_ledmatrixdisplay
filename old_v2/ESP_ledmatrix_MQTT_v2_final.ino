#define IOTappstory

#define VERSION "V2.0.2"

#ifdef IOTappstory
//#define APPNAME "LED-matrix_MQTT"
#define COMPDATE __DATE__ __TIME__
#define MODEBUTTON 0
#include <IOTAppStory.h>
IOTAppStory IAS(COMPDATE,MODEBUTTON);
#endif

//ESP8266 Wifi
#include <ESP8266WiFi.h>

//MAX7219
#include <SPI.h>
#include "MAX7219.h"

//NTP + clock (NTPClient, Time)
//https://github.com/arduino-libraries/NTPClient/blob/master/NTPClient.cpp
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>

//MQTT + SPIFFS (PubSubClient)
#include <PubSubClient.h>
#include <FS.h>
uint8_t mqttEnabled = 0;

//MAX7219
#define NUMBER_OF_DEVICES 4
#define CS_PIN 15
LedMatrix ledMatrix = LedMatrix(NUMBER_OF_DEVICES, CS_PIN);

//NTP
WiFiUDP ntpUDP;
// By default 'time.nist.gov' is used with 60 seconds update interval and no offset
//NTPClient timeClient(ntpUDP);
// You can specify the time server pool and the offset, (in seconds)
// additionaly you can specify the update interval (in milliseconds).
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 0, 5000); //5000 = interval before first succesful update
//NTPClient timeClient(ntpUDP, "www.fgsdsdgdssdgf.com", 0, 1000);
uint8_t clockMode = 1;
uint8_t ntpErrorCnt = 0;
uint8_t mqttTimeoutEnabled = 0;
uint32_t lastMqttMessage = 0;
uint32_t mqttTimeoutLimit = 1000UL*3600UL*2UL; // ms
//uint32_t mqttTimeoutLimit = 5000; // ms
uint8_t mqttTimeoutTriggered = 0;
uint32_t millisAtStart;

WiFiClient espClient;
PubSubClient client(espClient);
#define TOPIC "ledmatrix/"
char mqtt_server[100+1];
char mqtt_username[30+1];
char mqtt_passwd[30+1];
char mqtt_clientname[30+1];
char mqtt_topic[20+30+1];
char mqtt_statustopic[20+30+20+1];
char mqtt_message[15+20+30+10+50]; //time+topic+clientname+version+status
File f;

volatile uint32_t frame = 0;
uint8_t scrollerMode = 1;

time_t getNtpTime() {
  Serial.println("getNtpTime()"); //debug
  //mqttStatus("GetNtpTime");
  static uint8_t udpSetup = 0;
  if (udpSetup == 0){
    Serial.println("first getNtpTime call"); //DEBUG
    timeClient.begin();
    udpSetup = 1;
  }
  if (timeClient.forceUpdate()) {
    Serial.println("forceupdate ok"); //debug
    ntpErrorCnt = 0;
    uint32_t ti = timeClient.getEpochTime();
    Serial.println(ti); //DEBUG
    //mqttStatus("GetNtpTime ok");
    return ti;
  } else {
    Serial.println("forceupdate failed"); //debug
    //mqttStatus("GetNtpTime failed");
    ntpErrorCnt++;
    return 0;
  }
}

uint8_t isDST(time_t timeUTC) {
  //toimii ainakin vuoteen 2037
  static time_t dstStart, dstEnd;
  static uint16_t lastYear = 0;
  uint16_t yearNow = year(timeUTC);
  
  if (yearNow != lastYear) {
    time_t t;
    uint8_t d;
    tmElements_t tm; 

    tm.Year = yearNow - 1970;
    tm.Month = 3 + 1; //dstStart at 3rd month last sunday at 01:00:00 (UTC)
    tm.Day = 1;
    tm.Hour = 1;
    tm.Minute = 0;
    tm.Second = 0;
    t = makeTime(tm) - 86400UL; //last day of month
    d = weekday(t) - 1; // weekday(x): 1 = sunday, d: 0 = sunday
    dstStart = t - (((time_t)d) * 3600UL * 24UL);

    tm.Month = 10 + 1; //dstEnd at 10th month last sunday at 00:59:59 (UTC)
    tm.Day = 1;
    tm.Hour = 0;
    tm.Minute = 59;
    tm.Second = 59;
    t = makeTime(tm) - 86400UL; //last day of month
    d = weekday(t) - 1; // weekday(x): 1 = sunday, d: 0 = sunday
    dstEnd = t - (((time_t)d) * 3600UL * 24UL);    
    
    lastYear = yearNow;
  }
  
  if (timeUTC >= dstStart) if (timeUTC <= dstEnd) return 1;
  return 0;
}

String getFormattedTime(unsigned long epoch) {
  time_t localDeltaToUTC;
  if (isDST(epoch)) localDeltaToUTC = 3600*3; else localDeltaToUTC = 3600*2;
  unsigned long local = epoch + localDeltaToUTC;
  unsigned long hours = (local % 86400L) / 3600;
  String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);

  unsigned long minutes = (local % 3600) / 60;
  String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);

  unsigned long seconds = local % 60;
  String secondStr = seconds < 10 ? "0" + String(seconds) : String(seconds);

  return hoursStr + ":" + minuteStr + ":" + secondStr;
  //return hoursStr + ":" + minuteStr;
}


uint8_t pollScroller() {
  ledMatrix.clearOffscreen();
  ledMatrix.scrollTextLeft();
  ledMatrix.drawText();
  ledMatrix.commit();
  return true;
}


void callback(char* topic, byte* payload, unsigned int length) {
  lastMqttMessage = millis();
  mqttTimeoutTriggered = 0;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  char token = payload[0];
  if (token != 'r') { //print payload (if now "raw")
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
  }
  if (token == 's' || token == 'S') { // "string"
    clockMode = 0;
    Serial.print(", ");
    ledMatrix.clearOffscreen();
    for (int i = 0; i < length-1; i++) {
      if (i >= 4) break;
      ledMatrix.drawChar((char)payload[i+1], i);
      Serial.print((char)payload[i+1]);
    }
    ledMatrix.commit();
  }
  if (token == 'i' || token == 'I') { // intensity
    uint8_t br = payload[1];
    if (br > 47 && br < 58) ledMatrix.setIntensity(br-'0');
    if (br > 64 && br < 71) ledMatrix.setIntensity(br-'A'+10);
    if (br > 96 && br < 103) ledMatrix.setIntensity(br-'a'+10);
  }
  if (token == 't' || token == 'T') { // clock
    clockMode = 1;
  }
  if (token == 'p' || token == 'P') { // ping
    mqttStatus("ping");
  }
  #ifdef IOTappstory
  if (token == 'c' || token == 'C') { // IOTappstory call home
    IAS.callHome();
  }
  #endif
  if (token == 'r' || token == 'R') { // "raw"
    clockMode = 0;
    ledMatrix.setOffscreen(payload+1);
    ledMatrix.commit();
  }
  Serial.println();
  if (token == 'S' || token == 'I' || token == 'T' || token == 'R' || token == 'P') mqttTimeoutEnabled = 1;
  if (token == 's' || token == 'i' || token == 't' || token == 'r' || token == 'p') mqttTimeoutEnabled = 0;
}

void mqttStatus(char* statusmessage) { // don't call from getNtpTime() because now()!!!
  if(client.connected() && mqttEnabled) {
    //String message = String(now()) + " - topic: " + String(mqtt_topic) + ", clientname: " + String(mqtt_clientname) + ", address: " + String(address) + " - " + status;
    uint32_t ti = now(); // can't use now() from getNtpTime()!!!
    sprintf(mqtt_message, "%i - topic: %s, %s - %s", ti, mqtt_topic, VERSION, statusmessage);
    client.publish(mqtt_statustopic, mqtt_message, true); //retain
    //message = String(now()) + " - topic: " + mqtt_topic + " - " + status;
    //String topic = String(mqtt_topic) + "/status";
    //client.publish(topic.c_str(), message.c_str(), true); //retain
  }
}

void reconnect() {
  Serial.print("Attempting MQTT connection...");
  if (mqtt_clientname[0] == '\0') mqtt_clientname[0] = 'a';
  // Attempt to connect
  uint8_t result;
  //don't enable authentication if username is empty or only one character
  if (mqtt_username[0] != '\0' && mqtt_username[1] != '\0')
    result = client.connect(mqtt_clientname, mqtt_username, mqtt_passwd);
  else
    result = client.connect(mqtt_clientname);
  if (result) {
    Serial.println("connected");
    // ... and resubscribe
    client.subscribe(mqtt_topic);
    mqttStatus("connected");
  } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println(" try again in 2 seconds");
    delay(2000); //!!!
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
  //String message = String(now()) + " - topic: " + String(mqtt_topic) + ", clientname: " + String(mqtt_clientname) + ", address: " + String(address);
  String message = String(now()) + " - topic: " + mqtt_topic + ", version:" + VERSION;
  Serial.println(message);
  Serial.println();
  #ifndef IOTappstory
  Serial.println("1 = Give SSID, 2 = Give Wifi-password, 3 = Save SSID and Wifi-password");
  Serial.println("4 = Give and save MQTT-server, 6 = Give and save MQTT-client name");
  Serial.println("7 = Give and save MQTT-username, 8 = Give and save MQTT-password");
  #endif
  #ifdef IOTappstory
  Serial.println("C = Call home / update firmware (IOTappstory)");
  #endif
  Serial.println("9 = Reset, any other = exit");
  Serial.println();
  Serial.println("MQTT commands:");
  Serial.println("- s/S = string");
  Serial.println("- i/I = intensity (0...f)");
  Serial.println("- t/T = internet clock");
  Serial.println("- r/R = raw (4 bytes / line, 8 lines)");
  Serial.println("- p/P = ping (reply in \".../status\"-topic)");
  #ifdef IOTappstory
  Serial.println("- c/C = call home / update firmware(IOTappstory)");
  #endif
  Serial.println("(e.g. sHi! or ia or t or r12342234323442345234623472348234)");
  Serial.println("(Uppercase letter starts/resets 2h mqtt-timeout timer)");
  Serial.println();
  Serial.println("-If there is no connection to NTP-server in 24 hour, left bottom pixel blinks.");
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

// ----------------------------------------------------------------------------------
//                                    Setup
// ----------------------------------------------------------------------------------
void setup(){

  ledMatrix.init();
  ledMatrix.clearOffscreen();
  for (int i = 0; i < 4; i++) ledMatrix.drawChar('-', i);
  ledMatrix.commit();
  
  #ifdef IOTappstory
  IAS.preSetDeviceName("ledmatrixESP"); // preset deviceName this is also your MDNS responder: http://iasblink.local
  IAS.preSetAutoConfig(false); //go autoconfig-mode only with button
    
  char *ms, *mu, *mp, *mc;
  IAS.addField(ms, "MQTT server", 100);
  IAS.addField(mu, "MQTT username", 30);
  IAS.addField(mp, "MQTT passwd", 30);
  IAS.addField(mc, "MQTT clientname (e.g. livingroom)", 30);

  IAS.begin('L'); // Optional parameter: What to do with EEPROM on First boot of the app? 'F' Fully erase | 'P' Partial erase(default) | 'L' Leave intact
  IAS.setCallHome(true); // Set to true to enable calling home frequently (disabled by default)
  IAS.setCallHomeInterval(60*60*3); // Call home interval in seconds, use 60s only for development. Please change it to at least 2 hours in production
  
  strcpy(mqtt_server, ms);
  strcpy(mqtt_username, mu);
  strcpy(mqtt_passwd, mp);
  strcpy(mqtt_clientname, mc);
  sprintf(mqtt_topic, "%s%s", TOPIC, mqtt_clientname);
  sprintf(mqtt_statustopic, "%s%s/status", TOPIC, mqtt_clientname);
  /*Serial.print(F("ms: \"")); Serial.print(ms); Serial.println(F("\"")); 
  Serial.print(F("mqtt_server: \"")); Serial.print(mqtt_server); Serial.println(F("\"")); 
  Serial.print(F("mqtt_topic: \"")); Serial.print(mqtt_topic); Serial.println(F("\"")); 
  Serial.print(F("mqtt_username: \"")); Serial.print(mqtt_username); Serial.println(F("\"")); */
  
  #else
  Serial.begin(115200);
  ledMatrix.clearOffscreen();
  ledMatrix.commit();
  ledMatrix.setText("Jarnon kello");
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
  sprintf(mqtt_topic, "%s%s", TOPIC, mqtt_clientname);
  sprintf(mqtt_statustopic, "%s%s/status", TOPIC, mqtt_clientname);
  loadSetting("/mqtt_username.txt", mqtt_username, 30);
  loadSetting("/mqtt_passwd.txt", mqtt_passwd, 30);
  #endif
  
  Serial.println();
  uint32_t s = millis();
  while(WiFi.status() != WL_CONNECTED) {
    if (millis() - s > 1000) { Serial.print("."); s = millis(); }
    poll();
  }
  Serial.println("Wifi connected.");

  setSyncProvider(getNtpTime);// Set the external time provider
  setSyncInterval(3600); // Set the number of seconds between re-syncs

  //don't enable MQTT if server is empty or only one character
  if (mqtt_server[0] != '\0' && mqtt_server[1] != '\0') mqttEnabled = 1;
  if (mqttEnabled) {
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    lastMqttMessage = millis();
  }

  Serial.setTimeout(20000);

  printHelp();

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
    if (timeStatus()) {
      scrollerMode = 0;
      ledMatrix.clearOffscreen();
      ledMatrix.drawMiniText(getFormattedTime(now()));
      ledMatrix.commit();
    } else {
      //ledMatrix.clearOffscreen();
      //ledMatrix.drawMiniText("99:99:99");
      //ledMatrix.commit();
      //now(); //try to update clock
      Serial.println("Try to get ntp time again...");
      uint32_t result = timeClient.update(); //try to get ntptime
      if (result) setTime(result); //if success, update clock
    }
  }

  //if too long from last mqtt-message
  if (mqttTimeoutEnabled) {
    if (millis()-lastMqttMessage > mqttTimeoutLimit) {
      mqttTimeoutTriggered = 1;
    }
    if (mqttTimeoutTriggered) {
      ledMatrix.writePixel(31, 7, (millis() >> 6) & 1);
      ledMatrix.commit();
      delay(200);
    }
  }

  //if NTP time has failed
  //if (timeStatus() == timeNeedsSync) {
  if (ntpErrorCnt > 23) {
    ledMatrix.writePixel(0, 7, (millis() >> 6) & 1);
    ledMatrix.commit();
    delay(200);
  }
 
  poll();
}

void poll() {
  static uint32_t lastFrame = 0;
  yield();
  frame = (millis() - millisAtStart) >> 5;
  if (scrollerMode && lastFrame != frame) {
    pollScroller();
    lastFrame = frame;
  }
  pollSerial();
  #ifdef IOTappstory
  IAS.loop();                                // this routine handles the calling home on the configured itnerval as well as reaction of the Flash button. If short press: update of skethc, long press: Configuration
  #endif
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
  
void pollSerial() {
  if (!Serial.available()) return;
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
    case 'C':
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
}
