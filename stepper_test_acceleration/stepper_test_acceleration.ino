// defines pins numbers
const int stepPin = 14;
const int dirPin = 13;
int stepsPerRevolution = 1000;
#define ledPin 5

double initialPosition =  0.000000;
// const int enPin = 8;
void setup() {
  Serial.begin(115200);
  // Sets the two pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  pinMode(ledPin, OUTPUT);
  // pinMode(enPin,OUTPUT);
  // digitalWrite(enPin,LOW);

  // digitalWrite(dirPin, HIGH);
  digitalWrite(dirPin, LOW);
}

int stepperDelay = 6000;
void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(ledPin) == 0) {
    moveForward();
  } else {

    moveBackward();
  }
  //  delay(1000);  // Wait for 1 second before repeating or switching directions
}


void move(int stepperDelay) {
  for (int x = 0; x < stepsPerRevolution; x++) {  //2000 //1000
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepperDelay);  //700
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepperDelay / 2);  //700
    // delayMicroseconds(stepperDelay / 2);  //700
    Serial.println(initialPosition);

    if(stepperDelay >= 4500) {
      stepperDelay -= 100;
    } 
  }
}

void moveForward() {
  // int forwardDelay = 3000;
  digitalWrite(dirPin, HIGH);

  move(stepperDelay);
}

void moveBackward() {
  // int forwardDelay = 3000;

  digitalWrite(dirPin, LOW);
  move(stepperDelay);

  // for (int x = 0; x < 1000; x++) {  //2000 //1000
  //   digitalWrite(stepPin, HIGH);
  //   // delayMicroseconds(stepperDelay * 2);  //700
  //   delayMicroseconds(stepperDelay * 2);  //700
  //   digitalWrite(stepPin, LOW);
  //   // delayMicroseconds(3000);  //700
  //   delayMicroseconds(stepperDelay / 2);  //700

  //   // delayMicroseconds(stepperDelay);  //700
  // }
}