#include "Including.h"

String _deviceInfo = "GGPPSS Alpha";
Display _display(_deviceInfo);
BLE _ble;
int counter = 0;

void setup()
{
#if DEBUG
  Serial.begin(9600);
  Serial.println(F("INITIALIZE STARTED"));
  Serial.println(F(""));
#endif

  _display.Initialize();
  _ble.Initialize();

#if DEBUG
  Serial.println(F("INITIALIZE ENDED"));
  Serial.println(F(""));
#endif
}

void loop()
{
  _display.WriteText(String(++counter), 32, 32, HORIZONTAL_0);
  delay(1000);
}
