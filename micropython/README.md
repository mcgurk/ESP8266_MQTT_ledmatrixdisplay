### Firmware / flashing (ESP32)
- https://micropython.org/download/esp32/
- Firmware built with ESP-IDF v4.x, with support for BLE, but no LAN or PPP, Generic: https://micropython.org/resources/firmware/esp32-idf4-20200830-unstable-v1.12-694-g836bca995.bin

#### Erase flash
```
& $ENV:LOCALAPPDATA\Arduino15\packages\esp32\tools\esptool_py\2.6.1\esptool.exe --chip esp32 --port COM11 erase_flash
```
#### Flash
```
& $ENV:LOCALAPPDATA\Arduino15\packages\esp32\tools\esptool_py\2.6.1\esptool.exe --chip esp32 --port COM11 --baud 460800 write_flash -z 0x1000 $ENV:USERPROFILE\Downloads\esp32-idf4-20200830-unstable-v1.12-694-g836bca995.bin
```
### Windows IDE:
- https://randomnerdtutorials.com/uPyCraftWindows
- https://github.com/adobe-fonts/source-code-pro/raw/release/TTF/SourceCodePro-Regular.ttf ("Install for all users")

### Network
```
import network
sta_if = network.WLAN(network.STA_IF)
sta_if.active(True)
sta_if.connect('ssid', 'password')
```
### webrepl:
```
import webrepl_setup
```
http://micropython.org/webrepl/#192.168.1.185:8266/

### ESP32
ESP32 | MAX7219
--- | ---
5V | VCC
GND	| GND
23 MOSI	| DIN
26 CS	| CS
18 SCK | CLK
(19 MISO)

(You can choose pins quite freely. Maybe this uses software SPI)

### Ledmatrix MAX7219
- https://github.com/mcauser/micropython-max7219
- http://docs.micropython.org/en/latest/library/framebuf.html

### MQTT
- https://github.com/micropython/micropython-lib/blob/master/umqtt.simple/example_pub.py
- https://github.com/micropython/micropython-lib/blob/master/umqtt.simple/example_sub.py
- https://github.com/micropython/micropython-lib/blob/master/umqtt.robust/example_sub_robust.py

