# Arduino Weather Station
This is a project that uses multiple sensors to measure the environment around you. Some of the sensors have more data available,
but I didn't need them for my application. It also has a thermostat that will turn a relay on and off to adjust the temperature, 
which can be coded for a heater or air conditioner. (heater and air conditioner/relay is untested use at your own risk)
## Getting Started
1. Upload the .ino code to your Arduino Mega. 
2. Attach the DH11.
3. Attach the GY1145 light sensor.
4. Attach the GY-BMp 280.
5. Attach the Relay
6. Connect the 2.8" tft screen shield to the Mega.


## Troubleshooting

###  1. Not uploading to the Mega 
- Make sure the the board is plugged in to the computer.
- Have the right board and COM Port are selected in the editor.
- Try disconnecting the sensors connected to the SDA, SCL pins.
- If that all fails, try to reboot you computer (MAKE SURE TO SAVE ANY CHANGES MADE FIRST).

###  2. Sensors not reading properly
- Check the connections on the sensors so see if anything fell off.
- Make sure the cables are plugged into the right connection on the board.
- Sensor boards can be poorly labelled so look for a data sheet to double check the necessary connections.

###  3. Screen not displaying images or touch not working
- Make sure you have declared and initialized the screen.
- The libraries may not be formatted for you screen so you may have to try other libraries.

## Parts 
[Arduino Mega](https://www.amazon.ca/Elegoo-Board-ATmega2560-ATMEGA16U2-Arduino/dp/B01H4ZLZLQ/ref=sr_1_5?dchild=1&keywords=arduino+mega&qid=1623704530&sr=8-5 )

[Arduino 2.8 inch touch screen](https://www.amazon.ca/Homyl-Display-Controller-Arduino-Mega2560/dp/B07P13GK4D/ref=sr_1_5?dchild=1&keywords=2.8+inch+arduino+screen&qid=1623704495&sr=8-5 )

[DH11 sensor](https://www.amazon.ca/OSEPP-Humidity-Temperature-Components-HUMI-01/dp/B00VDKAPF6/ref=sr_1_13?dchild=1&keywords=dh11&qid=1623704446&sr=8-13)

[GY-1145 light sensor](https://www.banggood.com/GY-1145-DC-3V-I2C-Calibrated-SI1145-Flora-UV-Index-IR-Visible-Light-Digital-Sensor-Module-p-1252374.html?utm_source=googleshopping&utm_medium=cpc_organic&gmcCountry=CA&utm_content=minha&utm_campaign=minha-ca-en-pc&currency=CAD&cur_warehouse=CN&createTmp=1&utm_source=googleshopping&utm_medium=cpc_bgs&utm_content=sandra&utm_campaign=sandra-ssc-ca-all-0304&ad_id=502489423087&gclid=CjwKCAjw_JuGBhBkEiwA1xmbRWFs41OmNANG03EjN1ckOHLTokrjAdAFdEjJWQ9OFTiujvPDVczenRoCaboQAvD_BwE) 

[GY-BMP280 pressure](https://www.banggood.com/GY-BMP280-3_3-High-Precision-Atmospheric-Pressure-Sensor-Module-p-1111135.html?utm_source=googleshopping&utm_medium=cpc_organic&gmcCountry=CA&utm_content=minha&utm_campaign=minha-ca-en-pc&currency=CAD&cur_warehouse=CN&createTmp=1)

[Relay](https://www.electromaker.io/shop/product/single-relay-board)
