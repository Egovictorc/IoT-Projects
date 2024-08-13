#define ledPin 16


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("-------------- Serial communication started --------------- ");
  pinMode(ledPin, OUTPUT);
}

int count = 0;
void loop() {
  // put your main code here, to run repeatedly:

  for(count = 0; count <= 255; count++) {
    Serial.print(" ");
    Serial.print(count);
    analogWrite(ledPin, count);
    delay(300);
  }

  delay(5000);
}
