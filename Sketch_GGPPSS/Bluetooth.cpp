#include "Bluetooth.h"

#include <ArduinoJson.h>

#define BLE_NAME "GGPPSS_BLE"
#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"

#pragma region "Callbacks"

/// @brief Holds BLE read/write characteristics events. 
class MyCallbacks : public BLECharacteristicCallbacks
{
private:
    BLE* _ble;

public:
    
    /// @brief Constructor
    MyCallbacks(BLE* bleInstance) : _ble(bleInstance) {}

    void onWrite(BLECharacteristic *pRxCharacteristic) override
    {
        std::string rxValueStd = std::string((const char*)pRxCharacteristic->getValue().c_str());
        String rxValue = String(rxValueStd.c_str());

        if (rxValue.length() > 0)
        {
            DynamicJsonDocument doc(256);

            if (!deserializeJson(doc, rxValue.c_str()))
            {
                const char* nextDirection = doc["nD"];
                int nextDirectionDistance = doc["nDD"];
                const char* nextDirectionDistanceUnit = doc["nDDU"];
                const char* arrival = doc["a"];
                const char* distanceLeft = doc["dL"];
                const char* timeLeft = doc["tL"];

#if DEBUG
                Serial.println(String("\"") + nextDirection + "\" in " + String(nextDirectionDistance) + nextDirectionDistanceUnit);
                Serial.println(String("Arrival ") + arrival + " - " + distanceLeft + " left - (" + timeLeft + ")");
                Serial.println(F(""));
#endif

                // Send text to display
                _ble->DisplayObject->WriteText(nextDirection, String(nextDirectionDistance) + nextDirectionDistanceUnit, String(arrival), String(distanceLeft), String(timeLeft));
            }
            else
            {
#if DEBUG
                Serial.println(F("Invalid JSON received:"));
                Serial.println(rxValue);
                Serial.println(F(""));
#endif

                // Send text to display
                _ble->DisplayObject->WriteText(String(rxValue), 3, 14);
            }
        }
    }
};


/// @brief Holds BLE connection callbacks. 
class MyServerCallbacks : public BLEServerCallbacks
{
private:
    BLE *_ble;

public:

    /// @brief Holds BLE callbacks. 
    MyServerCallbacks(BLE *bleInstance) : _ble(bleInstance) { }

    /// @brief Occurs when BLE connection happens.
    void onConnect(BLEServer *pServer) override
    {
        _ble->IsDeviceConnected = true;
#if DEBUG
        Serial.println(F("BLE device connected"));
#endif        
    }

    /// @brief Occurs when BLE disconnection happens.
    void onDisconnect(BLEServer *pServer) override
    {
        _ble->IsDeviceConnected = false;
        pServer->getAdvertising()->start();
#if DEBUG
        Serial.println(F("BLE device disconnected"));
#endif
    }
};

#pragma endregion

/// @brief Constructor of the BLE class.
BLE::BLE(Display *displayObject)
    : IsDeviceConnected(false),
      DisplayObject(displayObject),
      pServer(nullptr),
      pRxCharacteristic(nullptr) {}

void BLE::Initialize()
{
    BLEDevice::init(BLE_NAME);

    // Create server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks(this));
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Create rx characteristic, passa 'this' a MyCallbacks
    pRxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_RX,
        BLECharacteristic::PROPERTY_WRITE);
    pRxCharacteristic->setCallbacks(new MyCallbacks(this));

    // Start service
    pService->start();

    // Configure advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMinPreferred(0x12);

    // Start advertising
    pServer->getAdvertising()->start();

#if DEBUG
    Serial.println(F("BLE initializing... SUCCESS"));
#endif
}