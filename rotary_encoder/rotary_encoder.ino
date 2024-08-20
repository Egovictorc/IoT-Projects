// volatile unsigned int temp, counter = 0;  //This variable will increase or decrease depending on the rotation of encoder
volatile double temp, counter = 0.000000;  //This variable will increase or decrease depending on the rotation of encoder
double factor = 0.000251;

void setup() {
  Serial.begin(9600);

  pinMode(2, INPUT_PULLUP);  // internal pullup input pin 2

  pinMode(3, INPUT_PULLUP);  // internalเป็น pullup input pin 3
                             //Setting up interrupt
  //A rising pulse from encodenren activated ai0(). AttachInterrupt 0 is DigitalPin nr 2 on moust Arduino.
  attachInterrupt(0, ai0, RISING);

  //B rising pulse from encodenren activated ai1(). AttachInterrupt 1 is DigitalPin nr 3 on moust Arduino.
  attachInterrupt(1, ai1, RISING);
}

void loop() {
  // Send the value of counter
  if (counter != temp) {
    // Serial.println(counter);
    Serial.println(counter, 6);
    temp = counter;
  }

  // debounce();
}


void debounce() {
  delay(200); //add some delay
}

void ai0() {
  // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
  // Check pin 3 to determine the direction

  if (digitalRead(3) == LOW) {
    // counter++;
    counter = counter + factor;
  } else {
    // counter--;
    counter = counter - factor;
  }
}

void ai1() {
  // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
  // Check with pin 2 to determine the direction
  if (digitalRead(2) == LOW) {
    // counter--;
    counter = counter - factor;
  } else {
    counter = counter + factor;
  }
}