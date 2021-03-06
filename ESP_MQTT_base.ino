#include <ESP8266WiFi.h>
#include <PubSubClient.h> //from library manager (https://pubsubclient.knolleary.net/)
#include <ArduinoJson.h>  //from library manager (https://arduinojson.org/)
#include <WiFiManager.h> // from library manager (https://github.com/tzapu/WiFiManager) (https://github.com/tzapu/WiFiManager/issues/656)

#define TRIGGER_PIN 0

//#define SAVE_DEFAULT_CONFIG_TO_FILE_AND_HALT
#define CONFIG_FILENAME "config.json"

//#define OVERRIDE_WIFIMANAGER_WIFI_SETTINGS //CAUTION! FOR TESTING ONLY. THIS IS PERMANENT AND WIFIMANAGER CAN'T SET SSID/PASSWORD IF THIS IS ENABLED. SSID AND PASSWORD IS TAKEN FROM CONFIG JSON FILE.

struct Mqtt_config {
  char server[100+1];
  char username[30+1];
  char password[30+1];
  char topic[100+1];
  char clientid[30+1];
};

struct Wifi_config { //NOT NORMALLY USED. WIFIMANAGER MANAGES THESE
  char ssid[50+1];
  char password[50+1];
};

struct config {
  Wifi_config wifi; //NOT NORMALLY USED. WIFIMANAGER MANAGES THESE
  Mqtt_config mqtt;
} Config;

config Default = {{"wsid","wpasswd"},{"msrv","muser","mpasswd","mtopic","mid"}};

WiFiClient espClient;
WiFiManager wm; // global wm instance
WiFiManagerParameter custom_mqtt_server; // global param ( for non blocking w params )
WiFiManagerParameter custom_mqtt_username; // global param ( for non blocking w params )
WiFiManagerParameter custom_mqtt_password; // global param ( for non blocking w params )
WiFiManagerParameter custom_mqtt_topic; // global param ( for non blocking w params )
WiFiManagerParameter custom_mqtt_clientid; // global param ( for non blocking w params )
PubSubClient client(espClient);
unsigned long lastMsg = 0;
char msg[1024]; //heap?
int value = 0;

void setup_wifi() {
  Serial.setDebugOutput(true);
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  #ifdef OVERRIDE_WIFIMANAGER_WIFI_SETTINGS
  WiFi.begin(Config.wifi.ssid, Config.wifi.password);
  #endif
  
  //new (&custom_field) WiFiManagerParameter("customfieldid", "Custom Field Label", "Custom Field Value", 100,"placeholder=\"Custom Field Placeholder\"");
  new (&custom_mqtt_server) WiFiManagerParameter("mqtt_server", "MQTT server", Config.mqtt.server, sizeof(Config.mqtt.server)-1);
  wm.addParameter(&custom_mqtt_server);
  new (&custom_mqtt_username) WiFiManagerParameter("mqtt_username", "MQTT username", Config.mqtt.username, sizeof(Config.mqtt.username)-1);
  wm.addParameter(&custom_mqtt_username);
  new (&custom_mqtt_password) WiFiManagerParameter("mqtt_password", "MQTT password", Config.mqtt.password, sizeof(Config.mqtt.password)-1);
  wm.addParameter(&custom_mqtt_password);
  new (&custom_mqtt_topic) WiFiManagerParameter("mqtt_topic", "MQTT topic", Config.mqtt.topic, sizeof(Config.mqtt.topic)-1);
  wm.addParameter(&custom_mqtt_topic);
  new (&custom_mqtt_clientid) WiFiManagerParameter("mqtt_clientid", "MQTT clientid", Config.mqtt.clientid, sizeof(Config.mqtt.clientid)-1);
  wm.addParameter(&custom_mqtt_clientid);
  
  wm.setSaveParamsCallback(saveParamCallback);
  std::vector<const char *> menu = {"wifi","info","param","sep","restart","exit"};
  wm.setMenu(menu);
  wm.setClass("invert");
  wm.setConfigPortalTimeout(60); // auto close configportal after n seconds
  // bool res = wm.autoConnect(); // auto generated AP name from chipid
  bool res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  // bool res = wm.autoConnect("AutoConnectAP","password"); // password protected ap
  if(!res) {
    Serial.println("Failed to connect or hit timeout");
    // ESP.restart();
  } 
  else {
    //if you get here you have connected to the WiFi    
    Serial.println("connected...yeey :)");
  }
  
  /*WiFi.begin(Config.wifi.ssid, Config.wifi.password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }*/
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    //auto result = client.connect(Config.mqtt.clientid);
    int result;
    if (Config.mqtt.username[0]) {
      result = client.connect(Config.mqtt.clientid, Config.mqtt.username, Config.mqtt.password);
    } else {
      result = client.connect(Config.mqtt.clientid);
    }
    if (result) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(Config.mqtt.topic, "hello world"); //greetings message
      // ... and resubscribe
      String inTopic = Config.mqtt.topic;
      inTopic += "/set";
      client.subscribe(inTopic.c_str());
    } else {
      checkButton(); //WIFIManager
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  Serial.begin(115200);
  //Serial.setDebugOutput(true);
  //while (!Serial) continue;
  while(!Serial);
  delay(1000); //!!!! Arduino Ide:n monitoria varten
  Serial.println(); Serial.print("--- (compilation date: "); Serial.print(__DATE__); Serial.print(" "); Serial.print(__TIME__); Serial.println(") ---");
  SPIFFS.begin();
  SPIFFS_dir();

  #ifdef SAVE_DEFAULT_CONFIG_TO_FILE_AND_HALT
  save_default_settings_to_file();
  #endif
  
  DynamicJsonDocument doccfg(1024);
    
  Serial.println("Loading "CONFIG_FILENAME);
  File f = SPIFFS.open("/"CONFIG_FILENAME, "r");
  DeserializationError error = deserializeJson(doccfg, f);
  if (error)
    Serial.println(F("Failed to read file, using default configuration"));
  f.close();
  strlcpy(Config.wifi.ssid,                           // <- destination (https://arduinojson.org/v6/example/config/)
          doccfg["wifi"]["ssid"] | Default.wifi.ssid, // <- source
          sizeof(Config.wifi.ssid));                  // <- destination's capacity (Note that a byte for the NUL should be included in size.)
  strlcpy(Config.wifi.password,                               // <- destination
          doccfg["wifi"]["password"] | Default.wifi.password, // <- source
          sizeof(Config.wifi.password));                      // <- destination's capacity
  strlcpy(Config.mqtt.server,                             // <- destination
          doccfg["mqtt"]["server"] | Default.mqtt.server, // <- source
          sizeof(Config.mqtt.server));                    // <- destination's capacity
  strlcpy(Config.mqtt.username,                               // <- destination
          doccfg["mqtt"]["username"] | Default.mqtt.username, // <- source
          sizeof(Config.mqtt.username));                      // <- destination's capacity
  strlcpy(Config.mqtt.password,                               // <- destination
          doccfg["mqtt"]["password"] | Default.mqtt.password, // <- source
          sizeof(Config.mqtt.password));                      // <- destination's capacity
  strlcpy(Config.mqtt.topic,                             // <- destination
          doccfg["mqtt"]["topic"] | Default.mqtt.topic,  // <- source
          sizeof(Config.mqtt.topic));                    // <- destination's capacity
  strlcpy(Config.mqtt.clientid,                               // <- destination
          doccfg["mqtt"]["clientid"] | Default.mqtt.clientid, // <- source
          sizeof(Config.mqtt.clientid));                      // <- destination's capacity
  Serial.print("Config.wifi.ssid: \""); Serial.print(Config.wifi.ssid); Serial.println("\"");
  Serial.print("Config.wifi.password: \""); Serial.print(Config.wifi.password); Serial.println("\"");
  Serial.print("Config.mqtt.server: \""); Serial.print(Config.mqtt.server); Serial.println("\"");
  Serial.print("Config.mqtt.username: \""); Serial.print(Config.mqtt.username); Serial.println("\"");
  Serial.print("Config.mqtt.password: \""); Serial.print(Config.mqtt.password); Serial.println("\"");
  Serial.print("Config.mqtt.topic: \""); Serial.print(Config.mqtt.topic); Serial.println("\"");
  Serial.print("Config.mqtt.clientid: \""); Serial.print(Config.mqtt.clientid); Serial.println("\"");
    
  setup_wifi();
  
  client.setServer(Config.mqtt.server, 1883);
  client.setCallback(callback);

  //implement NTP update of timekeeping (with automatic hourly updates)
  //configTime(timezone * 3600, dst * 0, "pool.ntp.org", "time.nist.gov");
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  // info to convert UNIX time to local time (including automatic DST update)
  // setenv("TZ", "EST+5EDT,M3.2.0/2:00:00,M11.1.0/2:00:00", 1);
  
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    //snprintf(msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
    Serial.print("Publish message: ");
    time_t now = time(nullptr);
    char *date = ctime(&now);
    date[strcspn(date, "\r\n")] = 0; //remove \r and/or \n
    DynamicJsonDocument doc(1024); //heap https://arduinojson.org/v6/how-to/reuse-a-json-document/
    //char msg[1024]; //stack
    doc["client"] = Config.mqtt.clientid;
    doc["counter"] = value;
    doc["time"] = now;
    doc["date"] = date;
    doc["data"] = "sdgsgsdfsf";
    JsonObject docesp = doc.createNestedObject("ESP");
    docesp["chipId"] = ESP.getChipId();
    docesp["freeHeap"] = ESP.getFreeHeap();
    serializeJson(doc, msg);
    Serial.println(msg);
    client.publish(Config.mqtt.topic, msg);
  }
  
  checkButton(); //WIFIManager
}

void SPIFFS_dir() {
  Dir dir = SPIFFS.openDir("/");
  Serial.println(); Serial.println("---SPIFFS-directory---");
  while (dir.next()) {
    Serial.print(dir.fileName()); Serial.print(" ");
    File f = dir.openFile("r");
    Serial.println(f.size());
    f.close();
    yield();
  }
  Serial.println("----------------------");
}


void save_default_settings_to_file() {
  DynamicJsonDocument doccfg(1024);
  JsonObject docwifi = doccfg.createNestedObject("wifi");
  docwifi["ssid"] = Default.wifi.ssid;
  docwifi["password"] = Default.wifi.password;
  JsonObject docmqtt = doccfg.createNestedObject("mqtt");
  docmqtt["server"] = Default.mqtt.server;
  docmqtt["username"] = Default.mqtt.username;
  docmqtt["password"] = Default.mqtt.password;
  docmqtt["topic"] = Default.mqtt.topic;
  docmqtt["clientid"] = Default.mqtt.clientid;
  serializeJsonPretty(doccfg, Serial); Serial.println();
  Serial.println(F("Saving "CONFIG_FILENAME));
  File f = SPIFFS.open("/"CONFIG_FILENAME, "w");
  if (serializeJsonPretty(doccfg, f) == 0) {
    Serial.println(F("Failed to write to file"));
  }
  f.close();
  Serial.println("Save Wifi-settings (WiFi.begin(ssid, password))");
  WiFi.begin(Config.wifi.ssid, Config.wifi.password);
  Serial.println("Ready. Halted. Flash sketch again without SAVE_DEFAULT_CONFIG_TO_FILE-flag.");
  while(true) delay(100);
}

 void checkButton(){
  // check for button press
  if ( digitalRead(TRIGGER_PIN) == LOW ) {
    // poor mans debounce/press-hold, code not ideal for production
    delay(50);
    if( digitalRead(TRIGGER_PIN) == LOW ){
      Serial.println("Button Pressed");
      // still holding button for 3000 ms, reset settings, code not ideaa for production
      delay(3000); // reset delay hold
      if( digitalRead(TRIGGER_PIN) == LOW ){
        Serial.println("Button Held");
        Serial.println("Erasing Config, restarting");
        wm.resetSettings();
        ESP.restart();
      }
      
      // start portal w delay
      Serial.println("Starting config portal");
      wm.setConfigPortalTimeout(120);
      
      //if (!wm.startConfigPortal("OnDemandAP","password")) {
      if (!wm.startConfigPortal("OnDemandAP")) {
        Serial.println("failed to connect or hit timeout");
        delay(3000);
        // ESP.restart();
      } else {
        //if you get here you have connected to the WiFi
        Serial.println("connected...yeey :)");
      }
    }
  }
}


String getParam(String name){
  //read parameter from server, for customhmtl input
  String value;
  if(wm.server->hasArg(name)) {
    value = wm.server->arg(name);
  }
  return value;
}

void saveParamCallback(){
  Serial.println("[CALLBACK] saveParamCallback fired");
  //Serial.println("PARAM mqtt_server = " + getParam("mqtt_server"));
  DynamicJsonDocument doccfg(1024);
  JsonObject docwifi = doccfg.createNestedObject("wifi");
  docwifi["ssid"] = Default.wifi.ssid; // not used
  docwifi["password"] = Default.wifi.password; // not used
  JsonObject docmqtt = doccfg.createNestedObject("mqtt");
  docmqtt["server"] = getParam("mqtt_server");
  docmqtt["username"] = getParam("mqtt_username");
  docmqtt["password"] = getParam("mqtt_password");
  docmqtt["topic"] = getParam("mqtt_topic");
  docmqtt["clientid"] = getParam("mqtt_clientid");
  serializeJsonPretty(doccfg, Serial); Serial.println();
  Serial.println(F("Saving "CONFIG_FILENAME));
  File f = SPIFFS.open("/"CONFIG_FILENAME, "w");
  if (serializeJsonPretty(doccfg, f) == 0) {
    Serial.println(F("Failed to write to file"));
  } else Serial.println(F("Done."));
}
