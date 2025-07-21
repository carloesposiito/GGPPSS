#include "Including.h"
#include "Screen.h"
#include "Bluetooth.h"

Display _display;
BLE _ble(&_display);

void setup()
{
#if DEBUG
  Serial.begin(115200);
  Serial.println(F("--- SETUP STARTED ---"));
#endif

  _ble.Initialize();
  _display.Initialize();

  // Keep boot image displayed for 3 seconds
  delay(3000);

  _display.DisplayWaitingScreen();

#if DEBUG
  Serial.println(F("--- SETUP ENDED ---"));
  Serial.println(F(""));
#endif
}

void loop()
{
  if (_ble.IsDeviceConnected)
  {
    _display.DisplayNavigationData();
  }
#if DEBUG
  else
  {
    Serial.println(F("Waiting for connection..."));
  }  
#endif
}
