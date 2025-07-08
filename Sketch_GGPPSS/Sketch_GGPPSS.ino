#include "Including.h"

String _deviceInfo = "GGPPSS v0.1";
Display _display(_deviceInfo);
int counter = 0;

void setup() {

#if DEBUG
  Serial.begin(9600);
  Serial.print(F("INITIALIZE STARTED"));
  Serial.print(F(""));
#endif

  _display.Initialize();

#if DEBUG
  Serial.print(F("INITIALIZE ENDED"));
  Serial.print(F(""));
#endif
}

void loop() {
  _display.WriteText(String(++counter), 32, 32, HORIZONTAL_0);
  delay(1000);
}
