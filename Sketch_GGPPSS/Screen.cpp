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
/// @param x X position of the text on screen.
/// @param y Y position of the text on screen.
/// @param direction Direction of the text.
void Display::WriteText(String text, int x, int y, Direction direction)
{
    _display.clearBuffer();
    _display.setFontDirection(int(direction));
    _display.setFont(u8g2_font_5x7_tr);

    _display.drawStr(x, y, text.c_str());
    
    _display.sendBuffer();
}

/// @brief Write direction data to display.
/// @param nextDirection Next direction.
/// @param nextDirectionDistance Distance to next direction.
/// @param arrivalTime Destination arrival time.
/// @param distanceLeft Distance left to destination.
/// @param timeLeft Time left to destination.
/// @param direction Direction of the text.
void Display::WriteText(String nextDirection, String nextDirectionDistance, String arrivalTime, String distanceLeft, String timeLeft, Direction direction)
{
    _display.clearBuffer();
    _display.setFontDirection(int(direction));

    #pragma region "Next direction"

    // Set right text size
    // Use u8g2_font_ncenB10_tr for a bigger text
    _display.setFont(u8g2_font_ncenB08_tr);

    _display.drawStr(3, 14, nextDirection.c_str());
    _display.drawStr(3, 27, nextDirectionDistance.c_str());
    
    #pragma endregion

    #pragma region "Arrival time"

    // Set smallest text size
    _display.setFont(u8g2_font_5x7_tr);
    _display.drawStr(3, 48, arrivalTime.c_str());

    #pragma endregion

    #pragma region "Total distance and time left"

    // Set smallest text size
    _display.setFont(u8g2_font_5x7_tr);
    
    // Build string to be displayed then display it
    String otherDetails = distanceLeft + " (" + timeLeft + ")";    
    _display.drawStr(3, 58, otherDetails.c_str());

    #pragma endregion

    // Send buffer to display
    _display.sendBuffer();
}
