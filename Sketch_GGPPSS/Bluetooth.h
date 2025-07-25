#pragma once

#include "Including.h"
#include "Screen.h"
#include <BLEDevice.h>
#include <BLEServer.h> 
#include <BLEUtils.h>
#include <BLE2902.h>

class BLE
{
private:

    /// @brief Pointer to the BLEServer object to manage BLE server.
    BLEServer *pServer;

    /// @brief BLE characteristic used to receive data from connected device.
    BLECharacteristic *pRxCharacteristic;

public:

    /// @brief Reference to display object, used to show data on screen.
    Display *DisplayObject;

    /// @brief Describes if smartphone is connected through BLE connection.
    bool IsDeviceConnected;

    /// @brief Constructor of the BLE class.
    BLE(Display *displayObject);

    /// @brief Initializes BLE connection.
    void Initialize();
};