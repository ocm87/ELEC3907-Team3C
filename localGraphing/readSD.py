import pandas, json

# This should read the data off the SD Card 
def readSD (SD_file_path: str) -> dict:
    """
    returns a dictionary with all values from the SD card into list 
    elements in a dictionary from the readings from the SD card.
    
    ex: SD card contents:
    {"SOS":false,"temp":24.50886,"humid":23.45505,"pressure":99945}
    {"SOS":false,"temp":24.51038,"humid":23.46525,"pressure":99949}
    {"SOS":false,"temp":24.53136,"humid":23.43197,"pressure":99959}
    {"SOS":false,"temp":24.53632,"humid":23.45085,"pressure":99955}

    Output:
    {
        "SOS": [false, false, false, false],
        "temp": [24.50886, 24.51038, 24.53136, 24.53632],
        "humid": [23.45505, 23.46525, 23.43197, 23.45085],
        "pressure": [99945, 99949, 99959, 99955]
    }
    """
    SD_file = open(SD_file_path, 'r')
    lines = SD_file.readlines()
    SD_file.close()

    # initialize main dict:
    basic_dict = json.loads(lines[0])
    keys = basic_dict.keys()
    sorted = dict.fromkeys(keys)
    for key in keys:
        sorted[key] = []

    # organize data into dict
    for line in lines:
        line_dict = json.loads(line)
        for key in keys:
            sorted[key].append(line_dict.get(key))

    return sorted

def main ():
    FILENAME = "/Users/owenmacintosh/Documents/ELEC3907-Team3C/localGraphing/exFile.txt"
    testDict = readSD(FILENAME)
    print(testDict)
    return

if __name__ == '__main__':
    main()