#include "Bluetooth.h"

#include <ArduinoJson.h>

#define BLE_NAME "GGPPSS_BLE"
#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"

#pragma region "Callbacks"

class MyCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pRxCharacteristic) override
{
    std::string rxValueStd = std::string((const char*)pRxCharacteristic->getValue().c_str());
    String rxValue = String(rxValueStd.c_str());  // conversione corretta

    if (rxValue.length() > 0)
    {
        DynamicJsonDocument doc(256);
        DeserializationError desError = deserializeJson(doc, rxValue.c_str());

        if (!desError)
        {
            const char* text = doc["text"];
            const char* unit = doc["unit"];
            const char* arrival = doc["arrival"];
            const char* left = doc["left"];
            const char* time = doc["time"];
            int distance = doc["distance"];

#if DEBUG
            Serial.print(F("Text: "));
            Serial.println(text);
            Serial.print(F("Distance: "));
            Serial.println(distance);
            Serial.print(F("Unit: "));
            Serial.println(unit);
            Serial.print(F("Arrival: "));
            Serial.println(arrival);
            Serial.print(F("Left: "));
            Serial.println(left);
            Serial.print(F("Time: "));
            Serial.println(time);
            Serial.println();
#endif
        }
#if DEBUG
        else
        {
            Serial.println(F("Errore nel parsing del JSON"));
            Serial.print(F("Dato ricevuto: "));
            Serial.println(rxValue);  // Mostra la stringa che ha fallito
            Serial.println();
        }
#endif
    }
}


};

class MyServerCallbacks : public BLEServerCallbacks {
private:
    BLE *_ble;

public:
    MyServerCallbacks(BLE *bleInstance) : _ble(bleInstance) {
#if DEBUG
        Serial.println(F("MyServerCallbacks created"));
#endif
    }

    void onConnect(BLEServer *pServer) override {
#if DEBUG
        Serial.println(F("BLE device connected"));
#endif
        _ble->IsDeviceConnected = true;
    }

    void onDisconnect(BLEServer *pServer) override {
#if DEBUG
        Serial.println(F("BLE device disconnected"));
#endif
        _ble->IsDeviceConnected = false;
        pServer->getAdvertising()->start();
    }
};


#pragma endregion

BLE::BLE()
    : IsDeviceConnected(false),
      pServer(nullptr),
      pRxCharacteristic(nullptr) {}

void BLE::Initialize()
{
    BLEDevice::init(BLE_NAME);

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks(this));
    BLEService *pService = pServer->createService(SERVICE_UUID);

    pRxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_RX,
        BLECharacteristic::PROPERTY_WRITE);
    pRxCharacteristic->setCallbacks(new MyCallbacks());

    pService->start();

    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMinPreferred(0x12);

    //BLESecurity *pSecurity = new BLESecurity();
   // pSecurity->setAuthenticationMode(ESP_LE_AUTH_NO_BOND);
    //pSecurity->setCapability(ESP_IO_CAP_NONE);
    //pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);

    pServer->getAdvertising()->start();

    #if DEBUG
    Serial.println(F("BLE init complete"));
#endif
}
