from data_get import get_text_data, parse_response_text, SOS_toggle, FULL_ARDUINO_IP

# simple test file to remove text from functions file
text_response = get_text_data(FULL_ARDUINO_IP, "/sensor/all")
if (text_response != False):
    parsed_data = parse_response_text(text_response)
    print(parsed_data)

#SOS_toggle(FULL_ARDUINO_IP, "on")
#SOS_toggle(FULL_ARDUINO_IP, "off")