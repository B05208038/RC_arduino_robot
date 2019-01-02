/*
  combination of the controller project 
  DC transmit/receive test
  Written By: Jui-Wen, Yeh. 
  Under modification. 
*/
//preinitialize
const byte addr[] = { 0xCC,0xCE,0xCC,0xCE,0xCC };
#include <SPI.h>
#include <Wire.h>
#include <Servo.h>
#include "RF24.h"
RF24 radioControl(7, 8); // CE pin, CSN pin
const byte pipe = 1; // assigned pipe number
//rf24 receiver part

//for DC motor signal input, L298N H-bridge
int lftFwd = 0; 
int lftBkd = 0; 
int rgtFwd = 100; 
int rgtBkd = 0; 

//data part
byte result[8];
int throttle = 0 ;
int yaw = 0 ;
int pitch = 0 ;
int roll = 0 ;
int lftMode = 0 ;
int rgtMode= 0 ;
//
const int motorLF = 5; //left motor forward  if high
const int motorLB = 6; //left motor backward if high
const int motorRF = 9; //right motor forward if high 
const int motorRB = 10; //right motoe backward if high

void setup() {
  Serial.begin(9600);
  //RF24 below
  radioControl.begin();
  radioControl.setChannel(15); //setting up channel(frequency)
  radioControl.setPALevel(RF24_PA_MIN);
  radioControl.setDataRate(RF24_250KBPS);
  radioControl.openReadingPipe(pipe, addr); //open up radio
  radioControl.startListening(); //start listening
  delay(1000);
  Serial.println("NRF ready~");
//  
  pinMode (motorLF, OUTPUT);
  pinMode (motorLB, OUTPUT);
  pinMode (motorRF, OUTPUT);
  pinMode (motorRB, OUTPUT);

}

void loop() {
  if (radioControl.available(&pipe)) 
  {
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
  }
  else
  {
    throttle = 128 ;
    yaw = 128 ;
    pitch = 128 ;
    roll = 128 ;
    lftMode = 0 ;
    rgtMode= 0 ;   
  }
//bounus
    Serial.print("left mode: ");
    Serial.print(lftMode); 
    Serial.print(" right mode: ");
    Serial.print(rgtMode); 
    //for DC motor  
    Serial.print("  Throttle: ");
    Serial.print(throttle);  
    Serial.print("  Yaw: ");
    Serial.print(yaw);
    Serial.print("  Pitch: ");
    Serial.print(pitch); 
    Serial.print("  Roll: ");
    Serial.print(roll);
// DC motor rotation
  throttle-=131; 
  yaw-=129;
  int lftNum = (throttle+yaw)*2.2; 
  int rgtNum = (throttle-yaw)*2.2;
//
  if (lftNum>0)
    {
      lftFwd = lftNum; 
      lftBkd = LOW; 
      if (lftFwd >256)
        lftFwd = 255;
    }
    else if (lftNum ==0)
    {
      lftFwd = 0; 
      lftBkd = 0;
    }
    else if (lftNum <0)
    {
      lftFwd = LOW; 
      lftBkd = -lftNum;
      if (lftBkd >256)
        lftBkd = 255;        
    }
//right case    
  if (rgtNum>0)
    {
      rgtFwd = rgtNum; 
      rgtBkd = LOW; 
      if (rgtFwd >256)
        rgtFwd = 255; 
    }
    else if (rgtNum ==0)
    {
      rgtFwd = 0; 
      rgtBkd = 0;
    }
    else if (rgtNum <0)
    {
      rgtFwd = LOW; 
      rgtBkd = -rgtNum;
      if (rgtBkd >256)
        rgtBkd = 255; 
    }
//
  Serial.print(" lftFwd= "); 
  Serial.print(lftFwd); 
  Serial.print(" Bkd= ");
  Serial.print(lftBkd); 
  Serial.print("  rgtFwd= "); 
  Serial.print(rgtFwd);
  Serial.print("  Bkd= ");       
  Serial.println(rgtBkd); 
  analogWrite(motorLF, lftFwd);
  analogWrite(motorLB, lftBkd);  
  analogWrite(motorRF, rgtFwd);
  analogWrite(motorRB, rgtBkd);

}
