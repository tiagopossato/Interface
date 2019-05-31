#include "interface.h"

Resource hall;
Resource hallAdjustRSC;

float tempAdust = 0;

uint8_t setHallAdjust(char * value) {
  tempAdust = (float) strtod(value, NULL);
  return true;
}
uint8_t getHallAdjust(char *buffer, uint8_t bufferSize) {
  snprintf(buffer, bufferSize, "%.2f", tempAdust);
  return true;
}

uint8_t getHall(char *buffer, uint8_t bufferSize) {

  snprintf(buffer, bufferSize, "%.2f", hallRead() + tempAdust);
  return true;
}

void initIHM() {
  beginInterface();
  registerResource(&hall, "hall", getHall, NULL, 0);
  registerResource(&hallAdjustRSC, "hallAdjust", getHallAdjust, setHallAdjust, 0);
}
