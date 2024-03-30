#include <MIDI.h>
#include <Boards.h>
#include <Firmata.h>


#include <CapacitiveSensor.h>

CapacitiveSensor cs_2_4 = CapacitiveSensor(2, 4);

MIDI_CREATE_DEFAULT_INSTANCE();

int distance = 0;

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
  // Set MIDI baud rate:
  Serial.begin(9600);
  pinMode(8, OUTPUT);
}

void loop() {

  long start = millis();
  long total5 = cs_2_4.capacitiveSensor(30);


  //Serial.print(millis() - start);  // check on performance in milliseconds
                                   //Serial.print("cc ch 10 ctrl 12 val = ");                    // tab character for debug windown spacing
  //Serial.print("\t");
  //Serial.println(total5);


 distance = 0.01723 * readUltrasonicDistance(4, 2);
  if (distance <= 30) {
    digitalWrite(8, HIGH);
    //MIDI_TX(144, 28, 48);  // NOTE ON
    //MIDI_TX(0x90, 0x5A, 0x45);  // NOTE ON
    Serial.println("ON");
    delay(10000); // Wait for 10000 millisecond(s)
  } else {
    digitalWrite(8, LOW);
  }

  //delay(500);            // Wait 5 sec before sending note again.
}

void MIDI_TX(unsigned char MESSAGE, unsigned char PITCH, unsigned char VELOCITY) {
  Serial.write(MESSAGE);
  Serial.write(PITCH);
  Serial.write(VELOCITY);
}