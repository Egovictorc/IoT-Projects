

int ledPin = 2;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available()) {
    String cmd = Serial.readString();

    if(cmd == "on") {
      digitalWrite(ledPin, HIGH);
    } else if(cmd == "off") {
      digitalWrite(ledPin, LOW);
    }
  }
}
