
int ledState = LOW;
unsigned long currentMillis = 0;
unsigned long blinkPreviousMillis = 0;
const unsigned long blinkPeriod = 0;


void setup() {
  // put your setup code here, to run once:

Serial.begin(9600);
}


void blinkLED() {
 if(currentTtime - blinkPreviousMilis >= blinkPeriod) {
    blinkPreviousMillis = currentMillis;
 }
}


void loop() {
  currentMillis = millis();
  // put your main code here, to run repeatedly:
  Serial.println(millis());
}
