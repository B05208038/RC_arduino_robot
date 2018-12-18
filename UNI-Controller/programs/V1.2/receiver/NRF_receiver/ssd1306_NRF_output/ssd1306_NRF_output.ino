/*
  combination of the receiver project 
  Written By: Jui-Wen, Yeh. 
  Under modification. 
*/
//preinitialize
const byte addr[] = { 0xCC,0xCE,0xCC,0xCE,0xCC };
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
//char incomingByte[32];
//ssd1306s above//////////////

#include "RF24.h"
RF24 radioControl(7, 8); // CE pin, CSN pin
const byte pipe = 1; // assigned pipe number
//rf24 receiver part

//data part
byte result[8];
int throttle = 0 ;
int yaw = 0 ;
int pitch = 0 ;
int roll = 0 ;
int lftMode = 0 ;
int rgtMode = 0 ;
//

void setup()   {                
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  //check for displayer -- init done
  display.clearDisplay();  
//  display.drawBitmap(0, 0,  Drone_Photo_bmp, 128, 64, 1);  
  display.display();
  delay(1000);
  display.clearDisplay();  
  //RF24 below
  radioControl.begin();
  radioControl.setChannel(15); //setting up channel(frequency)
  radioControl.setPALevel(RF24_PA_MIN);
  radioControl.setDataRate(RF24_250KBPS);
  radioControl.openReadingPipe(pipe, addr); //open up radio
  radioControl.startListening(); //start listening
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,15);
  display.println("Control Radio Ready! ");
  display.display();
  delay(1000);
  display.clearDisplay();  
//  
  // Writer name
  display.setTextSize(1.5);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Made by: ");
  display.setTextColor(BLACK, WHITE); // 'inverted' text
//
  display.setCursor(0,15);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.println("Hientz Ben"); 
  display.display();
  delay(2000);
  display.clearDisplay();
//
  display.setCursor(0,25);
  display.setTextSize(1.5);
  display.setTextColor(WHITE);
  display.println("please open the controller "); 
  display.display();
  display.clearDisplay();
}


void loop() {
      // send data only when you receive data:
  if (radioControl.available(&pipe)) {
    radioControl.read(&result, sizeof(result));  
  for (int cnt = 0; cnt < 6; cnt++)
    {
      if (cnt == 0)
        lftMode = result[cnt];
      else if (cnt == 1)
        rgtMode = result [cnt];        
      else if (cnt == 2)
        throttle = result [cnt];
      else if (cnt == 3)
        yaw = result [cnt];      
      else if (cnt == 4)
        pitch = result [cnt];
      else if (cnt == 5)   
        roll = result [cnt];             
    }
    Serial.print("lftMode: ");
    Serial.print(lftMode); 
    Serial.print("  rgtMode: ");
    Serial.print(rgtMode);    
    Serial.print("  Throttle: ");
    Serial.print(throttle);  
    Serial.print("  Yaw: ");
    Serial.print(yaw);
    Serial.print("  Pitch: ");
    Serial.print(pitch); 
    Serial.print("  Roll: ");
    Serial.print(roll);
    Serial.print("  t.Size: ");
    Serial.println(sizeof(result));   
  
         
    display.clearDisplay();  
    display.setCursor(0,0);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.print("lftMode: "); 
    display.println(lftMode); 
    display.print("rgtMode: "); 
    display.println(rgtMode); 
    display.setTextSize(1);
    display.print(" Thr: "); 
    display.print(throttle); 
    display.print(" Yaw: "); 
    display.println(yaw);
    display.print(" Pit: "); 
    display.print(pitch); 
    display.print(" rol: "); 
    display.println(roll);               
    display.display(); 
    }
    else
    {
        display.clearDisplay(); 
        display.setCursor(0,25);
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.println("please open the controller "); 
        display.display();    
    }
    delay(25);
}
