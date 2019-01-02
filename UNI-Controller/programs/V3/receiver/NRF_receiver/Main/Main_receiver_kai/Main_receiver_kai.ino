/*
  combination of the controller project 
  DC transmit/receive test
  Written By: Jui-Wen, Yeh. 
  Under modification. 
  New version, dealt with DC first
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

//data part
byte result[8];
int throttle = 0 ;
int yaw = 0 ;
int pitch = 0 ;
int roll = 0 ;
int lftMode = 0 ;
int templftMode = 0; 
int rgtMode = 0 ;
int temprgtMode = 0;
int totalMode = 0 ;
int temptotalMode = 0;  
//
const int motorLF = A4; //left motor forward  if high
const int motorLB = A5; //left motor backward if high
int LPWM = 5; 
const int motorRF = 9; //right motor forward if high 
const int motorRB = 10; //right motoe backward if high
int RPWM = 6; 
const int motorUPU = A1; //upper motor control up 
const int motorUPD = A2; //upper motor control down

Servo sv1;
int pos1;  
Servo sv2;
int pos2; 
Servo sv3; 
int pos3 = 90;
int tempPos3 = 90;   
//

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
  pinMode (motorUPU, OUTPUT);
  pinMode (motorUPD, OUTPUT);
  
  sv1.attach(3);
  sv2.attach(4);
  sv3.attach(A0);
}

void loop() {
  if (radioControl.available(&pipe)) 
  {
    radioControl.read(&result, sizeof(result));  
    for (int cnt = 0; cnt < 7; cnt++)
    {
      if (cnt == 0)
        {
          lftMode = result[cnt];
          templftMode = lftMode; 
        }
      else if (cnt == 1)
        {
          rgtMode = result [cnt]; 
          temprgtMode = rgtMode; 
        }
      else if (cnt == 2)
        {
          totalMode = result [cnt];
          temptotalMode = totalMode;  
        }            
      else if (cnt == 3)
        throttle = result [cnt];
      else if (cnt == 4)
        yaw = result [cnt];      
      else if (cnt == 5)
        pitch = result [cnt];
      else if (cnt == 6)   
        roll = result [cnt];             
    }
  }
  else
  {
    throttle = 131 ;
    yaw = 129 ;
    pitch = 126 ;
    roll = 130 ;
    lftMode = templftMode ;
    rgtMode= temprgtMode ;
    pos3 = tempPos3; 
    totalMode = 0;  
    LPWM = 0; 
    RPWM = 0;    
    Serial.println("offline");
  }
//bounus
    Serial.print("left mode: ");
    Serial.print(lftMode); 
    Serial.print(" right mode: ");
    Serial.print(rgtMode); 
    Serial.print(" total mode: ");
    Serial.print(totalMode); 
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
/*
const int motorLF = A1; //left motor forward  if high
const int motorLB = A2; //left motor backward if high
int LPWM = 5; 
const int motorRF = 9; //right motor forward if high 
const int motorRB = 10; //right motoe backward if high
int RPWM = 6; 
*/  
//left case
int ltip = 1; 
  if (lftNum>0)
    {
      digitalWrite(motorLF, HIGH); 
      digitalWrite(motorLB, LOW); 
      if (abs(lftNum) >256)
        lftNum = 255;
      ltip = 2; 
    }
    else if (lftNum ==0)
    {
      digitalWrite(motorLF, LOW); 
      digitalWrite(motorLB, LOW); 
      ltip = 1; 
    }
    else if (lftNum <0)
    {
      digitalWrite(motorLF, LOW); 
      digitalWrite(motorLB, HIGH); 
      if (abs(lftNum) >256)
        lftNum = 255;  
        ltip = 0;      
    }
    
//right case  
  int rtip = 1;   
  if (rgtNum>0)
    {
      digitalWrite(motorRF, HIGH); 
      digitalWrite(motorRB, LOW); 
      if (abs(rgtNum) >256)
        rgtNum = 255; 
      rtip = 2;   
    }
    else if (rgtNum ==0)
    {
      digitalWrite(motorRF, LOW); 
      digitalWrite(motorRB, LOW); 
      rtip = 1; 
    }
    else if (rgtNum <0)
    {
      digitalWrite(motorRF, LOW); 
      digitalWrite(motorRB, HIGH); 
      if (abs(rgtNum) >256)
        rgtNum = 255; 
      rtip = 0;
    }
//
  Serial.print(" tip = ");
  Serial.print(ltip);
  Serial.print(rtip);
/*  Serial.print(" LPWM= "); 
  Serial.print(lftNum); 
  Serial.print(" RPWM= ");
  Serial.print(rgtNum); */
  analogWrite(LPWM, abs(lftNum));
  analogWrite(RPWM, abs(rgtNum));   
  //servo stop = 90
  //servo full fwd = 180
  //servo full reverse = 0;
  //first servo
    if (lftMode==2)
      {
      pos1 = 180;
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
      pos2 = 80;
      sv2.write(pos2);
      }
    else if (rgtMode==0)
      {
      pos2 = 90;
      sv2.write(pos2);   
      }
      
  //0~360
  //roll at right throttle
    roll-=0;
    if (roll<100)
      pos3--; 
    else if (roll >200)
      pos3++;
    else 
      ;
    //adjust the number: 
    if (pos3>180)
      pos3 = 180; 
    else if (pos3<0)
      pos3 = 0; 
    
    tempPos3 = pos3;   
  Serial.print(" tpos3 = "); 
  Serial.println(tempPos3);    
    sv3.write(pos3);
  //test servo situation: 
  /*  Serial.print("  ");
    Serial.print(pos1);
    Serial.print("  ");
    Serial.print(pos2);
    Serial.print("  ");
    Serial.println(pos3); */   
    if (totalMode ==0)
      {
        digitalWrite(motorUPU, LOW); 
        digitalWrite(motorUPD, LOW); 
      }
      else if (totalMode ==1)
      {
        digitalWrite(motorUPU, HIGH); 
        digitalWrite(motorUPD, LOW);         
      }
      else if (totalMode ==2)
      {
        digitalWrite(motorUPU, LOW); 
        digitalWrite(motorUPD, HIGH);          
      }
}
