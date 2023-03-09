# ELEC3907-Team3C
Repository for ELEC3907 Project Course for Team 3C

--- 
## Current Project Status:

### General: 
- Sensor data gathering and wireless connectivity + Flask web server

### Software: 
Updated March 9.
- Graphing is on track + SOS visual functionality on the webserver.
- Webserver is able to get data from the Arduino via wifi
- updated JSON code on Arduino + AHT20 integration into the main code
- SOS Functionality working via web request and button. Button code is integrated. Added GET /sensor/all endpoint for all sensor data and will add button.
- Writing Python parser to interpret back into dictionary.
- Getting Basic Wifi Functionality updated for Arduino via HTTP request (going with plain text and python parser)
- Formatting HTTP request response (formatting function)
- LAMP webserver setup on Raspberry Pi (Flask Based)
- Added Remote enable/disable for the SOS function, with Python and the Arduino based code

### Hardware:
Updated March 9.
- SD Card Module is working.
- AHT20 is working code. 
- Testing Gas sensor and GPS
- Accelerometer is working, fall detection is tbd.
- Sensor Testing
- What kind of values are we getting out of each sensor?
- button debouncing for SOS functionality

### Integration:
- Sensor Testing
- Needed Libraries

---

**Group Members and Roles:**

Ariel - Hardware, 
Sumaiya - Hardware, 
Owen L. - Integrator, 
Orion - Software, 
Owen M - Software


**Parts List:**
Arduino Mega with Integrated ESP8266, 
Raspberry Pi,
Pulse/Pulse Oximeter Sensor, 
9DOF Accelerometer,
Micro SD Card Interface,
Micro SD card,
Button (SOS button), 
AHT20 Sensor,
CO Sensor,
Enclosure (3d printed),
etc

