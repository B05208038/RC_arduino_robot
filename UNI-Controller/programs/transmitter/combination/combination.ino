//combination of the controller project
#include <SPI.h>
#include "RF24.h"
RF24 rf24(9, 10); // CE腳, CSN腳

const byte addr[] = { 0xCC,0xCE,0xCC,0xCE,0xCC };
char msg[32] = "happy hacking";
String incomingByte;

byte output[8];
int mode, throttle, yaw, pitch, roll;
const int positive = 2; 
const int negative = 3; 
const int led[] = {4, 5, 6 };  

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
  //setting up switch
  pinMode(positive, INPUT);
  digitalWrite(positive, HIGH);
  pinMode(negative, INPUT);  
  digitalWrite(negative, HIGH);
  //setting instruction light
  for(int i=0;i<3;i++)
  {
    pinMode(led[i],OUTPUT); 
  }   
}

void loop() {
  /*
  Part 1 reading throttle
  */
  if (digitalRead(negative)==LOW)
    mode = 2; 
  else if (digitalRead(positive) == LOW)
    mode = 1; 
  else
    mode = 0;
  
  throttle = map(analogRead(A0), 0, 1024, 0, 256);
  yaw      = map(analogRead(A1), 0, 1024, 0, 256);
  pitch    = map(analogRead(A2), 0, 1024, 0, 256);
  roll     = map(analogRead(A3), 0, 1024, 0, 256);

  for (int num = 0; num < 5; num++)
  {
    if (num==0)
      output[num] = mode; 
    else if (num==1)
      output[num] = throttle;
    else if (num==2) 
      output[num] = yaw;
    else if (num==3)
      output[num] = pitch;
    else if (num==4)    
      output[num] = roll;
  };
  
  /*
  Part 2 Writing the throttle and transmit
  */
  Serial.print("mode: ");
  //Serial.print(throttle);
  Serial.print(output[0]);     
  Serial.print("   Throttle: ");
  //Serial.print(throttle);
  Serial.print(output[1]);  
  Serial.print("   Yaw: ");
  //Serial.print(yaw);
  Serial.print(output[2]);   
  Serial.print("   Pitch: ");
  //Serial.print(pitch);
  Serial.print(output[3]);   
  Serial.print("   Roll: ");
  //Serial.println(roll);
  Serial.print(output[4]);
  Serial.print("  t.Size: ");
  Serial.println(sizeof(output));   
  rf24.write(&output, sizeof(output));  // 傳送資料

/*
Part 3, light signal
*/
  if (mode==0)
    {
      digitalWrite(led[0], HIGH);
      digitalWrite(led[1], LOW);
      digitalWrite(led[2], LOW);
    }
  else if (mode==1)
    {  
      digitalWrite(led[0], LOW);
      digitalWrite(led[1], HIGH);
      digitalWrite(led[2], LOW);
    }
  else if (mode==2)
    {
    digitalWrite(led[0], LOW);
    digitalWrite(led[1], LOW);
    digitalWrite(led[2], HIGH);    
    }
}
