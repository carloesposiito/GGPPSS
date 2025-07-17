#ifndef Screen
#define Screen

#include <U8g2lib.h>

enum Direction
{
    HORIZONTAL_0 = 0,
    RIGHT_90 = 1,
    HORIZONTAL_180,
    LEFT_270 = 3
};

class Display
{
private:

    String _deviceInfo;

    /// @brief Display object.
    U8G2_SH1106_128X64_NONAME_F_HW_I2C _display;
    
public:

    /// @brief Constructor of the class.
    /// @param deviceInfo Reference to device info to show on boot screen.
    Display(String deviceInfo);

    /// @brief Initialized display module.
    void Initialize();

	/// @brief Write text to display.
	/// @param text Text to be displayed.
	/// @param x X position of the text on screen.
	/// @param y Y position of the text on screen.
	/// @param direction Direction of the text.
	void WriteText(String text, int x = 3, int y = 14, Direction direction = HORIZONTAL_0);

	/// @brief Write direction data to display.
	/// @param nextDirection Next direction.
	/// @param nextDirectionDistance Distance to next direction.
	/// @param otherData Other notification data.
	void WriteText(String nextDirection, String nextDirectionDistance, String otherData, Direction direction = HORIZONTAL_0);
};

#endif
