#ifndef Bluetooth
#define Bluetooth

// Include native BLE libraries
#include <BLEDevice.h>
#include <BLEServer.h> 
#include <BLEUtils.h>
#include <BLE2902.h>
#include "Including.h"


class BLE
{
private:

    /// @brief Pointer to the BLEServer object to manage BLE server.
    BLEServer *pServer;

    /// @brief BLE characteristic used to receive data from connected device.
    BLECharacteristic *pRxCharacteristic;

public:

    /// @brief Describes if variometer is connected through BLE connection.
    bool IsDeviceConnected;

    /// @brief Constructor of the BLE class.
    BLE();

    /// @brief Initializes BLE connection.
    void Initialize();
};

#endif 