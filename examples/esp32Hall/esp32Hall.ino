#include "interface.h"

void setup() {
  Serial.begin(115200);
  initIHM();
}

void loop() {
  // put your main code here, to run repeatedly:

  while (Serial.available())
  {
    char inChar = (char)Serial.read();

    if (inChar != '\n' && inChar != '\r')
    {
      sprintf(INTERFACE.buffer, "%s%c", INTERFACE.buffer, inChar);
    }
    if (inChar == '\n' || strlen(INTERFACE.buffer) == INPUT_STRING_LENGTH - 1)
    {
      if (strlen(INTERFACE.buffer) >= 1)
      {
        INTERFACE.execute(INTERFACE.buffer);
        if (strlen(INTERFACE.buffer) > 0) Serial.println(INTERFACE.buffer);
      }
      memset(INTERFACE.buffer, '\0', INPUT_STRING_LENGTH);
    }

  }
}
