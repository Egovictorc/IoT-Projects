const int ledPin = 6;  //LED pin
const int sensorPin = 2;     //PIR pin
boolean isOff = false;

int sensorValue;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("----------------- PIR Detection Started ------------------------");
  pinMode(ledPin, OUTPUT);
  pinMode(sensorPin, INPUT);
}
bool status;

void loop() {
  // put your main code here, to run repeatedly:
   sensorValue = digitalRead(sensorPin);
  if (sensorValue) {
    Serial.println(sensorValue);
    digitalWrite(ledPin, HIGH);
    if (isOff) {
      Serial.println("ON");
      isOff = false;
    }
  } else {
    digitalWrite(ledPin, LOW);
    Serial.println(sensorValue);
    if (!isOff) {
      Serial.println("OFF");
      isOff = true;
    }
  }
}

