#include "Including.h"

String _deviceInfo = "GGPPSS Alpha";
Display _display(_deviceInfo);
BLE _ble(&_display);

void setup()
{
#if DEBUG
  Serial.begin(115200);
  Serial.println(F("--- SETUP STARTED ---"));
#endif

  _display.Initialize();
  _ble.Initialize();

#if DEBUG
  Serial.println(F("--- SETUP ENDED ---"));
  Serial.println(F(""));
#endif
}

void loop()
{
  if (!_ble.IsDeviceConnected)
  {
    _display.WriteText("Waiting for connection...");
  } 
}
