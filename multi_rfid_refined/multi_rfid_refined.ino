
// DEFINES
// Provides debugging information over serial connection if defined
#define DEBUG
//constants
// Libraries
#include <SPI.h>
#include <MFRC522.h>

// GLOBALS
// PIN Numbers : RESET + SDAs
#define RST_PIN 9
#define SS_1_PIN 10
#define SS_2_PIN 8
#define SS_3_PIN 7
#define SS_4_PIN 6

// The number of RFID readers
const byte numReaders = 4;
//Each reader has a unique slave select pin
const byte ssPins[] = { SS_1_PIN, SS_2_PIN, SS_3_PIN, SS_4_PIN };

// They will share the same reset pin
const byte resetPin = RST_PIN;
// initialize an array of MFRC522 instances representing each reader
MFRC522 mfrc522[numReaders];

// The sequence of NFC tag IDs required to solve the puzzle
//const String correctIDs[] = {"3acbbbd45", "ca767564", "3af4f763", "87e0f0a4"};
//The tag IDs currently detected by each reader
String currentIDs[numReaders];
byte readerStats[numReaders] = {0,0,0,0};


void setup() {
// put your setup code here, to run once:
#ifdef DEBUG
  // Initialize serial communications channel with the PC
  Serial.begin(9600);
  Serial.println(F("Serial Communication Started"));
#endif

  //initialize the SPI bus
  SPI.begin();

  for (uint8_t i = 0; i < numReaders; i++) {
    //initialize the reader
    //Note that SPI pins on the reader must always be connected to certain
    // Arduino pins (on an Uno, MOSI => pin11, MISO=> pin12, SCK => pin13)
    // The slave select (SS) pin and reset pin can be assigned to any pin
    mfrc522[i].PCD_Init(ssPins[i], resetPin);

    //Set the gain to max - not sure this makes any difference...
    // mfrc522[i].PCD_SetAntennaGain(MFRC522::PCD_RxGain_max);

    //Dump some debug information to the serial monitor
    Serial.print(F("Reader #"));
    Serial.print(i);
    Serial.print(F(" initialized on Pin "));
    Serial.print(String(ssPins[i]));
    Serial.print(F(". Antenna Strength: "));
    Serial.print(mfrc522[i].PCD_GetAntennaGain());
    Serial.print(F(" . Version : "));
    mfrc522[i].PCD_DumpVersionToSerial();

    // Slight delay before activating next reader
    delay(1000);
  }
  // put your setup code here, to run once:
  Serial.println(F("------- END SETUP ------ "));
}


void loop() {
  // put your main code here, to run repeatedly:
  //Assume that the tags have not changed since last reading
  boolean changedValue = false;

  //Loop through each reader
  for (uint8_t i = 0; i < numReaders; i++) {
    //initialize the sensor
    mfrc522[i].PCD_Init();

    //String to hold the ID detected by each sensor
    String readRFID = "";
    byte readStat;

    // If the sensor detects a tag and is able to read it
    if (mfrc522[i].PICC_IsNewCardPresent() && mfrc522[i].PICC_ReadCardSerial()) {
      // Extract the ID from the Tag
      //readRFID = dump_byte_array(mfrc522[i].uid.uidByte, mfrc522[i].uid.size);
      readerStats[i] = 1;
    } else {
      readerStats[i] = 0;
    }
    // if the current reading is different from the last known reading
    //if(readRFID != currentIDs[i]) {
    if (readStat != readerStats[i]) {
      //Set the flag to show that the puzzle state has changed
      changedValue = true;
      // Dump to serial the current state of the changed sensors
      dump_byte_array(mfrc522[i].uid.uidByte, mfrc522[i].uid.size);
      Serial.println(F("---"));
      //Update the stored value for this sensor
      //currentIDs[i] = readRFID;
      readerStats[i] = readStat;
    } else {
      changedValue = false;
    }

    // if (changedValue) {

    //   // Dump to serial the current state of the changed sensors
    //   dump_byte_array(mfrc522[i].uid.uidByte, mfrc522[i].uid.size);
    //   Serial.println(F("---"));
    //   // Add a short delay before next polling sensors
    //   //delay(1000);
    // }

    //Halt PICC
    mfrc522[i].PICC_HaltA();
    // Stop encryption on PCD
    mfrc522[i].PCD_StopCrypto1();


  }

  //If the changedValue flag has been set, at least one sensor has changed
  if (changedValue) {
    //Dump to serial the current state of all sensors
    // for(uint8_t i= 0; i < numReaders; i++) {
    //   Serial.print(F("Reader #"));
    //   Serial.print(String(i));
    //   Serial.print(F(" on Pin #"));
    //   Serial.print(String((ssPins[i])));
    //   Serial.print(F(" detected tag: "));
    //   Serial.println(currentIDs[i]);
    // }
    // Dump to serial the current state of the changed sensors
    Serial.println(F("---"));
    // Add a short delay before next polling sensors
    //delay(1000);
  }
}


void dump_byte_array(byte* buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    //Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
/*
print RFID tag
*/
// String dump_byte_array(byte* buffer, byte bufferSize) {
//   String byteString = "";
//   for (byte i = 0; i < bufferSize; i++) {
//     //Serial.print(buffer[i] < 0x10 ? " 0" : " ");
//     byteString += String(buffer[i]);
//     Serial.print(buffer[i], HEX);
//   }

//   return byteString;
// }