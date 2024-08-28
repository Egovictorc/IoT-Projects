#define stepPin 14
#define dirPin 13

void setup() {
  // put your setup code here, to run once:
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(dirPin, HIGH);
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(500);
    //digitalWrite(dirPin, HIGH);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(500);
}
