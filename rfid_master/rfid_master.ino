#include <SPI.h>
#include <MFRC522.h>

const int COMM_LENGTH = 2; // Length in chars of the command sequence.
const int BLOCK_LENGTH = 16; // Length of a single block in an rfid tag.

const int rfid_RST_PIN = 9;
const int rfid_SS_PIN = 8;

MFRC522 rfid(rfid_SS_PIN, rfid_RST_PIN);

long cardUID = 0;
char firstName[BLOCK_LENGTH];
char lastName[BLOCK_LENGTH];

bool writing;

void setup() {
  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();
}

void loop() {

  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  // ---------- Rfid read / write ----------
  // Is there a card available.
  if (rfid.PICC_IsNewCardPresent()) {

    if (rfid.PICC_ReadCardSerial()) {
      unsigned long curCardUID = uid_to_long(rfid.uid.uidByte);

      if (curCardUID != cardUID) {
        writing = false;
        cardUID = curCardUID;
        
        Serial.print("Scanned card: ");
        Serial.println(cardUID, HEX);
        // Read the data from the card.
      }
    }
  }

  // ---------- User input ----------
  if (Serial.available() > 0) {
    char buffer[50];

    // Read a command from the serial monitor.
    int len = Serial.readBytesUntil('\n', buffer, sizeof(buffer));

    if (len >= 3) {
      if (buffer[1] == ' ') {
        // Read the command char.
        char command = buffer[0];
        // Read the data.
        char data[50];
        for (int i = COMM_LENGTH; i < len; i++) {
          data[i - COMM_LENGTH] = buffer[i];
        }
        // Padd with empty chars.
        for (int i = len - COMM_LENGTH; i < sizeof(data); i++) {
          data[i] = 0;
        }
        // Get the length of the data.
        int data_len = len - COMM_LENGTH;

        // ---- Execute commands. ----
        if (command == 'f') {
          // First name.
          if (data_len <= BLOCK_LENGTH) {
            for (int i = 0; i < BLOCK_LENGTH; i++) {
              firstName[i] = data[i];
            }
            print_current_data();

          } else { // Data is too long.
            Serial.print("First names can only be ");
            Serial.print(BLOCK_LENGTH - 1);
            Serial.println(" characters long.");
          }
        }
        if (command == 'l') {
          // Last name.
          if (data_len <= BLOCK_LENGTH) {
            for (int i = 0; i < BLOCK_LENGTH; i++) {
              lastName[i] = data[i];
            }
            print_current_data();

          } else { // Data is too long.
            Serial.print("Last names can only be ");
            Serial.print(BLOCK_LENGTH - 1);
            Serial.println(" characters long.");
          }
        }
        if (command == 'w') {
          // Write command.
          if (data[0] == 'w') {
            // User wants to write.
            if (cardUID != 0) {
              Serial.println("Place card in front of reader.");
              writing = true;
            } else {
              Serial.println("Please scan a card first.");
            }
          }
        }
        if (command == 'r') {
          // Reset.
          if (data[0] == 'r') {
            Serial.println("Abort operation.");
            writing = false;
            cardUID = 0L;
          }
        }
      }
    }
  }
}

// Prints all the data currently stored.
void print_current_data() {
  Serial.println("--------");
  Serial.print("UID: ");
  Serial.print(cardUID, HEX);
  Serial.println();

  Serial.print("First name is: \"");
  Serial.print(firstName);
  Serial.println("\".");

  Serial.print("Last name is: \"");
  Serial.print(lastName);
  Serial.println("\".");
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

