// Make sure the buffer is 16 bytes long.
boolean write_to_block(MFRC522 rfid, byte blockadr, MFRC522::Uid *uid, byte *buffer) {

  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  MFRC522::StatusCode status;
  rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockadr, &key, uid);

  if (status != MFRC522::STATUS_OK) {
    Serial.println("Authentication Failed:");
    Serial.println(rfid.GetStatusCodeName(status));

    rfid.PCD_StopCrypto1();  // Stop encryption on PCD
    return false;
  }

  Serial.println("Authentication was succesful");

  status = rfid.MIFARE_Write(blockadr, buffer, BLOCK_LENGTH);
  if (status != MFRC522::STATUS_OK) {
    Serial.println("Writing Failed:");
    Serial.println(rfid.GetStatusCodeName(status));

    rfid.PCD_StopCrypto1();  // Stop encryption on PCD
    return false;
  }

  rfid.PCD_StopCrypto1();  // Stop encryption on PCD
  return true;
}


boolean read_block(MFRC522 rfid, byte blockadr, MFRC522::Uid *uid, byte buffer[]) {

  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
  
  MFRC522::StatusCode status;
  rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockadr, &key, uid);

  if (status != MFRC522::STATUS_OK) {
    Serial.println("Authentication Failed:");
    Serial.println(rfid.GetStatusCodeName(status));

    rfid.PCD_StopCrypto1();  // Stop encryption on PCD
    return false;
  }

  Serial.println("Authentication was succesful");

  byte sizeOfBuffer = sizeof(*buffer);

  status = rfid.MIFARE_Read(blockadr, buffer, &sizeOfBuffer);

  Serial.println(*buffer);
  Serial.println(sizeOfBuffer);
  
  if (status != MFRC522::STATUS_OK) {
    Serial.println("Reading Failed:");
    Serial.println(rfid.GetStatusCodeName(status));

    rfid.PCD_StopCrypto1();  // Stop encryption on PCD
    return false;
  }

  rfid.PCD_StopCrypto1();
  return true;
}
