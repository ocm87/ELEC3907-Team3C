import requests
import json

ARDUINO_IP = "172.20.10.10"

request_on = "http://" + ARDUINO_IP + "/13/on"
request_off = "http://" + ARDUINO_IP + "/13/off"

# Some notes on the various requests
"""
POST requests for enabling the sos alarm remotly
GET request for reading sensor data
set appopriate error codes on raspi response for invalid api endpoints

GET reqests data into mySQL database for storage
"""

on = requests.get(request_on)


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

#! get request data formating
def get_text_data(ip_address: str, api_endpoint: str) -> str:
    req_addr = ip_address + api_endpoint
    response = requests.get(req_addr)
    if (check_return_code(response) == False):
        return ""
    text = response.text
    
    return text


def parse_response_text(text: str) -> str:
    parsed = text.strip()
    parsed = parsed
    return parsed

check_return_code(on, True)

on_text = get_text_data("http://172.20.10.10", "/13/off")
print(parse_response_text(on_text))