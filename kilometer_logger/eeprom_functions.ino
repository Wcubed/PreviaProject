const int KM_ADR = 0;
const int UID_ADR = 2;
const int UID_TYPE_LENGTH = 4;
const int UID_ARRAY_LENGTH = 10;

//adds a UID to the array of checked in UID's
bool eeprom_add_uid(long uid) {

  long curUID = 0;

  if (!eeprom_check_uid(uid)) {

    for (int i = 0; i < UID_ARRAY_LENGTH; i++) {
      EEPROM.get(UID_ADR + i * UID_TYPE_LENGTH, curUID);
      if (curUID == 0) {
        EEPROM.put(UID_ADR + i * UID_TYPE_LENGTH, uid);
        // UID was succesfully added.
        return true;
      }
    }
    // Array is full! error!
    return false;
  }
  // UID already exists in the array.
  return true;
}

//checks if a certain UID is checked in or not.
bool eeprom_check_uid(long uid) {

  long curUID = 0;

  for (int i = 0; i < UID_ARRAY_LENGTH; i++) {

    EEPROM.get(UID_ADR + i * UID_TYPE_LENGTH, curUID);
    if (uid == curUID) {
      return true;
    }
  }
  return false;
}

//deletes a UID from the array of checked in UID's or: check someone out.
void eeprom_delete_uid(long uid) {

  long curUID = 0;

  for (int i = 0; i < UID_ARRAY_LENGTH; i++) {
    EEPROM.get(UID_ADR + i * UID_TYPE_LENGTH, curUID);
    if (uid == curUID) {
      EEPROM.put(UID_ADR + i * UID_TYPE_LENGTH, 0L);
    }
  }
}

//empties eeprom, only use this function once on initialisation of a new arduino
void eeprom_init() {

  int kmstand = 0;

  EEPROM.put(KM_ADR, kmstand);

  long reset_uid = 0;

  for (int i = 0; i < UID_ARRAY_LENGTH; i++) {
    EEPROM.put((UID_ADR + i * UID_TYPE_LENGTH), reset_uid);
  }
}
