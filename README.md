# ESP8266_MQTT_ledmatrixdisplay

## Button

- NodeMCU: Flash button (GPIO0/D3 to GND)
- over 0,5s, under 4s: Call home
- over 4s, under 10s: Configuration mode
- over 10s: nothing

## Serial (115200bps)

1 = Give SSID, 2 = Give Wifi-password, 3 = Save SSID and Wifi-password

(4 = Give and save MQTT-server, 6 = Give and save MQTT-client name)

(7 = Give and save MQTT-username, 8 = Give and save MQTT-password)

9 = Reset

A = Call home / update firmware (IOTappstory)


## MQTT

### commands
- s/S = string
- i/I = intensity (0...f)
- c/C = clock
- t/T = temperature
- r/R = raw (4 bytes / line, 8 lines -> 32 bytes)

```
(e.g. "sHi!" or "ia" or "t" or "r12342234323442345234623472348234")
(echo -ne "r\x00\x01\x02\x03\x04..." | mosquitto_pub -d -h localhost -t /ledmatrix/mqtt_client_name -s -u username -P password)
sudo apt install imagemagick
convert nimetön.png -depth 1 koe.gray
hexdump koe.gray
{ echo -ne "r"; cat koe.gray; } | mosquitto_pub -d -h localhost -t /ledmatrix/mqtt_client_name -s -u username -P password

```

(S (not s) starts timeout timer, which triggers if last mqtt message is older than 2 hours. this way you can regularly update e.g. temperature and you see if updates are stopped)

### MQTT testing

Subscribe to all topics:
```
mosquitto_sub -d -h localhost -t /# -u username -P password
```
Send Hi! to display:
```
mosquitto_pub -d -h localhost -t /ledmatrix/mqtt_client_name -m "sHi!" -u username -P password
```

## Connections
- 3.3V -> VCC (red) 
- GND -> GND (brown)
- Din -> D7 (orange) (GPIO 13 HSPID)
- CS -> D8 (yellow) (GPIO 15 HSPICS)
- CLK -> D5 (green) (GPIO 14 HSPICLK)

## IOTAppsStory

- IOTAppStory-ESP (2.1.0-RC3 15.9.2020) (2.0.1 22.1.2019) (2.0.0 8.9.2018)
- https://iotappstory.com
- https://github.com/iotappstory/ESP-Library
- https://iotappstory.com/wiki/arduino-esp/library-api-20

Install these libraries:
- https://github.com/iotappstory/ESP-Library/releases
- https://github.com/me-no-dev/ESPAsyncTCP
- https://github.com/me-no-dev/ESPAsyncWebServer

User-button (default GPIO0/D3/"flash"):
- Short press (>500ms & <4000ms): Call home
- Long press (>4000ms & <10000ms): Reboot to config mode

Wifi and MQTT-information are given in configuration mode.

## Arduino IDE

- File -> Preferences -> Additional Boards Manager URLs: http://arduino.esp8266.com/stable/package_esp8266com_index.json
- NodeMCU 1.0 (ESP-12E Module)

## Flash binary directly from Windows PowerShell

```
& $ENV:LOCALAPPDATA\Arduino15\packages\esp8266\tools\esptool\0.4.13\esptool.exe -vv -cd nodemcu -cb 921600 -cp COM3 -ca 0x00000 -cf $ENV:USERPROFILE\Downloads\ESP_ledmatrix_MQTT_v2.ino.nodemcu.bin
```

## Image from NodeRED
```
var image =
`
------xx-------x--------------xx
--------------------------------
--------------------------------
--------------------------------
--------------------------------
--------------------------------
--xx----------------------------
xx------------------------------
`

var output = "r";
var p = 0;
var bit = 0;
var byte = 0;
while (p < image.length) {
  data = image[p++];
  if (data != '-' && data != 'x') continue;
  if (data == 'x') byte = byte | 1;
  if (bit == 7)  { output += String.fromCharCode(byte); bit = 0; byte = 0; } else { byte = byte << 1; bit++; }
}
  
//msg.retain = true;
msg.payload = new Buffer(output, "ascii");
return msg;
```

## Node-Red Google Sheet
```
var image = [];
for (var y = 0; y < 8; y++)
  for(var x = 0; x < 32; x++) {
    image.push('-');
    if(typeof msg.payload[y] !== 'undefined')
      if(typeof msg.payload[y][x] !== 'undefined') {
        var data = msg.payload[y][x];
        if(data !== "") image[image.length-1] = 'x';
      }
  }

var output = "r";
var p = 0;
var bit = 0;
var byte = 0;
while (p < image.length) {
  data = image[p++];
  if (data != '-' && data != 'x') continue;
  if (data == 'x') byte = byte | 1;
  if (bit == 7)  { output += String.fromCharCode(byte); bit = 0; byte = 0; } else { byte = byte << 1; bit++; }
}
  
//msg.retain = true;
msg.payload = new Buffer(output, "ascii");
return msg;
```
