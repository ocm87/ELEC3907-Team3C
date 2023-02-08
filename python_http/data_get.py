import requests

ARDUINO_IP = "172.20.10.10"

request_on = "http://" + ARDUINO_IP + "/13/on"
request_off = "http://" + ARDUINO_IP + "/13/off"

# Some notes on the various requests
"""
POST requests for enabling the sos alarm remotly
GET request for reading sensor data

GET reqests data into mySQL database for storage
"""

on = requests.post(request_on)

off = requests.post(request_off)

print(on)
print(on.text)