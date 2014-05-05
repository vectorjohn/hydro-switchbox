
#define EEPROM_END 511

unsigned int eepromOffset;

void updateTimestamp( unsigned long now ) {
}

/**
 * Look in EEPROM for the current time.
 * If the init timestamp in EEPROM differs
 * from the compile timestamp, assume reprogram.
 * Returns the current time offset.
 */
unsigned long initCurrentTime() {
    return 0L;
}
