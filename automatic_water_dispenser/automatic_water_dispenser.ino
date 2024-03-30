// C++ code
//
int distance = 0;

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

void setup()
{
  pinMode(6, OUTPUT);
  pinMode(8, OUTPUT);
}

void loop()
{
  distance = 0.01723 * readUltrasonicDistance(12, 10);
  if (distance < 20) {
    digitalWrite(6, HIGH);
    digitalWrite(8, HIGH);
  } else {
    digitalWrite(6, LOW);
    digitalWrite(8, LOW);
  }
  delay(10); // Delay a little bit to improve simulation performance
}