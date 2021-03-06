#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <SD.h>
#include <SPI.h>
#include <MFRC522.h>

const int rfid_RST_PIN = 9;
const int rfid_SS_PIN = 8;

const int UID_Block = 8;

MFRC522 rfid(rfid_SS_PIN, rfid_RST_PIN);

unsigned long lastMillis;
int lastCardCountdown = 0;
unsigned long lastCardUID;



void setup() {

  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();

  Serial.println("Previa Ready To Roll, Scan Your Card!");

}

void loop() {
  MFRC522::StatusCode status;
  rfid.setSPIConfig(); //reset SPI settings to use RC522 instead of SD e.g.
  // Calculate delta time.
  unsigned long curMillis = millis();
  unsigned int dt = curMillis - lastMillis;
  lastMillis = curMillis;

  if (lastCardCountdown > 0) {
    lastCardCountdown -= dt;
  }

  // Is there a card available.
  if (rfid.PICC_IsNewCardPresent()) {

    if (rfid.PICC_ReadCardSerial()) {

      unsigned long curCardUID = uid_to_long(rfid.uid.uidByte);

      // Is this a new card, or has enough time passed.
      if (lastCardUID != curCardUID || lastCardCountdown <= 0) {
        // Reset the countdown.
        lastCardCountdown = 5000;
        lastCardUID = curCardUID;

        Serial.print("Card UID: ");
        Serial.println(lastCardUID, HEX);

        // The actual checking in or out.
        handle_card(curCardUID);
      }
    }
  }
}

//check in or check out depending on situation
void handle_card(long uid) {
  MFRC522::MIFARE_Key UID_key;
  for (byte i = 0; i < 6; i++) UID_key.keyByte[i] = 0xFF;

  MFRC522::StatusCode status;
  status = rfid.PCD_Authenticate(rfid.PICC_CMD_MF_AUTH_KEY_A, UID_Block, &UID_key, &(rfid.uid));
  if (status == rfid.STATUS_OK) {
    if (eeprom_check_uid(uid)) {
      //kilometervereffeningcode

      //check out
      eeprom_delete_uid(uid);
      Serial.println("uitgecheckt");
    } else {
      //kilometervereffeningcode

      //check in
      if (eeprom_add_uid(uid)) {
        Serial.println("ingecheckt");
      } else {
        //beep beep code
        Serial.println("array is full, did not check in");
      }
    }
  }

  rfid.PCD_StopCrypto1();
}

// Converts uids into a single value.
unsigned long uid_to_long(byte uid[4]) {
  unsigned long val = 0;
  val += (long)uid[0] << 24;
  val += (long)uid[1] << 16;
  val += (long)uid[2] << 8;
  val += (long)uid[3];
  return val;
}

// Prints byte arrays to the serial monitor.
void print_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
