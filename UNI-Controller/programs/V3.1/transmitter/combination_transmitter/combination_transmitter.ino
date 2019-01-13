/*
  combination of the controller project 
  Transmitter Part
  Written By: Jui-Wen, Yeh. 
  Under modification. 
*/
#include <SPI.h>
#include "RF24.h"
RF24 rf24(9, 10); // CE腳, CSN腳

const byte addr[] = { 0xCC,0xCE,0xCC,0xCE,0xCC };
char msg[32] = "happy hacking";
String incomingByte;

int ModeCounter; 
byte output[8];
int lftMode, rgtMode, totalMode, throttle, yaw, pitch, roll;
const int lftPositive = 2; 
const int lftNegative = 3; 
const int totalModePin = 8; 
const int rgtPositive = A6; 
const int rgtNegative = A7; 
const int led[] = {4, 5, 6, 7};  
/*
4 for lft fwd, 5 for lft bkwd, 4+5 for not rolling;
6 for lft fwd, 7 for lft bkwd, 4+5 for not rolling;
 */
void setup() {
  //for checking
  Serial.begin(9600);
  //setting up radio
  rf24.begin();
  rf24.setChannel(15);       // 設定頻道編號
  rf24.openWritingPipe(addr); // 設定通道位址
  rf24.setPALevel(RF24_PA_MIN);   // 設定廣播功率
  rf24.setDataRate(RF24_250KBPS); // 設定傳輸速率
  rf24.stopListening();       // 停止偵聽；設定成發射模式
  Serial.println(msg);  
  //setting up lftSwitch
  pinMode(lftPositive, INPUT_PULLUP);
  //setting up rgtSwitch
  pinMode(lftNegative, INPUT_PULLUP);  
  //setting up last button 
  pinMode(totalModePin, INPUT_PULLUP); 
  
  pinMode(rgtPositive, INPUT_PULLUP);
//  digitalWrite(rgtPositive, HIGH);
  pinMode(rgtNegative, INPUT_PULLUP);  
//  digitalWrite(rgtNegative, HIGH); 
   
  //setting instruction light
  for(int i=0;i<4;i++)
  {
    pinMode(led[i],OUTPUT); 
  }   
}

void loop() {
  /*
  Part 1 reading throttle
  */
  if (digitalRead(lftNegative)==LOW)
    lftMode = 2; 
  else if (digitalRead(lftPositive) == LOW)
    lftMode = 1; 
  else
    lftMode = 0;
//
  if (analogRead(rgtNegative)==LOW)
    rgtMode = 2; 
  else if (analogRead(rgtPositive) == LOW)
    rgtMode = 1; 
  else
    rgtMode = 0;

  if (digitalRead(totalModePin)==LOW)
    {
      delay(50);
      if (digitalRead(totalModePin)==LOW)
        ModeCounter+=1; 
    }


  if (ModeCounter%4==1)
    totalMode = 1; 
  else if (ModeCounter%4==2)
    totalMode = 0; 
  else if (ModeCounter%4==3)
    totalMode = 2;     
  else
    totalMode = 0; 
    
  if (ModeCounter==100)
    ModeCounter = 0; 
  
  throttle = map(analogRead(A2), 0, 1024, 0, 256);
  yaw      = map(analogRead(A3), 0, 1024, 0, 256);
  pitch    = map(analogRead(A0), 0, 1024, 0, 256);
  roll     = map(analogRead(A1), 0, 1024, 0, 256);

  for (int num = 0; num < 7; num++)
  {
    if (num==0)
      output[num] = lftMode; 
    else if (num==1)
      output[num]=rgtMode;
    else if (num==2)
      output[num]=totalMode; 
    else if (num==3)
      output[num] = throttle;
    else if (num==4) 
      output[num] = yaw;
    else if (num==5)
      output[num] = pitch;
    else if (num==6)    
      output[num] = roll;
  };
  
  /*
  Part 2 Writing the throttle and transmit
  */
  Serial.print("lftMode: ");
  Serial.print(output[0]); 
  Serial.print("   rgtMode: ");
  Serial.print(output[1]);    
  Serial.print("   totalMode: ");
  Serial.print(output[2]);     
  Serial.print("   Throttle: ");
  Serial.print(output[3]);  
  Serial.print("   Yaw: ");
  Serial.print(output[4]);   
  Serial.print("   Pitch: ");
  Serial.print(output[5]);   
  Serial.print("   Roll: ");
  Serial.print(output[6]);
  Serial.print("  t.Size: ");
  Serial.println(sizeof(output));   
  rf24.write(&output, sizeof(output));  // 傳送資料

/*
Part 3, light signal
*/
  if (lftMode==0)
    {
      digitalWrite(led[0], LOW);
      digitalWrite(led[1], LOW);
    }
  else if (lftMode==1)
    {  
      digitalWrite(led[0], LOW);
      digitalWrite(led[1], HIGH);
    }
  else if (lftMode==2)
    {
    digitalWrite(led[0], HIGH);
    digitalWrite(led[1], LOW);    
    }
///rightMode    
  if (rgtMode==0)
    {
      digitalWrite(led[2], LOW);
      digitalWrite(led[3], LOW);
    }
  else if (rgtMode==1)
    {  
      digitalWrite(led[2], LOW);      
      digitalWrite(led[3], HIGH);
    }
  else if (rgtMode==2)
    {
    digitalWrite(led[2], HIGH);      
    digitalWrite(led[3], LOW); 
    } 
}
