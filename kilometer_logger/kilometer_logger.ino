#include <SoftwareSerial.h>
#include <SD.h>
#include <SPI.h>
#include <MFRC522.h>

const int rfid_RST_PIN = 9;           // Configurable, see typical pin layout above
const int rfid_SS_PIN = 8;          // Configurable, see typical pin layout above

MFRC522 rfid(rfid_SS_PIN, rfid_RST_PIN);

unsigned long lastMillis;
int lastCardCountdown = 0;
byte lastCardUID[4];

void setup() {

  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();

}

void loop() {
  // Calculate delta time.
  unsigned long curMillis = millis();
  unsigned int dt = curMillis - lastMillis;
  lastMillis = curMillis;

  if (lastCardCountdown > 0) {
    lastCardCountdown -= dt;
  }

  if (rfid.PICC_IsNewCardPresent()) {

    if (rfid.PICC_ReadCardSerial()) {

      if (!are_uids_equal(lastCardUID, rfid.uid.uidByte) || lastCardCountdown <= 0) {
        lastCardCountdown = 5000;
        
        for (int i = 0; i < 3; i++) {
          lastCardUID[i] = rfid.uid.uidByte[i]; //copy UID into lastcarduid
        }

        Serial.print("Card UID: ");
        print_byte_array(rfid.uid.uidByte, rfid.uid.size); //print UID of card
        Serial.println();
      }

    }

  }



}

// Checks if two UIDs are equal.
bool are_uids_equal(byte uid1[4], byte uid2[4]) {
  for (int i = 0; i < 3; i++) {
    if (uid1[i] != uid2[i]) {
      return false;
    }
  }

  return true;
}

// Prints byte arrays to the serial monitor.
void print_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
