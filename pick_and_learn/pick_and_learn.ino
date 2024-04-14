#include <SPI.h>
#include <MFRC522.h>

#include <AceSorting.h>
using ace_sorting::shellSortKnuth;

// GLOBALS
// PIN Numbers : RESET + SDAs
#define RST_PIN 9
#define SS_1_PIN 10
#define SS_2_PIN 8
// #define SS_3_PIN 7
#define SS_4_PIN 6
// The number of RFID readers
const byte numReaders = 3;
//Each reader has a unique slave select pin
// const byte ssPins[] = { SS_1_PIN, SS_2_PIN, SS_3_PIN, SS_4_PIN };
const byte ssPins[] = { SS_1_PIN, SS_2_PIN, SS_4_PIN };

// They will share the same reset pin
const byte resetPin = RST_PIN;
// initialize an array of MFRC522 instances representing each reader
MFRC522 mfrc522[numReaders];

// The sequence of NFC tag IDs required to solve the puzzle
//const String correctIDs[] = {"3acbbbd45", "ca767564", "3af4f763", "87e0f0a4"};
//The tag IDs currently detected by each reader
String currentIDs[numReaders];
int rfidStats[numReaders];


void setup() {

  Serial.begin(9600);  // Initialize serial communications with the PC
  Serial.println(F("Serial Communication Started"));
  while (!Serial)
    ;           // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();  // Init SPI bus

  for (uint8_t i = 0; i < numReaders; i++) {
    //initialize the reader
    //Note that SPI pins on the reader must always be connected to certain
    // Arduino pins (on an Uno, MOSI => pin11, MISO=> pin12, SCK => pin13)
    // The slave select (SS) pin and reset pin can be assigned to any pin
    mfrc522[i].PCD_Init(ssPins[i], resetPin);  // Init MFRC522

    //Dump some debug information to the serial monitor
    Serial.print(F("Reader #"));
    Serial.print(i);
    Serial.print(F(" initialized on Pin "));
    Serial.print(String(ssPins[i]));
    Serial.print(F(". Antenna Strength: "));
    Serial.print(mfrc522[i].PCD_GetAntennaGain());
    Serial.print(F(" . Version : "));
    mfrc522[i].PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details

    // Slight delay before activating next reader
    delay(1000);  // Optional delay. Some board do need more time after init to be ready, see Readme
  }
  // put your setup code here, to run once:
  Serial.println(F("------- END SETUP ------ "));
  String de = "3,1,5,2";
}

void loop() {
  //Assume that the tags have not changed since last reading
  boolean changedValue = false;
  byte removedID;
  byte placedID;
  //Loop through each reader
  for (uint8_t i = 0; i < numReaders; i++) {
    //initialize the sensor
    mfrc522[i].PCD_Init();

    //String to hold the ID detected by each sensor
    String readRFID = "";

    // If the sensor detects a tag and is able to read it
    if (mfrc522[i].PICC_IsNewCardPresent() && mfrc522[i].PICC_ReadCardSerial()) {
      // Extract the ID from the Tag
      readRFID = dump_byte_array(mfrc522[i].uid.uidByte, mfrc522[i].uid.size);

    } else if (rfidStats[i] != 0) {  //tag removed
      // Serial.println("OFF_"+ String(i));
      off_tags_when_removed(i);
      //currentIDs[i] = "";
      rfidStats[i] = 0;
    }
    // If the sensor does not detects a tag and is able to read it

    // if the current reading is different from the last known reading
    // if (readRFID != currentIDs[i]) {
    //   if (readRFID != "") {  // RFID Tag Changed
    //     //Set the flag to show that the puzzle state has changed
    //     changedValue = true;
    //     currentID = i;
    //     //Update the stored value for this sensor
    //     currentIDs[i] = readRFID;
    //   } else {  // RFID Tag removed
    //     //currentIDs[i] = readRFID;
    //     off_tags_when_removed(i);
    //     current_value = ;
    //   }
    // }

    //tag changed
    if (readRFID != "" && rfidStats[i] == 0) {
      // changedValue = true;
      // print_placed_tag(readRFID);
      status_when_placed(readRFID, i);
      rfidStats[i] = 1;
      currentIDs[i] = readRFID;
    }
    // if (readRFID != currentIDs[i] && readRFID != "") {
    //     // changedValue = true;
    //     // print_placed_tag(readRFID);
    //     status_when_placed(readRFID, i);
    //     currentIDs[i] = readRFID;
    // }

    //Halt PICC
    mfrc522[i].PICC_HaltA();
    // Stop encryption on PCD
    mfrc522[i].PCD_StopCrypto1();
  }

  //check if tag has been removed
  // if (readRFID == "" && readRFID != currentIDs[i]) {
  //   Serial.println("reader_" + String(currentID) + "_removed");
  // }
}


void status_when_placed(String rfid, int placedId) {
  String off_arr[numReaders];
  String on_arr[numReaders];
  for (int i = 0; i < numReaders; i++) {
    if (i == placedId) {
      on_arr[i] = rfid;
      continue;
    }
    if (rfidStats[i] == 0) {
      off_arr[i] = currentIDs[i];
    } else {
      on_arr[i] = currentIDs[i];
    }
  }

  // sort the array of offs
  shellSortKnuth(off_arr, numReaders);
  shellSortKnuth(on_arr, numReaders);

  String off_str = "";
  String on_str = "";
  for (uint8_t i = 0; i < numReaders; i++) {
    // int val = id_arr[i].toInt();
    if (off_arr[i] != "") {
      off_str += +"_" + off_arr[i];
    }
  }

  for (uint8_t i = 0; i < numReaders; i++) {
    // int val = id_arr[i].toInt();
    if (on_arr[i] != "") {
      on_str += +"_" + on_arr[i];
    }
  }
  // Serial.println("OFF" + off_str);

  if (off_str.length() == 0) {
    Serial.println("ON" + on_str + "_NO_OFF");
    return;
  }
  Serial.println("ON" + on_str + "_OFF" + off_str);
}


void off_tags_when_removed(int id) {
  String id_str[numReaders];
  for (int i = 0; i < numReaders; i++) {
    if (i == id) {
      id_str[i] = currentIDs[id];
      continue;
    }
    if (rfidStats[i] == 0) {
      id_str[i] = currentIDs[i];
      ;
    }
  }

  shellSortKnuth(id_str, numReaders);

  String result = "";
  for (uint8_t i = 0; i < numReaders; i++) {
    // int val = id_arr[i].toInt();
    if (id_str[i] != "") {
      result += +"_" + id_str[i];
    }
  }
  Serial.println("OFF" + result);


  // Serial.println("sorted ")
  // Serial.println("OFF_"+ sorted);
}








// void off_tags_when_removed(int id) {
//   String id_str = String(id);
//   for (int i = 0; i < numReaders; i++) {
//     if (i == id) {
//       continue;
//     }
//     if (rfidStats[i] == 0) {
//       id_str += String(i);
//     }
//   }

//   // Serial.println("OFF_" + id_str);
//   String id_arr[id_str.length()];
//   for (uint8_t i = 0; i < id_str.length(); i++) {
//     id_arr[i] = id_str[i];
//   }
//   shellSortKnuth(id_arr, id_str.length());
//   // print_arr(id_arr, id_str.length());
//   String sorted = "";
//   for (uint8_t i = 0; i < id_str.length(); i++) {
//      sorted += id_arr[i];
//   }

//   // serial_print(id_arr, id_str.length());
//   String result = "";
//     for (uint8_t i = 0; i < id_str.length(); i++) {
//       int val = id_arr[i].toInt();
//      result += + "_"+ currentIDs[val];
//   }
//   Serial.println("OFF"+ result);


//   // Serial.println("sorted ")
//   Serial.println("OFF_"+ sorted);
// }







void print_placed_tag(String id) {
  Serial.println("Placed_" + id);
}

// void status_when_placed(String rfid, int placedId) {
//   String off_str = "";
//   for (int i = 0; i < numReaders; i++) {
//     if (i == placedId) {
//       continue;
//     }
//     if (rfidStats[i] == 0) {
//       off_str += "_"+ currentIDs[i];
//     }
//   }

//   if (off_str.length() == 0) {
//     Serial.println("ON_" + rfid + "_NO_OFF");
//     return;
//   }
//   Serial.println("ON_" + rfid + "_OFF_" + off_str);
// }

void print_arr(int arr[], int count) {
  String ids = "";
  for (byte i = 0; i < count; i++) {
    ids += currentIDs[arr[i]];
  }

  Serial.println("OFF_" + ids);
}

String dump_byte_array(byte *buffer, byte bufferSize) {
  String str = "";
  for (byte i = 0; i < bufferSize; i++) {
    str = str + ((((buffer[i] & 0xF0) >> 4) <= 9) ? (char)(((buffer[i] & 0xF0) >> 4) + '0') : (char)(((buffer[i] & 0xF0) >> 4) + 'A' - 10));
    str = str + (((buffer[i] & 0x0F) <= 9) ? (char)((buffer[i] & 0x0F) + '0') : (char)((buffer[i] & 0x0F) + 'A' - 10));
    //if (i < (bufferSize - 1)) str = str + ' '; //uncomment if you want a space between each HEX number
  }
  //Serial.println(str);

  return str;
}