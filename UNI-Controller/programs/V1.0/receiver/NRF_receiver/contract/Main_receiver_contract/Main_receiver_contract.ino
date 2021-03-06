/*
  combination of the controller project 
  receiver Part
  Written By: Jui-Wen, Yeh. 
  Under modification. 
*/

//preinitialize
const byte addr[] = { 0xCC,0xCE,0xCC,0xCE,0xCC };
#include <SPI.h>
#include <Wire.h>
#include <Servo.h>




#include "RF24.h"
RF24 radioControl(9, 10); // CE pin, CSN pin
const byte pipe = 1; // assigned pipe number
//rf24 receiver part



//data part
byte result[8];
int throttle = 0 ;
int yaw = 0 ;
int pitch = 0 ;
int roll = 0 ;
int lftMode = 0 ;
int rgtMode= 0 ;
//

//motor part
const int motorLF = 2; //left motor forward  if high
const int motorLB = 3; //left motor backward if high
const int motorRF = 6; //right motor forward if high 
const int motorRB = 7; //right motor backward if high

Servo sv1;
int pos1;  
Servo sv2;
int pos2;  
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
  pinMode (motorLF, OUTPUT);
  pinMode (motorLB, OUTPUT);
  pinMode (motorRF, OUTPUT);
  pinMode (motorRB, OUTPUT);
  sv1.attach(4);
  sv2.attach(5);
  
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
  }
//debug check    
    Serial.print("mode: ");
    //Serial.print(throttle); 
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

//output into the motor
  if(throttle>128)
  {
    int lft_output = throttle+128; 
    analogWrite(motorLF, lft_output);
    analogWrite(motorLB, LOW);
    analogWrite(motorRF, 200);
    analogWrite(motorRB, LOW);
  }
//servo stop = 90
//servo full fwd = 180
//servo full reverse = 0;
  pos1 = yaw*180/256; 
  sv1.write(pos1);
  pos1 = pitch*180/256;   
  sv2.write(pos2);



}
