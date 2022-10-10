void setup(){
  Serial.begin(38400);
  Serial.print("Hello from arduino!");
  Serial1.begin(38400);
}

void loop(){
  if (Serial.available()) {
    Serial1.write(Serial.read());
  }

  if (Serial1.available()) {
    Serial.write(Serial1.read());
  }
}