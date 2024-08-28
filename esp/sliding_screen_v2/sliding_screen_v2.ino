// defines pins numbers
const int stepPin = 14; 
const int dirPin = 13; 
// const int enPin = 8;
void setup() {
  
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);

  // pinMode(enPin,OUTPUT);
  // digitalWrite(enPin,LOW);

  digitalWrite(dirPin, HIGH);
  
}

  int stepperDelay = 3000;

void loop() {

  // int stepperDelay = 3000;
  // for(int x = 0; x < 1300; x++) { //2300

  //   digitalWrite(stepPin, HIGH);
  //   delayMicroseconds(stepperDelay);
  //   digitalWrite(stepPin, LOW);
  //   delayMicroseconds(stepperDelay);

  //   // decrease stepper delay value by 1 microsecond
  //   // stepperDelay--;
  // }

  // run at constant velocity for 2000 steps
  for(int x = 0; x < 1500; x++) { //2000 //1000
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(3000); //700
    digitalWrite(stepPin, LOW);
    delayMicroseconds(3000);  //700
  }

  // delay(1000); // One second delay

  // digitalWrite(dirPin,LOW); //Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  // for(int x = 0; x < 800; x++) {
  //   digitalWrite(stepPin,HIGH);
  //   delayMicroseconds(500);
  //   digitalWrite(stepPin,LOW);
  //   delayMicroseconds(500);
  // }
  // delay(1000);
  
}
