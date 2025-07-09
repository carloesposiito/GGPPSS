#include "Bluetooth.h"

#include <ArduinoJson.h>
#include <string>

#define BLE_NAME "GGPPSS_BLE"
#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"

#pragma region "Callbacks"

/// @brief Callbacks for writing/reading events.
class MyCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pRxCharacteristic)
    {
        String rxValue = pRxCharacteristic->getValue(); 

        if (rxValue.length() > 0)
        {
            DynamicJsonDocument doc(256);
            DeserializationError desError = deserializeJson(doc, rxValue.c_str());

            if (!desError)
            {
                const char *next = doc["next"];
                int distance = doc["distance"];
#if DEBUG
                Serial.println(F("Next istruction: " + String(next)));
                Serial.println(F("Distance: " + String(distance) + " meters"));
                Serial.println(F(""));
#endif
            }
#if DEBUG
            else
            {
                Serial.println(F("Error parsing JSON data"));
                Serial.println(F(""));
            }
#endif
        }
    }
};

/// @brief Callbacks for connecting/disconnecting events.
class MyServerCallbacks : public BLEServerCallbacks
{
private:
    BLE *_ble;

public:
    MyServerCallbacks(BLE *bleInstance) : _ble(bleInstance) {}

    void onConnect(BLEServer *pServer)
    {
        _ble->IsDeviceConnected = true;
    }

    void onDisconnect(BLEServer *pServer)
    {
        _ble->IsDeviceConnected = false;
        pServer->getAdvertising()->start();
    }
};

#pragma endregion

BLE::BLE()
    : IsDeviceConnected(false),
      pServer(nullptr),
      pRxCharacteristic(nullptr) {};

void BLE::Initialize()
{
    BLEDevice::init(BLE_NAME);

    // Crate BLE server and set callbacks for connection/disconnection
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks(this));
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Create BLE characteristic to read data
    pRxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_RX,
        BLECharacteristic::PROPERTY_WRITE);
    pRxCharacteristic->setCallbacks(new MyCallbacks());

    // Start service
    pService->start();

    // Set some connection settings
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMinPreferred(0x12);

    // Disable security
    BLESecurity *pSecurity = new BLESecurity();
    pSecurity->setAuthenticationMode(ESP_LE_AUTH_NO_BOND);
    pSecurity->setCapability(ESP_IO_CAP_NONE);
    pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);

    // Start advertising making device discoverable
    pServer->getAdvertising()->start();
};