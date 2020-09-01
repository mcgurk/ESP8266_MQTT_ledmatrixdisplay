## Wiring (ESP32)
ESP32 | MAX7219
--- | ---
5V | VCC
GND	| GND
23 VSPI MOSI | DIN
18 VSPI CLK | CLK
5 VSPI CS0	| CS
(19 VSPI MISO)

(You can choose pins quite freely. Maybe this uses software SPI)

## Firmware / flashing (ESP32)
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

## Setup/Install
```
# ctrl-E
wssid = 'my_ssid'
wpasswd = 'my_passwd'
mserver = 'my_mqtt_server'
muser = 'my_mqtt_username'
mpasswd = 'my_mqtt_password'
import network
import utime
sta_if = network.WLAN(network.STA_IF)
sta_if.active(True)
sta_if.connect(wssid, wpasswd)
if not sta_if.isconnected():
    print("Waiting for connection...")
    while not sta_if.isconnected():
        utime.sleep(1)
def wget(url, filename):
  import upip
  s = upip.url_open(url)
  with open(filename,'w') as f:
    while True:
      data = s.read(512)
      if (len(data) < 1):
        break
      f.write(data)
  s.close()
wget('https://raw.githubusercontent.com/mcgurk/ESP8266_MQTT_ledmatrixdisplay/master/micropython/setup.py', 'setup.py')
import setup
setup.config(wssid, wpasswd, mserver, muser, mpasswd)
setup.install()
# ctrl-D
```
### Reinstall files
```
import setup
setup.install()
```


## Windows IDE:
- https://randomnerdtutorials.com/uPyCraftWindows
- https://github.com/adobe-fonts/source-code-pro/raw/release/TTF/SourceCodePro-Regular.ttf ("Install for all users")

## Network
```
import network
sta_if = network.WLAN(network.STA_IF)
sta_if.active(True)
sta_if.connect('ssid', 'password')
```
## Network at boot and credentials from config file
Save credentials
```
# ctrl-E
import ujson
conf = {}
conf['ssid'] = 'my_wifi_ssid'
conf['passwd'] = 'my_wifi_password'
with open('wifi_config.json', 'w') as outfile:
  ujson.dump(conf, outfile)
del conf
# ctrl-D
```
Add to boot.py
```
import ujson
with open('wifi_config.json') as infile:
  conf = ujson.load(infile)
import network
sta_if = network.WLAN(network.STA_IF)
sta_if.active(True)
sta_if.connect(conf['ssid'], conf['passwd'])
del conf
```

## Upload file with repl
```
# ctrl-E
code=r'''(paste)'''
with open('utils.py','w') as f:
  f.write(code)
# ctrl-D
```

## Download from web
```
# ctrl-E
def wget(url, filename):
  import upip
  s = upip.url_open(url)
  with open(filename,'w') as f:
    while True:
      data = s.read(512)
      if (len(data) < 1):
        break
      f.write(data)
  s.close()
# ctrl-D
```
```
wget('https://raw.githubusercontent.com/mcgurk/ESP8266_MQTT_ledmatrixdisplay/master/micropython/utils.py', 'utils.py')
```

### webrepl:
```
import webrepl_setup
```
http://micropython.org/webrepl/#192.168.1.185:8266/



### Ledmatrix MAX7219
```
import max7219
from machine import Pin, SPI
spi = SPI(1, baudrate=10000000, polarity=1, phase=0, sck=Pin(18), mosi=Pin(23))
ss = Pin(5, Pin.OUT)
display = max7219.Matrix8x8(spi, ss, 4)
display.fill(0)
display.text('1234',0,0,1)
display.show()
```
- https://github.com/mcauser/micropython-max7219
- http://docs.micropython.org/en/latest/library/framebuf.html

### MQTT
- https://github.com/micropython/micropython-lib/blob/master/umqtt.simple/example_pub.py
- https://github.com/micropython/micropython-lib/blob/master/umqtt.simple/example_sub.py
- https://github.com/micropython/micropython-lib/blob/master/umqtt.robust/example_sub_robust.py

## TODO
- Error handling with received message
- Maintaining MQTT-connection
