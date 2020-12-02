int displayPin = 3;

void setup() {
  pinMode(displayPin,OUTPUT);
}

void loop() {
  int sensorValue = analogRead(A0);

  float voltage = sensorValue * (5.0 / 1023.0);

  Serial.println(voltage);
  
  if (voltage < 1.5) {
    analogWrite(displayPin,167);
  }
  
  else {analogWrite(displayPin,0);}
  delay(5);
}
