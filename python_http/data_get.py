import requests
import json
import re
from pythonping import ping

ARDUINO_IP = "172.20.10.10"
FULL_ARDUINO_IP = "http://" + ARDUINO_IP

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
    return the plain text from the response. Will return false if
    the HTTP code is bad or the target is unreachable.
    """
    req_addr = ip_address + api_endpoint

    # Check if the target is reachable:
    try:
        ping (ip_address, timeout=1, verbose=True)
    except:
        print("Target not reachable")
        return False
    response = requests.get(req_addr)

    if (check_return_code(response) == False):
        #response.raise_for_status()
        return False
    return response.text

def post_data(ip_address: str, api_endpoint: str) -> str:
    """
    Send a HTTP POST request wihtout payload to a given 
    HTTP endpoint. Returns the plain text response.
    """
    req_addr = ip_address + api_endpoint
    response = requests.post(req_addr)
    if (check_return_code(response) == False):
        return "invalid request"
    return response.text


def parse_response_text(text: str) -> dict:
    """
    Parse the HTTP response plain text from the Arduino.
    Returns the dictionary of sensor data
    """
    data = re.findall("{.*}",text)
    data = data[0]
    data = json.loads(data)
    return data


def SOS_toggle(ip_address: str, on_off: str):
    endpoint = "/sos/" + on_off
    text = post_data(ip_address, endpoint)
    if (on_off == "on" and (text.find("SOS Var true")) >= 1):
        print("SOS turned on")
    elif (on_off == "off" and (text.find("SOS Var false")) >= 1):
        print("SOS turned off")
    return

#SOS_toggle(FULL_ARDUINO_IP, "on")
#SOS_toggle(FULL_ARDUINO_IP, "off")

#text_response = get_text_data(FULL_ARDUINO_IP, "/sensor/all")
#if (text_response != False):
#    parsed_data = parse_response_text(text_response)
#    print(parsed_data)