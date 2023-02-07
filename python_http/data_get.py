import requests

ARDUINO_IP = "172.20.10.10"

request_on = "http://" + ARDUINO_IP + "/13/on"
request_off = "http://" + ARDUINO_IP + "/13/off"

on = requests.get(request_on)

off = ""

print(on.text)





