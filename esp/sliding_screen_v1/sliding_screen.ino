
#define stepPin 16  //Pull -ve
#define dirPin 17   //DIR -ve
#define stepsPerRevolution 10000
int mode = 1;
int pos = 0;
void setup() {
  // put your setup code here, to run once:
  // declare pins as output
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  //initialize serial
  Serial.begin(115200);
  Serial.println("--------- Initialization Completed ------------------ ");
}


void loop() {
  // put your main code here, to run repeatedly:
  //spin the stepper motor 1 revolution slowly
  Serial.println("--------------- Forward Direction --------------- ");

  if (mode == 1) {
  //set the spinning direction clockwise
  digitalWrite(dirPin, HIGH);
    for (pos = 0; pos < stepsPerRevolution; pos++) {
      //these four lines result in 1 step
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(30);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(30);
    }
    mode = 2;
  }


  delay(5000); // delay before changing direction
  Serial.println("--------------- Backward Direction ---------------- ");

  if (mode == 2) {
  //set the spinning direction clockwise
  digitalWrite(dirPin, LOW);
    for (pos = 0; pos < stepsPerRevolution; pos++) {
      //these four lines result in 1 step
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(30);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(30);
    }
    mode = 1;
  }

  delay(5000); // delay before changing direction
}
