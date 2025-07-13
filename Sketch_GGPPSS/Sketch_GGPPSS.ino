#include "Including.h"

String _deviceInfo = "GGPPSS Alpha";
Display _display(_deviceInfo);
BLE _ble;
int counter = 0;

void setup()
{
#if DEBUG
  Serial.begin(115200);
  Serial.println(F("INITIALIZE STARTED"));
  Serial.println(F(""));
#endif

  _display.Initialize();
  Serial.println("AA");
  _ble.Initialize();
Serial.println("BB");

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
