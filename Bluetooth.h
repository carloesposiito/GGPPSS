#ifndef Bluetooth
#define Bluetooth

#include "HardwareConfiguration.h"
#include "Structures.h"

// Include native BLE libraries
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

    /// @brief BLE characteristic to transmit data to connected device.
    BLECharacteristic *pTxCharacteristic; 

    /// @brief Characteristic related to get or set variometer parameters.
    BLECharacteristic *pGetSetConfCharacteristic;

    /// @brief BLE characteristic used to manage the shutdown of the BLE connection.
    BLECharacteristic *pGetSetBleCharacteristic;

    /// @brief Sends a string to connected BLE device.
    /// @param str String to be sent.
    void SendSentence(const char *str);

public:

    /// @brief Describes if variometer is connected through BLE connection.
    bool IsDeviceConnected;

    /// @brief Constructor of the BLE class.
    BLE();

    /// @brief Initializes BLE connection.
    void Initialize();
    
    /// @brief Starts BLE advertising and makes BLE device discoverable.
    void StartAdvertising();

    /// @brief Stops BLE advertising and turns off BLE connection.
    void StopAdvertising();

    /// @brief Reads data received from BLE.
    String ReadData();
};

#endif 