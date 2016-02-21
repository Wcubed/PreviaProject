#include <SoftwareSerial.h>
#include <SD.h>
#include <SPI.h>
#include <MFRC522.h>

const int rfid_RST_PIN = 9;           // Configurable, see typical pin layout above
const int rfid_SS_PIN = 8;          // Configurable, see typical pin layout above

MFRC522 rfid(rfid_SS_PIN, rfid_RST_PIN);

void setup() {

  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();

}

void loop() {

  if (rfid.PICC_IsNewCardPresent()) {

    if (rfid.PICC_ReadCardSerial()){

      Serial.print("Card UID: ");
      print_byte_array(rfid.uid.uidByte, rfid.uid.size);
      Serial.println();
      
    }
    
  }

}

void print_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}
