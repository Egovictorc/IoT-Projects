const int led = 8;  //LED pin
const int pir = 2;  //PIR pin
int motion;
boolean isOff = false;
boolean isOn = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(pir, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  motion = digitalRead(pir);  //read sensor pin
  if (motion) {               // if motion detected
    //Serial.println(motion);   // print sensor reading 0 or 1
    printON();
    isOff = false;
    //delay(10000);  // Wait for 10000 millisecond(s)
    delay(5000);  // Wait for 5 seconds
  } else {        //no motion detected
    printOFF();
    isOn = false;
    isOff = true;
  }
}


void printOFF() {
  if (!isOff) {
    Serial.println("OFF");
    digitalWrite(led, LOW);
    digitalWrite(LED_BUILTIN, LOW);
  }
  
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