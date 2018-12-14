/*
 RC PulseIn Serial Read out
 By: Nick Poole
 SparkFun Electronics
 Date: 5
 License: CC-BY SA 3.0 - Creative commons share-alike 3.0
 use this code however you'd like, just keep this license and
 attribute. Let me know if you make hugely, awesome, great changes.
 */
 
int ch1; // Here's where we'll keep our channel values
int ch2;
int ch3;
int ch4;
int ch5;
int ch6;

void setup() {

pinMode(8, INPUT); // Set our input pins as such
pinMode(9, INPUT);
pinMode(10, INPUT);
pinMode(11, INPUT); 
pinMode(12, INPUT);
pinMode(13, INPUT);

  Serial.begin(9600); // Pour a bowl of Serial

}

void loop() {

  ch1 = pulseIn(2, HIGH, 25000); //Throttle
  ch2 = pulseIn(4, HIGH, 25000); //AILE
  ch3 = pulseIn(7, HIGH, 25000);//Elevator
  ch4 = pulseIn(8, HIGH, 25000);//Rudder
  ch5 = pulseIn(12, HIGH, 25000);//Gear
  ch6 = pulseIn(13, HIGH, 25000);//Auxilary 1
  Serial.print("Throttle: "); // Print the value of 
  Serial.print(ch1);        // each channel

  Serial.print("  AILE: ");
  Serial.print(ch2);

  Serial.print("  Elevator: ");
  Serial.print(ch3);

  Serial.print("  Rudder:");
  Serial.print(ch4);

  Serial.print("  Gear: ");
  Serial.print(ch5);

  Serial.print("  Auxilary 1:");
  Serial.println(ch6);
    
  delay(100); // I put this here just to make the terminal 
              // window happier
}
