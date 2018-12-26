/*
A sketch for checking the values of each joystick.
*/

void setup() {
  Serial.begin(9600);
}

void loop() {
  int throttle = analogRead(A0);
  int yaw      = analogRead(A1);
  int pitch    = analogRead(A2);
  int roll     = analogRead(A3);

  Serial.print("Throttle: ");
  Serial.print(throttle);
  Serial.print("      Yaw: ");
  Serial.print(yaw);
  Serial.print("      Pitch: ");
  Serial.print(pitch);
  Serial.print("      Roll: ");
  Serial.println(roll);

  delay(20);
}
