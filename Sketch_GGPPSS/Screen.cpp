#include "Screen.h"

#define FIRST_SCREEN_DURATION 10000
#define SECOND_SCREEN_DURATION 3000

/// @brief Constructor of the class.
/// @param deviceInfo Reference to device info to show on boot screen.
Display::Display(String deviceInfo)
    : _deviceInfo(deviceInfo),
      _display(U8G2_R0, U8X8_PIN_NONE),
      _previousMillis(0),
      _displayedScreen(EMPTY_SCREEN),
      _screenToBeDisplayed(FIRST_SCREEN) {}

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
        while (true)
        {
        }
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

/// @brief Write notification data to display.
/// @param nextDirection Next direction.
/// @param nextDirectionDistance Distance to next direction.
/// @param otherData Other notification data.
/// @param direction Direction of the text.
void Display::WriteText(String nextDirection, String nextDirectionDistance, String otherData, Direction direction)
{
    // Get current millis
    unsigned long currentMillis = millis();

#pragma region "Check which screen should be displayed"

    switch (_displayedScreen)
    {
    case FIRST_SCREEN:
        if (currentMillis - _previousMillis >= FIRST_SCREEN_DURATION)
        {
            // Change to second screen
            _screenToBeDisplayed = SECOND_SCREEN;
            _previousMillis = currentMillis;
        }
        break;

    case SECOND_SCREEN:
        if (currentMillis - _previousMillis >= SECOND_SCREEN_DURATION)
        {
            // Change to first screen
            _screenToBeDisplayed = FIRST_SCREEN;
            _previousMillis = currentMillis;
        }
        break;

    default:
        // Should never happen
        break;
    }

#pragma endregion

    // Clear screen
    _display.clearBuffer();
    _display.setFontDirection(int(direction));

#pragma region "Display right screen"    

    switch (_screenToBeDisplayed)
    {
    case FIRST_SCREEN:

#pragma region "Next direction"

        _display.setFont(u8g2_font_6x10_tr);

        if (nextDirection.length() > 0)
        {
            uint16_t textWidth = _display.getStrWidth(nextDirection.c_str());

            if (textWidth >= 128)
            {
                int bestSplit = -1;
                int center = nextDirection.length() / 2;
                int closestDistance = nextDirection.length();

                for (int i = 1; i < nextDirection.length() - 1; ++i)
                {
                    if (nextDirection[i] == ' ')
                    {
                        int distance = abs(i - center);
                        if (distance < closestDistance)
                        {
                            closestDistance = distance;
                            bestSplit = i;
                        }
                    }
                }

                if (bestSplit != -1)
                {
                    String line1 = nextDirection.substring(0, bestSplit);
                    String line2 = nextDirection.substring(bestSplit + 1);

                    _display.drawStr(3, 12, line1.c_str());
                    _display.drawStr(3, 26, line2.c_str());

                    _display.setFont(u8g2_font_7x14_tr);
                    _display.drawStr(3, 45, nextDirectionDistance.c_str());
                }
                else
                {
                    _display.drawStr(3, 12, nextDirection.c_str());
                    _display.setFont(u8g2_font_7x14_tr);
                    _display.drawStr(3, 45, nextDirectionDistance.c_str());
                }
            }
            else
            {
                _display.drawStr(3, 12, nextDirection.c_str());

                _display.setFont(u8g2_font_7x14_tr);
                _display.drawStr(3, 31, nextDirectionDistance.c_str());
            }
        }

#pragma endregion

        break;

    case SECOND_SCREEN:

#pragma region "Arrival time, total distance and time left"

        _display.setFont(u8g2_font_5x7_tr);
        _display.drawStr(3, 61, otherData.c_str());

#pragma endregion
        break;

    default:
        break;
    }

    _displayedScreen = _screenToBeDisplayed;
    _display.sendBuffer();

#pragma endregion
}
