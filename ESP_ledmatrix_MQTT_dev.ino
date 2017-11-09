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
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 0, 60000);
uint8_t clockMode = 1;
uint8_t mqttTimeoutEnabled = 0;
uint32_t lastMqttMessage = 0;
uint32_t mqttTimeoutLimit = 1000UL*3600UL*2UL; // ms
//uint32_t mqttTimeoutLimit = 5000; // ms
uint8_t mqttTimeoutTriggered = 0;
uint32_t millisAtStart;

WiFiClient espClient;
PubSubClient client(espClient);
char mqtt_server[100+1];
char mqtt_username[50+1];
char mqtt_passwd[50+1];
char mqtt_clientname[50+1];
char mqtt_topic[50+1];
char address[2+1];
File f;

volatile uint32_t frame = 0;
uint8_t scrollerMode = 1;

time_t getNtpTime() {
  Serial.println("getNtpTime()"); //debug
  static uint8_t udpSetup = 0;
  if (udpSetup == 0){
    timeClient.begin();
    udpSetup = 1;
  }
  if (timeClient.forceUpdate()) {
    Serial.println("forceupdate ok"); //debug
    return timeClient.getEpochTime();
  } else {
    Serial.println("forceupdate failed"); //debug
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
  //static int frame = 0;
  //if (frame >= 116) return false;
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
  if (token >= '0' && token <='9') {
    if (payload[0] == address[0] && payload[1] == address[1]) {
      Serial.print("(My address)");
      memcpy(payload, payload+2, length-2);
      token = payload[0];
    } else {
      Serial.println("(Not my address)");
      return;
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
  if (token == 'r' || token == 'R') { // "raw"
    clockMode = 0;
    ledMatrix.setOffscreen(payload+1);
    ledMatrix.commit();
  }
  Serial.println();
  if (token == 'S') mqttTimeoutEnabled = 1; else mqttTimeoutEnabled = 0;
}

void reconnect() {
  Serial.print("Attempting MQTT connection...");
  if (mqtt_clientname[0] == '\0') mqtt_clientname[0] = 'a';
  // Attempt to connect
  uint8_t result;
  if (mqtt_username[0] == '\0')
    result = client.connect(mqtt_clientname);
  else
    result = client.connect(mqtt_clientname, mqtt_username, mqtt_passwd);
  if (result) {
    Serial.println("connected");
    // ... and resubscribe
    client.subscribe(mqtt_topic);
    // Once connected, publish an announcement...
    char buf[60];
    sprintf(buf, "clientname: %s, address: %s - connected", mqtt_clientname, address);
    client.publish(mqtt_topic, buf);
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
  Serial.println();
  Serial.println("1 = Give SSID, 2 = Give Wifi-password, 3 = Save SSID and Wifi-password");
  Serial.println("4 = Give and save MQTT-server, 5 = Give and save MQTT-topic, 6 = Give and save MQTT-client name");
  Serial.println("7 = Give and save MQTT-username, 8 = Give and save MQTT-password");
  Serial.println("a = Give and save address (2 numbers)");
  Serial.println("9 = Reset, any other = exit");
  Serial.println();
  Serial.println("MQTT commands:");
  Serial.println("- s/S = string");
  Serial.println("- i/I = intensity (0...f)");
  Serial.println("- t/T = internet clock");
  Serial.println("- r/R = raw (4 bytes / line, 8 lines)");
  Serial.println("(e.g. sHi! or ia or t or r12342234323442345234623472348234)");
  Serial.println("(you can prefix with 2 number address: 10sHi!)");
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
  Serial.begin(115200);
  Serial.setTimeout(20000);
 
  ledMatrix.init();
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

  printHelp();

  WiFi.begin();

  Serial.println();

  uint32_t s = millis();
  while(WiFi.status() != WL_CONNECTED) {
    if (millis() - s > 1000) { Serial.print("."); s = millis(); }
    poll();
  }
  Serial.println("Wifi connected.");

  setSyncProvider(getNtpTime);// Set the external time provider
  setSyncInterval(3600); // Set the number of seconds between re-syncs

  Serial.println(); Serial.println("Loading settings");
  uint8_t result = loadSetting("/mqtt_server.txt", mqtt_server, 100);
  if (result == 1 && mqtt_server[0] != '\0') mqttEnabled = 1;
  loadSetting("/mqtt_clientname.txt", mqtt_clientname, 50);
  loadSetting("/mqtt_topic.txt", mqtt_topic, 50);
  loadSetting("/mqtt_username.txt", mqtt_username, 50);
  loadSetting("/mqtt_passwd.txt", mqtt_passwd, 50);
  loadSetting("/address.txt", address, 2);
  Serial.println();

  if (mqttEnabled) {
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    lastMqttMessage = millis();
  }

  while (frame < 116) { poll(); }
  scrollerMode = 0;

}



void loop() {

  if (mqttEnabled) {
    if (!client.connected()) {
      reconnect();
      poll();
    }
    client.loop();
  }

  if (clockMode) {
    if (timeStatus()) {
      ledMatrix.clearOffscreen();
      ledMatrix.drawMiniText(getFormattedTime(now()));
      ledMatrix.commit();
    } else {
      ledMatrix.clearOffscreen();
      ledMatrix.drawMiniText("99:99:99");
      ledMatrix.commit();
      now(); //try to update clock
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
    }
  }

  //if NTP time has failed
  if (timeStatus() == timeNeedsSync) {
    ledMatrix.writePixel(0, 7, (millis() >> 6) & 1);
    ledMatrix.commit();
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
    case '5':
      Serial.println("Give MQTT-topic");
      readBytes(mqtt_topic, 50);
      saveSetting("/mqtt_topic.txt", mqtt_topic);
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
    case 'a':
    case 'A':
      Serial.println("Give address (2 numbers)");
      readBytes(address, 2);
      saveSetting("/address.txt", address);
      break;
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

