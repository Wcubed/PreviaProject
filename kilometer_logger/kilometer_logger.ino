#include <SoftwareSerial.h>
#include <SD.h>
#include <SPI.h>
#include <MFRC522.h>

const int RST_PIN = 9;           // Configurable, see typical pin layout above
const int SS_PIN = 8;          // Configurable, see typical pin layout above

MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {

  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();

}

void loop() {

  if (rfid.PICC_IsNewCardPresent()) {

    if (rfid.PICC_ReadCardSerial()){

      Serial.print("Card UID: ");
      
      
    }
    
  }

}
