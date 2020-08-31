import ujson
with open('wifi_config.json') as infile:
  conf = ujson.load(infile)
import network
sta_if = network.WLAN(network.STA_IF)
sta_if.active(True)
sta_if.connect(conf['ssid'], conf['passwd'])
del conf

from utils import *
#from setup import wget
