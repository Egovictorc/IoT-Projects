#include <AccelStepper.h>

// Define motor interface type
#define MotorInterfaceType 1  // 1 means Driver type (DIR, STEP)

#define stepPin 14  // STEP pin
#define dirPin 13   // DIR pin

// Create an instance of AccelStepper
AccelStepper stepper(MotorInterfaceType, stepPin, dirPin);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Serial initialized");

  // Configure motor speed and acceleration
  stepper.setMaxSpeed(1000);     // Set maximum speed (steps per second)
  stepper.setAcceleration(200); // Set acceleration (steps per second squared)
  stepper.setSpeed(1000); 
  // Set initial target position
  stepper.moveTo(1600);         // Move to 1600 steps (e.g., 2 revolutions if 800 steps per revolution)


}
void loop() {
 if (stepper.distanceToGo() < 1000) {
    // If the motor reached the target, set a new target position
    stepper.moveTo(stepper.currentPosition() + 2000); // Change direction by moving back to the start position
  }
  stepper.run(); // Run the motor to the target position
}
