#include <AccelStepper.h>

// Define motor interface type
#define MotorInterfaceType 1 // 1 means Driver type (DIR, STEP)

// Define motor connections and motor interface type
#define stepPin 14   // STEP pin
#define dirPin 13    // DIR pin

// Create an instance of AccelStepper
AccelStepper stepper(MotorInterfaceType, stepPin, dirPin);

void setup() {
  // Set maximum speed and acceleration
  stepper.setMaxSpeed(1000);      // Maximum speed (steps per second)
  stepper.setAcceleration(500);   // Acceleration (steps per second^2)

  // Set the initial target position (number of steps to move)
  stepper.moveTo(2000);  // Move 2000 steps forward
}

void loop() {
  // If the motor reached the target position, reverse direction
  if (stepper.distanceToGo() == 0) {
    // Reverse direction
    stepper.moveTo(-stepper.currentPosition());
  }

  // Move the motor one step
  stepper.run();
}