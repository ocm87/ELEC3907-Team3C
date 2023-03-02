import requests
import json
import time
import re

ARDUINO_IP = "172.20.10.10"
FULL_ARDUINO_IP = "http://" + ARDUINO_IP

request_on = "http://" + ARDUINO_IP + "/13/on"
request_off = "http://" + ARDUINO_IP + "/13/off"

# TODO Some notes on the various requests
"""
GET reqests data into mySQL database for storage

API ENDPOINTS

POST:
/sos/on - turns on SOS
/sos/off - turns off SOS

GET:
/sos/status - on/off status of the SOS function
/sensor/all - json document of all sensor data
/sensor/x - any other indivudial sensor info that we want to poll
"""

#on = requests.get(request_on)


#! check return code function
def check_return_code(http_response, verbose: bool =False) -> bool:
    """
    This function requires the python http request library. It returns
    true if the the http response code of the Response object is 200, 
    otherwise it returns false.

    The verbose option allows the function to print out response code and
    details if wanted.
    """
    status = False

    code = http_response.status_code
    if (code == 200):
        status = True
        if (verbose == True):
            print(code, "OK")
    else:
        status = False
        if (verbose == True):
            print(code, "BAD")  
    return status

def get_text_data(ip_address: str, api_endpoint: str) -> str:
    """
    Simple function to return the plain text of an HTTP
    response. Takes api IP address and the endpoint as inputs,
    return the plain text from the response.
    """
    req_addr = ip_address + api_endpoint
    response = requests.get(req_addr)
    if (check_return_code(response) == False):
        return "invalid request"
    return response.text

def post_data(ip_address: str, api_endpoint: str) -> str:
    """
    """
    req_addr = ip_address + api_endpoint
    response = requests.post(req_addr)
    if (check_return_code(response) == False):
        return "invalid request"
    return response.text


def parse_response_text(text: str) -> str:
    data = re.findall("{.*}",text)
    data = data[0]
    data = json.loads(data)
    return data


def SOS_toggle(ip_address: str, on_off: str):
    endpoint = "/sos/" + on_off
    text = post_data(ip_address, endpoint)
    if (on_off == "on" and (text.find("SOS true")) >= 1):
        print("SOS turned on")
    elif (on_off == "off" and (text.find("SOS false")) >= 1):
        print("SOS turned off")
    else:
        print("invalid response - look into this")
    return

#SOS_toggle(FULL_ARDUINO_IP, "on")

#time.sleep(5)

#SOS_toggle(FULL_ARDUINO_IP, "off")

text_response = get_text_data(FULL_ARDUINO_IP, "/sensor/all")
#print(text_response)

parsed_data = parse_response_text(text_response)
print(parsed_data)


#test_string = '\nContent-type: text/plain\n{"SOS":false,"time":"3:48pm"}\n\nConnection: close'
#print(parse_response_text(test_string))