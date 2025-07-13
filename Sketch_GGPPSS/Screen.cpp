#include "Screen.h"

/// @brief Constructor of the class.
/// @param deviceInfo Reference to device info to show on boot screen.
Display::Display(String deviceInfo)
    : _deviceInfo(deviceInfo),
      _display(U8G2_R0, U8X8_PIN_NONE) { }

/// @brief Initialized display module.
void Display::Initialize()
{
    if (_display.begin())
    {
#if DEBUG
        Serial.println(F("Display initializing... SUCCESS"));
#endif
        _display.enableUTF8Print();
        this->WriteText(_deviceInfo, 32, 32);
        delay(3000);
    }
    else
    {
#if DEBUG
        Serial.println(F("Display initializing... FAILED!"));
#endif    
    
        // Stop loop
        while (true) { }
    }
};

/// @brief Write text to display.
/// @param text Text to be displayed.
void Display::WriteText(String text, int x, int y, Direction direction)
{
    _display.clearBuffer();
    _display.setFontDirection(int(direction));
    _display.setFont(u8g2_font_ncenB08_tr);
    _display.drawStr(x, y, text.c_str());
    _display.sendBuffer();
}

/// @brief Write text to display.
/// @param text Text to be displayed.
void Display::WriteText(String nextDirection, String nextDirectionDistance, String arrival, String distanceLeft, String timeLeft)
{
    _display.clearBuffer();
    _display.setFont(u8g2_font_ncenB08_tr);

    _display.drawStr(2, 10, nextDirection.c_str());
    _display.drawStr(2, 20, nextDirectionDistance.c_str());
    _display.drawStr(2, 30, arrival.c_str());
    _display.drawStr(2, 40, distanceLeft.c_str());
    _display.drawStr(2, 50, timeLeft.c_str());

    _display.sendBuffer();
}