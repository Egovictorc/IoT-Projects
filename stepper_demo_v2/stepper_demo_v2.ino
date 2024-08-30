const int stepPin = 14; 
const int dirPin = 13; 
const int enPin = 8;  // Uncomment if you need to use the enable pin

void setup() {
  // Set the pin modes
  pinMode(stepPin, OUTPUT); 
  pinMode(dirPin, OUTPUT);
  pinMode(enPin, OUTPUT);  // Uncomment if you need to use the enable pin

  digitalWrite(enPin, LOW);  // Enable the stepper driver
}

void loop() {
  moveForwardWithAcceleration();  // Move the motor forward with acceleration
  delay(1000);  // Wait for 1 second
  moveBackwardWithAcceleration();  // Move the motor backward with acceleration
  delay(1000);  // Wait for 1 second
}

void moveForwardWithAcceleration() {
  digitalWrite(dirPin, HIGH);  // Set direction to forward
  accelerateMotor();
}

void moveBackwardWithAcceleration() {
  digitalWrite(dirPin, LOW);  // Set direction to backward
  accelerateMotor();
}

void accelerateMotor() {
  int stepperDelay = 3000;  // Start with a higher delay (slower speed)
  int minDelay = 500;  // Minimum delay to prevent going too fast and skipping steps
  int stepDecrease = 10;  // Amount to decrease delay per step for acceleration

  // Run the stepper motor with acceleration
  for (int x = 0; x < 1500; x++) {  // Number of steps to move
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepperDelay);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepperDelay);

    // Gradually decrease the delay for acceleration
    if (stepperDelay > minDelay) {
      stepperDelay -= stepDecrease;
    }
  }
}
