// C++ code
//
#include <Servo.h>

int distance = 0;

int height = 0;

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}

Servo servo_A0;

void setup()
{
  servo_A0.attach(A0, 500, 2500);
  pinMode(7, OUTPUT);
}

void loop()
{
  distance = 0.01723 * readUltrasonicDistance(10, 11);
  if (distance < 30) {
    servo_A0.write(120);
    digitalWrite(7, HIGH);
    delay(1000); // Wait for 1000 millisecond(s)
  } else {
    servo_A0.write(0);
    digitalWrite(7, LOW);
    delay(1000); // Wait for 1000 millisecond(s)
  }
}