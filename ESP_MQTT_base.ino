#include <ESP8266WiFi.h>
#include <PubSubClient.h> //from library manager
#include <ArduinoJson.h>  //from library manager

//#define SAVECONFIG
#define USE_CONFIG_FILE

struct Mqtt_config {
  char server[100+1];
  char username[30+1];
  char password[30+1];
  char topic[100+1];
  char clientid[30+1];
};

struct Wifi_config {
  char ssid[50+1];
  char password[50+1];
};

struct config {
  Wifi_config wifi;
  Mqtt_config mqtt;
} Config;

config Default = {{"wsid","wpasswd"},{"msrv","muser","mpasswd","mtopic","mid"}};

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
//#define MSG_BUFFER_SIZE	1024
//char msg[MSG_BUFFER_SIZE];
char msg[1024]; //heap?

int value = 0;
StaticJsonDocument<1024> doc; //stack?? or heap?? or static memory??
StaticJsonDocument<1024> jsonconfig;

void setup_wifi() {
  //Serial.setDebugOutput(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(Config.wifi.ssid, Config.wifi.password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
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
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
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
    auto result = client.connect(Config.mqtt.clientid, Config.mqtt.username, Config.mqtt.password);
    if (result) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(Config.mqtt.topic, "hello world"); //greetings message
      // ... and resubscribe
      String inTopic = Config.mqtt.topic;
      inTopic += "/set";
      client.subscribe(inTopic.c_str());
    } else {
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
  Serial.begin(115200);
  //Serial.setDebugOutput(true);
  //while (!Serial) continue;
  while(!Serial);
  delay(1000); //!!!!
  Serial.println(); Serial.print("--- (compilation date: "); Serial.print(__DATE__); Serial.print(" "); Serial.print(__TIME__); Serial.println(") ---");
  SPIFFS.begin();
  SPIFFS_dir();

  /*JsonObject docwifi = jsonconfig.createNestedObject("wifi");
  docwifi["ssid"] = Config.wifi.ssid;
  docwifi["password"] = Config.wifi.password;
  JsonObject docmqtt = jsonconfig.createNestedObject("mqtt");
  docmqtt["server"] = Config.mqtt.server;
  docmqtt["username"] = Config.mqtt.username;
  docmqtt["password"] = Config.mqtt.password;
  docmqtt["topic"] = Config.mqtt.topic;
  docmqtt["clientid"] = Config.mqtt.clientid;
  serializeJsonPretty(jsonconfig, Serial); Serial.println();
  Serial.println("Ready. Halted.");
  while(true) delay(100);*/

  /*#ifdef SAVECONFIG
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(jsonconfig, config);
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }
  serializeJsonPretty(jsonconfig, Serial); Serial.println();
  Serial.println("Saving config.json");
  File f = SPIFFS.open("/config.json", "w");
  if (serializeJsonPretty(jsonconfig, f) == 0) {
    Serial.println(F("Failed to write to file"));
  }
  f.close();
  Serial.println("Ready. Halted.");
  while(true) delay(100);
  #endif*/
  
  #ifdef USE_CONFIG_FILE
  Serial.println("Loading config.json");
  File f = SPIFFS.open("/config.json", "r");
  DeserializationError error = deserializeJson(jsonconfig, f);
  if (error)
    Serial.println(F("Failed to read file, using default configuration"));
  f.close();
  strlcpy(Config.wifi.ssid,                                // <- destination (https://arduinojson.org/v6/example/config/)
          jsonconfig["wifi"]["ssid"] | Default.wifi.ssid,  // <- source
          sizeof(Config.wifi.ssid));                       // <- destination's capacity
  strlcpy(Config.wifi.password,                                    // <- destination
          jsonconfig["wifi"]["password"] | Default.wifi.password,  // <- source
          sizeof(Config.wifi.password));                           // <- destination's capacity
  strlcpy(Config.mqtt.server,                                  // <- destination
          jsonconfig["mqtt"]["server"] | Default.mqtt.server,  // <- source
          sizeof(Config.mqtt.server));                         // <- destination's capacity
  strlcpy(Config.mqtt.username,                                    // <- destination
          jsonconfig["mqtt"]["username"] | Default.mqtt.username,  // <- source
          sizeof(Config.mqtt.username));                           // <- destination's capacity
  strlcpy(Config.mqtt.password,                                    // <- destination
          jsonconfig["mqtt"]["password"] | Default.mqtt.password,  // <- source
          sizeof(Config.mqtt.password));                           // <- destination's capacity
  strlcpy(Config.mqtt.topic,                                 // <- destination
          jsonconfig["mqtt"]["topic"] | Default.mqtt.topic,  // <- source
          sizeof(Config.mqtt.topic));                        // <- destination's capacity
  strlcpy(Config.mqtt.clientid,                                    // <- destination
          jsonconfig["mqtt"]["clientid"] | Default.mqtt.clientid,  // <- source
          sizeof(Config.mqtt.clientid));                           // <- destination's capacity
  Serial.print("Config.wifi.ssid: \""); Serial.print(Config.wifi.ssid); Serial.println("\"");
  Serial.print("Config.wifi.password: \""); Serial.print(Config.wifi.password); Serial.println("\"");
  Serial.print("Config.mqtt.server: \""); Serial.print(Config.mqtt.server); Serial.println("\"");
  Serial.print("Config.mqtt.username: \""); Serial.print(Config.mqtt.username); Serial.println("\"");
  Serial.print("Config.mqtt.password: \""); Serial.print(Config.mqtt.password); Serial.println("\"");
  Serial.print("Config.mqtt.topic: \""); Serial.print(Config.mqtt.topic); Serial.println("\"");
  Serial.print("Config.mqtt.clientid: \""); Serial.print(Config.mqtt.clientid); Serial.println("\"");
  #endif
    
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
    //Serial.println(msg);
    time_t now = time(nullptr);
    String date = ctime(&now);
    //Serial.print(ctime(&current));
    //client.publish("outTopic", msg);
    //Serial.println(ESP.getFreeHeap());
    //DynamicJsonDocument doc(1024); //heap (https://arduinojson.org/v6/example/generator/)
    //char msg[1024]; //stack
    //Serial.println(ESP.getFreeHeap());
    doc["client"] = Config.mqtt.clientid;
    doc["sensor"] = value;
    doc["time"] = now;
    doc["date"] = date;
    doc["data"] = "sdgsgsdfsf";
    Serial.print(date);
    serializeJson(doc, msg);
    Serial.println(msg);
    client.publish("lora", msg);
    //SPIFFS_dir();
  }
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
