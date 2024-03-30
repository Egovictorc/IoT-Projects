#include <MIDI.h>
#include <Boards.h>
#include <Firmata.h>


#include <CapacitiveSensor.h>

CapacitiveSensor cs_2_4 = CapacitiveSensor(2,4);

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
// Set MIDI baud rate:
Serial.begin(9600);
}

void loop() {

long start = millis(); 
long total5 = cs_2_4.capacitiveSensor(30);

  
Serial.print(millis() - start);        // check on performance in milliseconds
//Serial.print("cc ch 10 ctrl 12 val = ");                    // tab character for debug windown spacing
 Serial.print("\t");   
Serial.println(total5);  

  
MIDI_TX(144,28,48); // NOTE ON
delay(500); // Wait 5 sec before sending note again.
}

void MIDI_TX(unsigned char MESSAGE, unsigned char PITCH, unsigned char VELOCITY)
{
Serial.write(MESSAGE);
Serial.write(PITCH);
Serial.write(VELOCITY);
}