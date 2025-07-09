#include "Screen.h"

Display::Display(String deviceInfo)
    : _deviceInfo(deviceInfo),
      _display(U8G2_R0, U8X8_PIN_NONE)
{
}

void Display::Initialize()
{
    if (_display.begin())
    {
        _display.enableUTF8Print();
        this->WriteText(_deviceInfo, 32, 32);
        delay(5000);
    }
    else
    {
#if DEBUG
        Serial.println(F("Display error!"));
        
        // Stop loop
        while (true) { }
#endif
    }
};

void Display::WriteText(String text, int x, int y, Direction direction)
{
    _display.clearBuffer();
    _display.setFontDirection(int(direction));
    _display.setFont(u8g2_font_ncenB08_tr);
    _display.drawStr(x, y, text.c_str());
    _display.sendBuffer();
}