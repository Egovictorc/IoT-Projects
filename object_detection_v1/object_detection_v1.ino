// C++ code
//
int distance = 0;
int ledPin = 8;
const int echoPin = 2;
const int triggerPin = 4;
boolean isOFF = true;


long readUltrasonicDistance(int triggerPin, int echoPin) {
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

void setup() {
  Serial.begin(9600);
  Serial.println("------------------- Setup completed ------------------");
  pinMode(8, OUTPUT);
}

void loop() {
  distance = 0.01723 * readUltrasonicDistance(triggerPin, echoPin);
  Serial.println(distance);
  if (distance <= 120) {
    if (isOFF == true) {
      Serial.println("ON");
      digitalWrite(ledPin, HIGH);
      isOFF = false;
    }
    delay(2000);
  } else {
    if (isOFF == false) {
      Serial.println("OFF");
      digitalWrite(ledPin, LOW);
      isOFF = true;
    }
  }
  //delay(3000);
  // } else {
  //     isOff = true;
  // }
}


// void printOFF() {
//   if (!isOff) {
//     Serial.println("OFF");
//     digitalWrite(led, LOW);
//     digitalWrite(LED_BUILTIN, LOW);
//   }

// }

// void printON() {
//   if (!isOn) {
//     //Serial.println("ON");
//     Serial.println("ON");
//     digitalWrite(led, HIGH);  // turn LED On
//     digitalWrite(LED_BUILTIN, HIGH);
//   }
// }