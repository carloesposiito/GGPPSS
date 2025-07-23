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

            // Create notification data structure with default values
            NAVIGATION_DATA navigationData;

            if (!deserializeJson(doc, rxValue.c_str()))
            {
                // If data a value is not found it will be replaced by an empty string    
                navigationData.nextDirection = String(doc["nD"] | "");
                navigationData.nextDirectionDistance = String(doc["nDD"] | "");
                navigationData.arrivalTime = String(doc["aT"] | "");
                navigationData.leftData = String(doc["l"] | "");
#if DEBUG
                Serial.println(F("Received JSON:"));
                Serial.println("\"" + navigationData.nextDirection + "\" in " + navigationData.nextDirectionDistance);
                Serial.println(navigationData.arrivalTime + " (" + navigationData.leftData + ")");
                Serial.println(F(""));
#endif          
            }
            else
            {
                navigationData.isValid = false;
#if DEBUG
                Serial.println(F("Received not valid JSON:"));
                Serial.println(rxValue);
                Serial.println(F(""));
#endif   
            }   
            
            // Refresh notification data in display object properties
            _ble->DisplayObject->UpdateNavigationData(navigationData);
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