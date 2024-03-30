const int led = 8;  //LED pin
const int pir = 2;  //PIR pin
int motion;
boolean hasRun = false;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(pir, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  boolean status = false;     // default to absence of motion
  motion = digitalRead(pir);  //read sensor pin
  if (motion) {               // if motion detected
    //Serial.println(motion);   // print sensor reading 0 or 1
    Serial.println("ON");
    digitalWrite(led, HIGH);  // turn LED On
    hasRun = false;
    delay(10000);  // Wait for 10000 millisecond(s)
  } else {  //no motion detected
    digitalWrite(led, LOW);
    printOFF();
  }
}


void printOFF() {
  if (!hasRun) {
    Serial.println("OFF");
  }
  hasRun = true;
}