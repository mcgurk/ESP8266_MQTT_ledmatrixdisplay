# ESP8266_MQTT_ledmatrixdisplay


1 = Give SSID, 2 = Give Wifi-password, 3 = Save SSID and Wifi-password

4 = Give and save MQTT-server, 5 = Give and save MQTT-topic, 6 = Give and save MQTT-client name

7 = Give and save MQTT-username, 8 = Give and save MQTT-password

9 = Reset


## MQTT commands
- s/S = string
- i/I = intensity (0...f)
- t/T = internet clock
- r/R = raw (4 bytes / line, 8 lines)

(e.g. "sHi!" or "ia" or "t" or "r12342234323442345234623472348234")

(S (not s) starts timeout timer, which triggers if last mqtt message is older than 2 hours. this way you can regularly update e.g. temperature and you see if updates are stopped)

## MQTT testing
```
mosquitto_sub -d -h localhost -t /# -u username -P password
```
```
mosquitto_pub -d -h localhost -t /ledmatrix/mqtt_client_name -m "sHi!" -u username -P password
```

## Connections
- 3.3V -> VCC (red) 
- GND -> GND (brown)
- Din -> D7 (orange) (GPIO 13 HSPID)
- CS -> D8 (yellow) (GPIO 15 HSPICS)
- CLK -> D5 (green) (GPIO 14 HSPICLK)
