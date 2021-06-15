/* Arduino Weather Station 
 *  by: Nathan Cameron
 *  
 *  Adapted from pcimule 
 *  https://create.arduino.cc/projecthub/pcimule/weather-station-general-detail-screen-0a96a2
 *  
 *  This is a demonstration for a coding camp to 
 *  show the possibilities of Arduino
 *  
 *  Date: 6/15/21
 */

// Libraries Declaration
#include <Elegoo_GFX.h>       // Core graphics library
#include <Elegoo_TFTLCD.h>    // Hardware-specific library
#include <TouchScreen.h>      // Touch Screen Library

#include <dht.h>              //DHT Library
#include <Wire.h>
#include "Adafruit_SI1145.h"  //UV sensor
#include <Adafruit_BMP280.h>  //temperature/altitude


// The control pins for the LCD 
#define LCD_CS A3 
#define LCD_CD A2
#define LCD_WR A1 
#define LCD_RD A0
#define LCD_RESET A4 

// Color definitions
#define BLACK       0x0000      
#define NAVY        0x000F      
#define DARKGREEN   0x03E0     
#define DARKCYAN    0x03EF      
#define MAROON      0x7800      
#define PURPLE      0x780F      
#define OLIVE       0x7BE0      
#define LIGHTGREY   0xC618      
#define DARKGREY    0x7BEF      
#define BLUE        0x001F      
#define GREEN       0x07E0      
#define CYAN        0x07FF      
#define RED         0xF800      
#define MAGENTA     0xF81F      
#define YELLOW      0xFFE0      
#define WHITE       0xFFFF      
#define ORANGE      0xFD20      
#define GREENYELLOW 0xAFE5      
#define PINK        0xF81F


#define YP A3  // must be an analog pin
#define XM A2  // must be an analog pin
#define YM 9   
#define XP 8   


#define TS_MINX 120
#define TS_MAXX 900

#define TS_MINY 70
#define TS_MAXY 920

#define MINPRESSURE 10
#define MAXPRESSURE 1000

//Screen Declaration
Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

//TouchScreen Area Declaration
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// I2C Interface
Adafruit_BMP280 bmp; 
Adafruit_SI1145 uv = Adafruit_SI1145();

dht DHT;

//Button object declaration
Elegoo_GFX_Button buttons;

//Custiom Variables
uint16_t identifier;                 //Store Screen Identifier
#define DHT11_PIN 22                 //what pin we're connected to DTH Sensor
#define heater_pin 23

//Variables
unsigned long startMillis;           //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 2500;   //the value is a number of milliseconds

int currentPage;                     //Current Page indicator  "1" First Page,  "2" Second Page
double setTemperature = 19.00;              //begining temp on boot in C

void setup(void) {

  Serial.begin(9600);
  Serial.println(F("WEATHER Station"));
  Serial.print("TFT size is "); 
  Serial.print(tft.width()); 
  Serial.print("x"); 
  Serial.println(tft.height());
  
  Serial.println(F("BMP280 test"));
  if (!bmp.begin()) {                      //Initialize BMP
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }
  Serial.println("Adafruit SI1145 test");

  if (! uv.begin()) {                     // Initialize Light sensor
    Serial.println("Didn't find Si1145");
    while (1);
  }

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* temperature oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  drawInitialScreen();

  currentPage = 1;         // First Page Default
  readSensors();           // Read Sensors
  startMillis = millis();  //initial start time

}

void loop() {

  //Start Timing to next temperature read
  currentMillis = millis();
  if (currentMillis - startMillis >= period)
  {
    readSensors();
    checkTemperature();
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current .
  }

  //TouchScreen Definition
  TSPoint p = ts.getPoint();
  //if sharing pins, you'll need to fix the directions of the touchscreen pins
  pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  pinMode(heater_pin, OUTPUT);
  //check for touch
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    Serial.print("PRESSURE: "); Serial.print(p.z);
    
    // scale from 0->1023 to tft.width
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = (tft.height() - map(p.y, TS_MINY, TS_MAXY, tft.height(), 0));

    //Print TouchScreen area seleted
    Serial.print("P X"); Serial.print(p.x);
    Serial.print("P Y"); Serial.print(p.y);

    if (p.x >= 15 && p.x <= 40 && p.y >= 130 && p.y <= 220 && currentPage == 1) { //check button for touch
      Serial.println("Next Page Selected");
      currentPage = 2;
      thermostatScreen();
      readSensors();            
    }
    else if (p.x >= 5 && p.x <= 30 && p.y >= 12 && p.y <= 55 && currentPage == 2) {   //check button for touch
      Serial.println("Next Page Selected");
      currentPage = 1;       // First Page Default
      drawInitialScreen();
      readSensors();     
    }
    else if (p.x >= 130 && p.x <= 230 && p.y >= 10 && p.y <= 50 && currentPage == 2) {    //check button for touch
      Serial.println("Minus");
      setTemperature--;
      readSensors();      
    }
    else if (p.x >= 130 && p.x <= 230 && p.y >= 180 && p.y <= 225 && currentPage == 2) {  //check button for touch
      Serial.println("Plus");
      setTemperature++;
      readSensors();     
    }
  }
}

void   drawInitialScreen() { //prints out the titles as static text

  tft.reset();
  getIdentifierScreen();
  tft.begin(identifier);
  tft.setRotation(3);
  tft.fillScreen(BLACK);

  tft.setCursor(30, 10);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.println("Weather");

  tft.setCursor(15, 40);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.println("Temperature:");

  tft.setCursor(15, 90);
  tft.println("Humidity:");

  tft.setCursor(15, 65);
  tft.println("Temperature: ");

  tft.setCursor(15, 115);
  tft.println("Pressure: ");

  tft.setCursor(15, 140);
  tft.println("IR Light: ");

  tft.setCursor(15, 165);
  tft.println("UV Index: ");

  //CLASSBUTTON[index].initButton( &tft, BUTON_X_pos, BUTTON_Y_pos, X_WIDTH, Y_height, BORDER_COLOR, TEXT_COLOR, BUTTON_COLOR, TEXT, FONT_SIZE );
  buttons.initButton( &tft, 245, 215, 110, 50, WHITE, WHITE, DARKGREY, "Thermo", 1 );
  buttons.drawButton(true);

}

void thermostatScreen() { // prints out the static text on the thermostat

  tft.reset();
  getIdentifierScreen();
  tft.begin(identifier);
  tft.setRotation(3);
  tft.fillScreen(BLACK);

  tft.setCursor(80, 10);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.println("Thermostat");

  tft.setCursor(75, 40);
  tft.setTextSize(2);
  tft.println("Set Temperature:");

  //CLASSBUTTON[index].initButton( &tft, BUTON_X_pos, BUTTON_Y_pos, X_WIDTH, Y_LARGE, BORDER_COLOR, TEXT_COLOR, BUTTON_COLOR, TEXT, FONT_SIZE );
  buttons.initButton( &tft, 50, 215, 110, 50, WHITE, WHITE, DARKGREY, "Weather", 1 );
  buttons.drawButton(true);

  buttons.initButton( &tft, 45, 100, 80, 80, WHITE, WHITE, DARKGREY, "-", 6 );
  buttons.drawButton(true);
  
  buttons.initButton( &tft, 280, 100, 80, 80, WHITE, WHITE, DARKGREY, "+", 6 );
  buttons.drawButton(true);
  //p.x >= 130 && p.x <= 230 && p.y >= 180 && p.y <= 225 && currentPage == 2
}


void getIdentifierScreen() { //identift the right screen

  identifier = tft.readID();
  if (identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if (identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if (identifier == 0x4535) {
    Serial.println(F("Found LGDP4535 LCD driver"));
  } else if (identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if (identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if (identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } else if (identifier == 0x0101)
  {
    identifier = 0x9341;
    Serial.println(F("Found 0x9341 LCD driver"));
  } else {
    identifier = 0x9341;
  }
}


void   readSensors() {
  DHT.read11(DHT11_PIN); 
  delay(250);
  checkTemperature();
  if (currentPage == 1) {
    tft.setTextColor(WHITE);
    tft.setTextSize(2);

    //First temperature line
    tft.fillRect(158, 35, 90, 22, BLACK); 
    tft.setCursor(160, 40);
    tft.print(bmp.readTemperature() - 2);
    tft.println(" C");

    //Second temperature line
    tft.fillRect(158, 65, 90, 22, BLACK);
    tft.setCursor(160, 65);
    tft.print(DHT.temperature);
    tft.println(" C");

    //Humidity line
    tft.fillRect(158, 90, 90, 22, BLACK);
    tft.setCursor(160, 90);
    tft.print(DHT.humidity);
    tft.println(" %");

    //Pressure line
    tft.fillRect(133, 115, 120, 22, BLACK);
    tft.setCursor(135, 115);
    tft.print(bmp.readPressure()/10);
    tft.println(" kPa");

    //IR light line
    tft.fillRect(133, 140, 80, 22, BLACK);
    tft.setCursor(135, 140);
    tft.print(uv.readIR()/1000);

    //UV Index line
    tft.fillRect(133, 165, 50, 22, BLACK);
    tft.setCursor(135, 165);
    tft.print(uv.readUV()/100);
    
  }
  else {
    tft.setTextColor(BLACK);
    tft.setTextSize(3);
    tft.drawRect(93, 75, 140, 60, WHITE);
    tft.fillRect(98, 80, 130, 50, CYAN);

   //Thermostat line
    tft.setCursor(103, 95);
    tft.print(setTemperature);
    tft.println(" C");
   
  }
}

void checkTemperature() {       //turn on a heater if the temperature falls to low
  double avgTemperature = ((bmp.readTemperature() - 2) + DHT.temperature) / 2;

  if (setTemperature > avgTemperature) {  //switch the ">" to "<" for an Air Conditioner
    digitalWrite(heater_pin, HIGH);
  }
  else {
    digitalWrite(heater_pin, LOW);
  }
}
