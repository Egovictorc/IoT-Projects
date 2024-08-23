#define dirPin 13
#define stepPin 14
// #define dirPin 2
// #define stepPin 3
#define stepsPerRevolution 1000
int Mode = 0;



void setup() {
  // put your setup code here, to run once:
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (Mode == 1) {
    ///Spin clockwise
    digitalWrite(dirPin, HIGH);
    for (int i = 0; i < stepsPerRevolution; i++) {
      digitalWrite(stepPin, HIGH);
      //delay(10000);
      delayMicroseconds(30);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(30);
      //delay(10000);
    }
  } else {
    digitalWrite(dirPin, LOW);
    for (int i = 0; i < stepsPerRevolution; i++) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(30);
      //delay(10000);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(30);
      //delay(10000);
    }
  }
}
