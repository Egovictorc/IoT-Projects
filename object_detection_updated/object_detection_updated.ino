// C++ code
//
int distance = 0;
int led = 8;
boolean isOff = false;
boolean isOn = false;


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
  Serial.begin(9600);
  pinMode(8, OUTPUT);
}

void loop()
{
  distance = 0.01723 * readUltrasonicDistance(4, 2);
  if (distance <= 30) {
    printON();
    isOff = false;
    //delay(10000); // Wait for 10000 millisecond(s)
    delay(3000);
  } else {
    printOFF();
      isOn = false;
  }
}



void printOFF() {
  if (!isOff) {
    Serial.println("OFF");
    digitalWrite(led, LOW);
    digitalWrite(LED_BUILTIN, LOW);
  }
  isOff = true;
}

void printON() {
  if (!isOn) {
    //Serial.println("ON");
    Serial.println("ON");
    digitalWrite(led, HIGH);  // turn LED On
    digitalWrite(LED_BUILTIN, HIGH);
  }
  isOn = true;
}