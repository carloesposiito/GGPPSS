#include "Including.h"

String _deviceInfo = "GGPPSS Alpha";
Display _display(_deviceInfo);
BLE _ble(&_display);
int counter = 0;

void setup()
{
#if DEBUG
  Serial.begin(115200);
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
#if DEBUG
  //_display.WriteText(String(++counter), 32, 32, HORIZONTAL_0);
#endif

  delay(1000);
}
