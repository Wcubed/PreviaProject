#include <SPI.h>
#include <MFRC522.h>

const int rfid_RST_PIN = 9;
const int rfid_SS_PIN = 8;

MFRC522 rfid(rfid_SS_PIN, rfid_RST_PIN);
MFRC522::MIFARE_Key key;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}

void loop() {

  MFRC522::StatusCode status;
  byte buffer[18];
  byte size = sizeof(buffer);

  // put your main code here, to run repeatedly:
  if (rfid.PICC_IsNewCardPresent()) {

    if (rfid.PICC_ReadCardSerial()) {

      status = rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 5, &key, &(rfid.uid));

      if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(rfid.GetStatusCodeName(status));
      }

      status = (MFRC522::StatusCode) rfid.MIFARE_Read(5, buffer, &size);
      if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(rfid.GetStatusCodeName(status));
      } else {
        dump_byte_array(buffer, 16);
        Serial.println();
      }

      rfid.PCD_StopCrypto1();
    }
  }
}


void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
