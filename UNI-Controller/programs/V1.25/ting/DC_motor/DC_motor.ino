const int motorLF = 5; //left motor forward  if high
const int motorLB = 6; //left motor backward if high
const int motorRF = 9; //right motor forward if high 
const int motorRB = 10; //right motoe backward if high

void setup() {
  pinMode (motorLF, OUTPUT);
  pinMode (motorLB, OUTPUT);
  pinMode (motorRF, OUTPUT);
  pinMode (motorRB, OUTPUT);

}

void loop() {
//bounus
// 
  analogWrite(motorLF, 0);
  analogWrite(motorLB, 100);  
  analogWrite(motorRF, 0);
  analogWrite(motorRB, 100);

}
