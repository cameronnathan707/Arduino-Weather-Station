# Arduino Weather Station
This is a project that uses multiple sensors to measure the environment around you, some of the sensors have more data available
but I didn't need them for my application, It also has a thermostat that will turn a relay on and off to adjust the temperature, 
can be coded for a heater or air conditioner 
## Getting Started
1. Upload the .ino code to your Arduino Mega, 
2. attach the DH11, and other sensors as stated on the boards and in the code, 
3. connect the 2.8" tft screen shield to the Mega

## Trouble Shooting
### 1. Not uploading to the Mega, 
- Make sure the the board is plugged in to the computer
- Have the right board and COM Port are selected in the editor
- Try disconnecting the sensors connected to the SDA, SCL pins
- If that all fails, try to reboot you computer (MAKE SURE TO SAVE ANY CHANGES MADE FIRST)

### 2. Sensors not reading properly
- Check the connections on the sensors so see if anything fell off
- Make sure the cables are plugged into the right connection on the board 
- Sensor boards can be poorly labelled so look for a data sheet to double check the necessary connections

### 3. Screen not displaying images or touch not working
- Make sure you have declared and initialized the screen
- the libraries may not be formatted for you screen so you may have to try other libraries

