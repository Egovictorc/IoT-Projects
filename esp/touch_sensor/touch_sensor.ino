
int ledPin = 2;
void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
  int val = touchRead(T0);
  if(val < 50) {
    Serial.println(val);
      digitalWrite(ledPin, HIGH);
  } else {
    Serial.println(val);
    digitalWrite(ledPin, LOW);
  }
  delay(300);

}
