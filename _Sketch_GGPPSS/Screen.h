#pragma once

#include <U8g2lib.h>
#include "Including.h"

/// @brief Enum describing text direction (USELESS ATM).
enum Direction
{
    HORIZONTAL_0 = 0,
    RIGHT_90 = 1,
    HORIZONTAL_180,
    LEFT_270 = 3
};

/// @brief Enums describing all type of possible screens.
enum Screens
{
    EMPTY_SCREEN,
    BOOT_PAGE_SCREEN,
    WAITING_PAGE_SCREEN,
    FIRST_PAGE_SCREEN,
    SECOND_PAGE_SCREEN,
    TEXT_PAGE_SCREEN
};

class Display
{
private:

    /// @brief Holds previous millis;
    unsigned long _previousMillis;
    
    /// @brief Holds current displayed screen.
    Screens _currentScreen;
    
    /// @brief Display object.
    U8G2_SH1106_128X64_NONAME_F_HW_I2C _display;
    
    /// @brief Holds previous navigation data.
    NAVIGATION_DATA _previousNavigationData;

    /// @brief Holds current navigation data.
    NAVIGATION_DATA _currentNavigationData;

    /// @brief Displayes a screen.
    /// @param screenToDisplay Screen to be displayed.
    /// @param imageToDisplay Background image to be displayed.
    /// @param refreshDisplay [OPTIONAL] True to refresh display content immediatly.
    void DisplayScreen(Screens screenToDisplay, const unsigned char* imageToDisplay, bool refreshDisplay = false);

    /// @brief Handles timer to change screen to be displayed.
    void CheckScreenToDisplay();

    /// @brief Writes data of first screen to display.
    void WriteFirstScreen();
    
    /// @brief Writes data of second screen to display.
    void WriteSecondScreen();

public:

    /// @brief Constructor of the class.
    Display();

    /// @brief Initializes display module showing boot screen.
    void Initialize();

    /// @brief Displayes waiting for connection screen.
    void DisplayWaitingScreen();

    /// @brief Displayes navigation data switching automatically between two screens.
    void DisplayNavigationData();

    /// @brief Updates navigation data to be displayed.
    /// @param navigationData Updated navigation data.
    void UpdateNavigationData(NAVIGATION_DATA navigationData);

	/// @brief Write text to display.
	/// @param text Text to be displayed.
	/// @param x X position of the text on screen.
	/// @param y Y position of the text on screen.
	/// @param direction Direction of the text.
	void WriteText(String text, int x = 3, int y = 14, Direction direction = HORIZONTAL_0);
};