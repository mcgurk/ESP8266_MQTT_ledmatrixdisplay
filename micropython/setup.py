import ujson
import network

def config(wssid, wpasswd, mserver, muser, mpasswd):
  conf = {}
  conf['ssid'] = wssid
  conf['passwd'] = wpasswd
  with open('wifi_config.json', 'w') as outfile:
    ujson.dump(conf, outfile)
  conf = {}
  conf['server'] = mserver
  conf['user'] = muser
  conf['passwd'] = mpasswd
  with open('mqtt_config.json', 'w') as outfile:
    ujson.dump(conf, outfile)
  del conf

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

def install():
  print('Loading WiFi-settings from wifi_config.json...')
  with open('wifi_config.json') as infile:
    conf = ujson.load(infile)
  import network
  import utime
  print('Starting network...')
  sta_if = network.WLAN(network.STA_IF)
  sta_if.active(True)
  sta_if.connect(conf['ssid'], conf['passwd'])
  del conf
  if not sta_if.isconnected():
    print("Waiting for connection...")
    while not sta_if.isconnected():
      utime.sleep(1)
  print('Downloading mqtt.py...')
  wget('https://raw.githubusercontent.com/mcgurk/ESP8266_MQTT_ledmatrixdisplay/master/micropython/ESP_MQTT_ledmatrixdisplay.py', 'mqtt.py')
  print('Downloading boot.py...')
  wget('https://raw.githubusercontent.com/mcgurk/ESP8266_MQTT_ledmatrixdisplay/master/micropython/boot.py', 'boot.py')
  print('Downloading utils.py...')
  wget('https://raw.githubusercontent.com/mcgurk/ESP8266_MQTT_ledmatrixdisplay/master/micropython/utils.py', 'utils.py')
  print('Downloading max7219.py...')
  wget('https://raw.githubusercontent.com/mcauser/micropython-max7219/master/max7219.py', 'max7219.py')
