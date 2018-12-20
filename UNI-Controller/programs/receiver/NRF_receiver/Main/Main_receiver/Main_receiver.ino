/*
  combination of the controller project 
  receiver Part
  Written By: Jui-Wen, Yeh. 
  Under modification. 
*/

//for DC motor signal input, L298N H-bridge
int lftFwd = 0; 
int lftBkd = 0; 
int rgtFwd = 0; 
int rgtBkd = 0; 

//preinitialize
const byte addr[] = { 0xCC,0xCE,0xCC,0xCE,0xCC };
#include <SPI.h>
#include <Wire.h>
#include <Servo.h>
#include "RF24.h"
RF24 radioControl(7, 8); // CE pin, CSN pin
const byte pipe = 1; // assigned pipe number
//rf24 receiver part

//dealt with package lost 
float pkglr; //package lost rate
int pkgt; //package transport

//data part
byte result[8];
int throttle = 0 ;
int yaw = 0 ;
int pitch = 0 ;
int roll = 0 ;
int lftMode = 0 ;
int rgtMode= 0 ;
//

//motor function
int lftFwdOuput = 0; 
int lftBkdOuput = 0;    
int rgtFwdOuput = 0; 
int rgtBkdOuput = 0;
//

//motor part
const int lftFwdPos = 5; 
const int lftBkdPos = 6; 
const int rgtFwdPos = 9; 
const int rgtBkdPos = 10; 

Servo sv1;
int pos1;  
Servo sv2;
int pos2; 
Servo sv3; 
int pos3;  
//

void setup()   {                
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
  //motor section
  pinMode (5, OUTPUT);
  pinMode (6, OUTPUT);
  pinMode (9, OUTPUT);
  pinMode (10, OUTPUT);
  sv1.attach(3);
  sv2.attach(4);
  sv3.attach(A0);
  
}


void loop() {
      // send data only when you receive data:
  if (radioControl.available(&pipe)) 
  {
    pkgt+=1; 
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
    pkglr = 1/pkgt; 
    pkgt=0; 
    Serial.println("package lost rate: ");
    Serial.println(pkglr);
    //restart radio
    radioControl.begin();
    radioControl.setChannel(15); //setting up channel(frequency)
    radioControl.setPALevel(RF24_PA_MIN);
    radioControl.setDataRate(RF24_250KBPS);
    radioControl.openReadingPipe(pipe, addr); //open up radio
    radioControl.startListening(); //start listening
        
  }
    //debug check   
    //for servo     
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
/*    Serial.print("  t.Size: ");
    Serial.print(sizeof(result)); */  

  /*
  output into the motor
  training the data
  assume everything original = 128
  DC motor control by h-bridge, 
  PWM only for D3, 5, 6, 9, 10, 11
  take 5, 6 for left, 10, 11 for right
  lftFwdPos, lftBkdPos, rgtFwdPos, rgtBkdPos
  lftFwd,    lftBkd,    rgtFwd,    rgtBkd 
  affected by throttle, forward and backward, 
              yaw,      leftward and rightward. 
  have to adjust the rolling way by yourself, especially the formula for the input. 
  */
  int lftNum = (throttle+yaw-256)*sqrt(2); 
  int rgtNum = (throttle-yaw)*sqrt(2);
  lftNum-=5; 
  rgtNum-=2;
  if (lftNum>0)
  {
    lftFwd = lftNum; 
    lftBkd = LOW;   
  }
  else
  {
    lftFwd = LOW; 
    lftBkd = 0-lftNum;       
  }
  if (rgtNum>0)
  {
    rgtFwd = rgtNum; 
    rgtBkd = LOW;   
  }
  else
  {
    rgtFwd = LOW; 
    rgtBkd = 0-rgtNum;       
  }
  Serial.print(" lftFwd= "); 
  Serial.print(lftFwd); 
  Serial.print(" Bkd= ");
  Serial.print(lftBkd); 
  Serial.print("  rgtFwd= "); 
  Serial.print(rgtFwd);
  Serial.print("  Bkd= ");       
  Serial.println(rgtBkd);  
  analogWrite(5, lftFwd); 
  analogWrite(6, lftBkd); 
  analogWrite(9, rgtFwd); 
  analogWrite(10, rgtBkd); 

//servo stop = 90
//servo full fwd = 180
//servo full reverse = 0;
//first servo
  if (lftMode==2)
    {
    pos1 = 100;
    sv1.write(pos1);
    }
  else if (lftMode==1)
    {
    pos1 = 0;
    sv1.write(pos1);
    }
  else if (lftMode==0)
    {
    pos1 = 90;
    sv1.write(pos1);   
    }
//another servo          
  if (rgtMode==2)
    {
    pos2 = 100;
    sv2.write(pos2);
    }
  else if (rgtMode==1)
    {
    pos2 = 0;
    sv2.write(pos2);
    }
  else if (rgtMode==0)
    {
    pos2 = 90;
    sv2.write(pos2);   
    }
//0~360
//roll at right throttle
  pos3 = map(roll, 0, 255, 180, 0);
  sv3.write(pos3);
//test servo situation: 
/*  Serial.print("  ");
  Serial.print(pos1);
  Serial.print("  ");
  Serial.print(pos2);
  Serial.print("  ");
  Serial.println(pos3); */   
}
