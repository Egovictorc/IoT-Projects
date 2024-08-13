#include <ESP32Servo.h>

Servo myServo;
int servoPin = 18;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  myServo.attach(servoPin);
  myServo.write(0);
}

void loop() {
  // put your main code here, to run repeatedly:

  if(Serial.available()) {
    String cmd = Serial.readString();
    int angle = Serial.parseInt(cmd);
    myServo.write(angle);
  }

  delay(200);
}
