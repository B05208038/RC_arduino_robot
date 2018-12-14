/*
 RC PulseIn Joystick
 By: Nick Poole
 SparkFun Electronics
 Date: 5
 License: CC-BY SA 3.0 - Creative commons share-alike 3.0
 use this code however you'd like, just keep this license and
 attribute. Let me know if you make hugely, awesome, great changes.
 modified by Jui-Wen,Yeh
 https://www.sparkfun.com/tutorials/348
 remember to bind everytime to make it stable.  
 If there are some disturbance on the signal part: http://www.efly98.com/cdb_efly98/viewthread.php?tid=6665
 */
#include <Servo.h>

// Here's where we'll keep our channel values
int ch1; 
int ch2;
int ch3;
int ch4;
int ch5;
int ch6;

//input number modification
int throttle = 0 ;
int temp_throttle = 0;
int yaw = 0 ;
int pitch = 0 ;
int roll = 0 ;
int temp_roll = 0; 
int lftMode = 0 ;
int rgtMode= 0 ;

//for DC motor signal input, L298N H-bridge
int lftFwd = 0; 
int lftBkd = 0; 
int rgtFwd = 0; 
int rgtBkd = 0; 

//for H-bridge wiring 
const int lftFwdPos = 5; 
const int lftBkdPos = 6; 
const int rgtFwdPos = 10; 
const int rgtBkdPos = 11; 

//for servo motor input. 
Servo servo1; 
Servo servo2; 

void setup() {
  pinMode(8, INPUT); // Set our input pins as such
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT); // Set our input pins as such
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  Serial.begin(9600); // Pour a bowl of Serial
  servo1.attach(3); 
  servo2.attach(9);
}

void loop() {
  
  ch1 = pulseIn(2, HIGH, 25000); // Read the pulse width of 
  ch2 = pulseIn(4, HIGH, 25000); // each channel
  ch3 = pulseIn(7, HIGH, 25000);
  ch4 = pulseIn(8, HIGH, 25000); 
  ch5 = pulseIn(12, HIGH, 25000); 
  ch6 = pulseIn(13, HIGH, 25000);

  /*
  adjust the position by using mapping. 
  map(channel, input_min, input_max, output_min, output_max)
  need to use input_range_test.ino to find the pulse input for your controller

  */ 
  //Lower resolution to make it easier to control
  lftMode = ch5; //==flight Mode
  yaw = map(ch4, 1000,2000,-256,256); //==rudder
  throttle = map(ch1, 1000,2000,-256,256); 
///////
  if (throttle<-256)
  ;
  else
    temp_throttle = throttle; 
///////    
  pitch = map(ch3, 1000,2000,-256,256); //==elevator
  roll = map(ch2, 1000,2000,-256,256); //==aile
///////
  if (roll<-256)
  ;
  else
    temp_roll = roll; 
////////    
  //rgtMode = ch2; 
   
/*  if(lftMode>1600)
  {
    Serial.print("Left Switch: case 1  ");
  } 
  else if(lftMode<1200)
  {
    Serial.print("Left Switch: case 3  ");
  }
  else
  {
    Serial.print("Left Switch: case 2  ");
  }
//  if(rgtMode>1000){Serial.println("Left Switch: Engaged");} 
//  if(rgtMode<1000){Serial.println("Left Switch: Disengaged");}  
  Serial.print("left Stick X:"); // Ch3 was x-axis 
  Serial.print(yaw); // center at 0
//fixing throttle data bug issue: 
  if (throttle<-256)
    throttle = temp_throttle; 
//
  Serial.print("  left Stick Y:"); 
  Serial.print(throttle); // center at 0
*/  Serial.print("  Right Stick X:");  
  Serial.print(pitch); // center at 0
  Serial.print("  Right Stick Y:"); 
  Serial.print(roll); // center at 0
//fixing roll data bug issue: 
  if (roll<-256)
    roll = temp_roll; 
//
  /*
  Servo motor part. 
  Giving 90 for not turning the motor, 180 for turning clockwise, 0 for turniing counter-clockwise. 
  The range against 90 will determine it's speed. 
  Take the servo example for reference
  take 3 for servo1, 9 for servo2
  servo2 = 360 deg
  adjust the number by what it times, theoretically it's 0.3515625 = 180/512
  */
  int servo1_input = (pitch+180)*.55;
  if (abs(servo1_input-90)<5)
    servo1_input = 90;
  servo1.write(servo1_input); 
  Serial.print("  servo1_input = "); 
  Serial.print(servo1_input);
  int servo2_input = (256-roll)*.30; 
  if (abs(servo2_input-90)<5)
    servo2_input = 90;
  servo2.write(servo2_input);   
  Serial.print("  servo2_input = "); 
  Serial.println(servo2_input);
  /*
  DC motor control by h-bridge, 
  PWM only for D3, 5, 6, 9, 10, 11
  take 5, 6 for left, 10, 11 for right
  lftFwdPos, lftBkdPos, rgtFwdPos, rgtBkdPos
  lftFwd,    lftBkd,    rgtFwd,    rgtBkd 
  affected by throttle, forward and backward, 
              yaw,      leftward and rightward. 
  have to adjust the rolling way by yourself, especially the formula for the input. 
  */

/* 
  lftFwd = throttle; 
  lftBkd
  rgtFwd 
  rgtBkd
*/
/*  Serial.print(" // lftFwd = "); 
  Serial.print(lftFwd); 
  Serial.print("  lftBkd = ");
  Serial.print(lftBkd); 
  Serial.print("  rgtFwd = "); 
  Serial.print(rgtFwd);
  Serial.print("  rgtBkd = ");       
  Serial.println(rgtBkd); 
  Serial.println();  */
  analogWrite(lftFwd, lftFwdPos); 
  analogWrite(lftBkd, lftBkdPos); 
  analogWrite(rgtFwd, rgtFwdPos); 
  analogWrite(rgtBkd, rgtBkdPos); 
  delay(5);
}
