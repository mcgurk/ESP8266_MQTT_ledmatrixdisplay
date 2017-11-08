# ESP8266_MQTT_ledmatrixdisplay


1 = Give SSID, 2 = Give Wifi-password, 3 = Save SSID and Wifi-password

4 = Give and save MQTT-server, 5 = Give and save MQTT-topic, 6 = Give and save MQTT-client name

7 = Give and save MQTT-username, 8 = Give and save MQTT-password

a = Give and save address (2 numbers)

9 = Reset


## Commands
- s/S = string
- i/I = intensity (0...f)
- t/T = internet clock
- r/R = raw (4 bytes / line, 8 lines)

(e.g. "sHi!" or "ia" or "t" or "r12342234323442345234623472348234")

## Connections
- 3.3V -> VCC (red) 
- GND -> GND (brown)
- Din -> D7 (orange) (GPIO 13 HSPID)
- CS -> D8 (yellow) (GPIO 15 HSPICS)
- CLK -> D5 (green) (GPIO 14 HSPICLK)
