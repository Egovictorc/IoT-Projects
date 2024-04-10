
/**
   --------------------------------------------------------------------------------------------------------------------
   Example sketch/program showing how to read data from more than one PICC to serial.
   --------------------------------------------------------------------------------------------------------------------
   This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid

   Example sketch/program showing how to read data from more than one PICC (that is: a RFID Tag or Card) using a
   MFRC522 based RFID Reader on the Arduino SPI interface.

   Warning: This may not work! Multiple devices at one SPI are difficult and cause many trouble!! Engineering skill
            and knowledge are required!

   @license Released into the public domain.

   Typical pin layout used:
   -----------------------------------------------------------------------------------------
               MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
               Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
   Signal      Pin          Pin           Pin       Pin        Pin              Pin
   -----------------------------------------------------------------------------------------
   RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
   SPI SS 1    SDA(SS)      ** custom, take a unused pin, only HIGH/LOW required *
   SPI SS 2    SDA(SS)      ** custom, take a unused pin, only HIGH/LOW required *
   SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
   SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
   SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15

*/

#include <SPI.h>
#include <MFRC522.h>

// PIN Numbers : RESET + SDAs
#define RST_PIN 9
#define SS_1_PIN 10
#define SS_2_PIN 8
#define SS_3_PIN 7
#define SS_4_PIN 6

// Led and Relay PINS
#define GreenLed 2
#define relayIN 3
#define RedLed 4

boolean card_0_status = false;
// List of Tags UIDs that are allowed to open the puzzle
byte tagarray[][4] = {
  { 0x4B, 0x17, 0xBC, 0x79 },
  { 0x8A, 0x2B, 0xBC, 0x79 },
  { 0x81, 0x29, 0xBC, 0x79 },
  { 0xE6, 0xDF, 0xBB, 0x79 },
};

// Inlocking status :
int tagcount = 0;
bool access = false;

#define NR_OF_READERS 4

byte ssPins[] = { SS_1_PIN, SS_2_PIN, SS_3_PIN, SS_4_PIN };

boolean card_1 = false;
boolean card_2 = false;
boolean card_3 = false;

// reader status
int reader_status[] = { 0, 0, 0, 0 };
// Create an MFRC522 instance :
MFRC522 mfrc522[NR_OF_READERS];

/**
   Initialize.
*/
void setup() {

  Serial.begin(9600);  // Initialize serial communications with the PC
  while (!Serial)
    ;  // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)

  SPI.begin();  // Init SPI bus

  /* Initializing Inputs and Outputs */
  pinMode(GreenLed, OUTPUT);
  digitalWrite(GreenLed, LOW);
  pinMode(relayIN, OUTPUT);
  digitalWrite(relayIN, HIGH);
  pinMode(RedLed, OUTPUT);
  digitalWrite(RedLed, LOW);

  /* looking for MFRC522 readers */
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN);
    Serial.print(F("Reader "));
    Serial.print(reader);
    Serial.print(F(": "));
    mfrc522[reader].PCD_DumpVersionToSerial();
    //mfrc522[reader].PCD_SetAntennaGain(mfrc522[reader].RxGain_max);
  }
}

uint8_t buf[10] = {};
uint8_t control = 0x00;
/*
   Main loop.
*/

void loop() {

  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    // Looking for new cards
    //if (mfrc522[reader].PICC_IsNewCardPresent()) {

    //if (mfrc522[reader].PICC_IsNewC..
    // Look for new cards
    while (true) {
      //digitalWrite(Green, HIGH);
      control = 0;
      for (int i = 0; i < 3; i++) {
        if (!mfrc522[reader].PICC_IsNewCardPresent()) {
          if (mfrc522[reader].PICC_ReadCardSerial()) {
            //Serial.print('a');
            control |= 0x16;
          }
          if (mfrc522[reader].PICC_ReadCardSerial()) {
            //Serial.print('b');
            control |= 0x16;
          }
          //Serial.print('c');
          control += 0x1;
        }
        //Serial.print('d');
        control += 0x4;
      }

      //Serial.println(control);
      if (control == 13 || control == 14) {
        //card is still there
        //Serial.println("Card Present");
        //Serial.print(F("Reader "));
        //Serial.print(reader);
        //print_status(reader);
        status_on(reader);
      } else {
        status_off(reader);
        //Serial.print(reader);
        //Serial.print(F("Absent "));
        
        break;
      }
    }
    //Serial.println("CardRemoved");
    //digitalWrite(Green, LOW);
    //delay(500);  //change value if you want to read cards faster

    mfrc522[reader].PICC_HaltA();
    mfrc522[reader].PCD_StopCrypto1();
  }
  //Serial.println(reader_status[reader]);
  //for(uint8_t reader..
}

/**
   Helper routine to dump a byte array as hex values to Serial.
*/
void dump_byte_array(byte* buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    //Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

void status_on(int reader) {

  switch(reader) {
    case 0: {
      if (reader_status[0] == 0) {
        //Serial.println("YES");
        print_serial(reader);
      }
      reader_status[0] = 1;
      break;
    };
    case 1: {
      if (reader_status[1] == 0) {
          //Serial.println("YES1");
          print_serial(reader);
      }
      reader_status[1] = 1;
      break;
    };
    case 2: {
      if (reader_status[2] == 0) {
          //Serial.println("YES2");
          print_serial(reader);
      }
      reader_status[2] = 1;
      break;
    };
    case 3: {
      if (reader_status[3] == 0) {
          //Serial.println("YES3");
          print_serial(reader);
      }
      reader_status[3] = 1;
      break;
    };
  }
}

void status_off(int reader) {
  switch(reader) {
    case 0: {
      if (reader_status[0] == 1) {
          Serial.println("NO");
      }
      reader_status[0] = 0;
      break;
    };
    case 1: {
      if (reader_status[1] == 1) {
          Serial.println("NO1");
      }
      reader_status[1] = 0;
      break;
    };
    case 2: {
      if (reader_status[2] == 1) {
          Serial.println("NO2");
      }
      reader_status[2] = 0;
      break;
    };
    case 3: {
      if (reader_status[3] == 1) {
          Serial.println("NO3");
      }
      reader_status[3] = 0;
      break;
    };
  }
}

void print_serial(int reader) {
  // Show some details of the PICC (that is: the tag/card)
      mfrc522[reader].PICC_ReadCardSerial();            
      //Serial.print(F(": Card UID:"));
      dump_byte_array(mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size);
      Serial.println();
}