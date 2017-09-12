//ESP8266 Wifi
#include <ESP8266WiFi.h>

//MAX7219
#include <SPI.h>
#include "MAX7219.h"

//NTP + clock
//https://github.com/arduino-libraries/NTPClient/blob/master/NTPClient.cpp
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>

//MQTT + SPIFFS
#include <PubSubClient.h>
#include <FS.h>
// mosquitto_pub -d -t esp -m "1234"


#define NUMBER_OF_DEVICES 4
#define CS_PIN 15
LedMatrix ledMatrix = LedMatrix(NUMBER_OF_DEVICES, CS_PIN);

WiFiUDP ntpUDP;
// By default 'time.nist.gov' is used with 60 seconds update interval and
// no offset
NTPClient timeClient(ntpUDP);
// You can specify the time server pool and the offset, (in seconds)
// additionaly you can specify the update interval (in milliseconds).
//NTPClient timeClient("europe.pool.ntp.org", 3600*3, 60000);
//NTPClient timeClient("europe.pool.ntp.org", 0, 60000);
uint8_t clock = 1;
uint8_t time_set = 0; //don't show clock, if time never set.
uint8_t timer = 1;
uint32_t timer_value = 0;
uint32_t timer_limit = 2*3600*1000; //10000; // ms
uint8_t timer_triggered = 0;

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
uint8_t scroller = 1;
void pollSerial(uint8_t skip_delay = 0);

uint8_t isDST(time_t timeUTC) {
  //toimii ainakin vuoteen 2037
  static time_t dstStart, dstEnd;
  static uint16_t lastYear; //pitäisi alustua automaattisesti käynnistyksessä 0:ksi
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

String getFormattedTime() {
  unsigned long rawTime = timeClient.getEpochTime();
  time_t localDeltaToUTC;
  if (isDST(rawTime)) localDeltaToUTC = 3600*3; else localDeltaToUTC = 3600*2;
  rawTime += localDeltaToUTC;
  unsigned long hours = (rawTime % 86400L) / 3600;
  String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);

  unsigned long minutes = (rawTime % 3600) / 60;
  String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);

  unsigned long seconds = rawTime % 60;
  String secondStr = seconds < 10 ? "0" + String(seconds) : String(seconds);

  return hoursStr + ":" + minuteStr + ":" + secondStr;
  //return hoursStr + ":" + minuteStr;
}


uint8_t pollScroller() {
  static int frame = 0;
  if (frame >= 116) return false;
  ledMatrix.clearOffscreen();
  ledMatrix.scrollTextLeft();
  ledMatrix.drawText();
  ledMatrix.commit();
  return true;
}

void timerUpdate(char c) {
  if (c == 's' || c == 'S' || c == 'r' || c == 'R' || c == 't' || c == 'T') {
    timer = 1;
    timer_value = millis();
    timer_triggered = 0;
  } else {
    timer = 0;
    timer_triggered = 0;
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  char token = payload[0];
  if (token != 'r') { //print payload (if now "raw)
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
    timerUpdate(token);
    clock = 0;
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
    timerUpdate(token);
    time_set = 0;
    clock = 1;
  }
  if (token == 'r' || token == 'R') { // "raw"
    timerUpdate(token);
    clock = 0;
    ledMatrix.setOffscreen(payload+1);
    ledMatrix.commit();
  }
  Serial.println();
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
    //client.publish(mqtt_topic, "ESP8266 connected");
    client.publish(mqtt_topic, buf);
  } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println(" try again in 2 seconds");
    delay(2000);
  }
}


void loadSetting(char *filename, char *data, uint16_t maxsize) {
  pollSerial(1);
  Serial.print("Opening "); Serial.println(filename);
  f = SPIFFS.open(filename, "r");
  f.setTimeout(30);
  if (!f) {
    Serial.println("File doesn't exist.");
  } else {
    uint16_t cnt = f.readBytesUntil('\n', data, maxsize);
    /*uint16_t i = 0;
    while (data[i]) { //get rid of extra line feeds (CR = '\r' 0x0D, LF = '\n' 0x0A)
      Serial.print(data[i], HEX); Serial.print(" ");
      //if (data[i] == '\r' || data[i] == '\n') data[i] = '\0';
      i++;
    }*/
    Serial.print(cnt); Serial.print(" \""); Serial.print(data); Serial.println("\"");
  }
  f.close();
}

void printHelp() {
  Serial.println();
  Serial.println("1 = Give SSID, 2 = Give Wifi-password, 3 = Save SSID and Wifi-password");
  Serial.println("4 = Give and save MQTT-server, 5 = Give and save MQTT-topic, 6 = Give and save MQTT-client name");
  Serial.println("7 = Give and save MQTT-username, 8 = Give and save MQTT-password");
  Serial.println("a = Give and save address (2 numbers)");
  Serial.println("9 = Reset, any other = exit");
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

  SPIFFS.begin();
  Dir dir = SPIFFS.openDir("/");
  Serial.println(); Serial.println("---SPIFFS-directory---");
  while (dir.next()) {
    Serial.print(dir.fileName()); Serial.print(" ");
    f = dir.openFile("r");
    Serial.println(f.size());
    f.close();
    pollSerial();
  }

  printHelp();

  WiFi.begin();

  Serial.println();

  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    pollSerial();
  }
  Serial.println("Wifi connected.");

  Serial.println(); Serial.println("Loading settings");
  loadSetting("/mqtt_server.txt", mqtt_server, 100);
  loadSetting("/mqtt_clientname.txt", mqtt_clientname, 50);
  loadSetting("/mqtt_topic.txt", mqtt_topic, 50);
  loadSetting("/mqtt_username.txt", mqtt_username, 50);
  loadSetting("/mqtt_passwd.txt", mqtt_passwd, 50);
  loadSetting("/address.txt", address, 2);
  Serial.println();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  while (frame < 116) { pollSerial(); }
  scroller = 0;

  timer_value = millis();
}



void loop() {

  if (!client.connected()) {
    reconnect();
    pollSerial();
  }
  client.loop();

  if (clock) {
    uint8_t result = timeClient.update();
    if (result) {
      time_set = 1;
      timer_value = millis();
    }
    if(!result) {
      //Serial.print("ERROR at millis "); Serial.println(millis());
    }

    if (time_set) {
      ledMatrix.clearOffscreen();
      ledMatrix.drawMiniText(getFormattedTime());
      ledMatrix.commit();
    } else {
      ledMatrix.clearOffscreen();
      ledMatrix.drawMiniText("99:99:99");
      ledMatrix.commit();
    }
  }

  if (timer) {
    if (millis()-timer_value > timer_limit) {
      timer_triggered = 1;
    }
    if (timer_triggered) {
      ledMatrix.writePixel(31, 7, (millis() >> 6) & 1);
      ledMatrix.commit();
    }
  }
 
  pollSerial();
  //delay(50);
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
  //data[cnt] = '\0'; //string termination
  //uint16_t i = 0;
  //while (data[i]) { //get rid of extra carriage returns and line feeds (CR = '\r' 0x0D, LF = '\n' 0x0A)
  for (i = 0; i < cnt; i++) {
    if (data[i] == '\r' || data[i] == '\n') data[i] = '\0';
    //i++;
  }
  flush_inbuf;
}

void saveSetting(char *filename, char *data) {
  Serial.print("Saving to "); Serial.println(filename);
  Serial.print("Data: "); Serial.println(data);
  uint16_t i = 0;
  /*while (data[i]) { //get rid of extra carriage returns and line feeds (CR = '\r' 0x0D, LF = '\n' 0x0A)
    //Serial.print(data[i], HEX); Serial.print(" ");
    if (data[i] == '\r' || data[i] == '\n') data[i] = '\0';
    i++;
  }*/
  f = SPIFFS.open(filename, "w");
  if (!f) {
    Serial.println("Error - cannot create file");
  } else {
    f.print(data);
    Serial.println("Saved");
  }
  f.close();
}
  
void pollSerial(uint8_t skip_delay) {
  yield();
  if (!skip_delay) delay(40);
  frame++;  
  if (scroller) pollScroller();

  if (!Serial.available()) return;
  char token;
  //token = waitAndReadNextChar();
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


