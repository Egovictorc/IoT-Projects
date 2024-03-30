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
  //Serial.begin(9600);
  pinMode(8, OUTPUT);
}

void loop()
{
  distance = 0.01723 * readUltrasonicDistance(4, 2);
  if (distance <= 30) {
    digitalWrite(8, HIGH);
    Serial.println("Yeap yeap");
    delay(10000); // Wait for 10000 millisecond(s)
  } else {
    digitalWrite(8, LOW);
  }
}