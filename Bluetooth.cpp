#include "Bluetooth.h"

#include "Sounds.h"
#include <ArduinoJson.h>
#include "esp_system.h"
#include "Storage.h"

#define BLE_LED 8

// Define UUIDS
#define SERVICE_UUID "6e400001-b5a3-f393-e0a9-e50e24dcca9e"
#define CHARACTERISTIC_UUID_RX "6e400002-b5a3-f393-e0a9-e50e24dcca9e"
#define CHARACTERISTIC_UUID_TX "6e400003-b5a3-f393-e0a9-e50e24dcca9e"
#define CONFIGURATION_CHAR_UUID "12344321-1221-1221-1221-abc987654321"
#define BT_STATUS_CHAR_UUID "87654321-4321-2222-4334-abc987654321"

#if USING_DISPLAY
#define BLE_NAME "GGPPSS_v0.1"
#else
#define BLE_NAME "VARIOglide_BLE"
#endif

#pragma region "Callbacks"

/// @brief Callback for all events.
class MyCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic) {}
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
        Buzzer::Sound(COMMAND_SUCCEEDED);
    }

    void onDisconnect(BLEServer *pServer)
    {
        _ble->IsDeviceConnected = false;
        _ble->StartAdvertising();
        Buzzer::Sound(COMMAND_FAILED);
    }
};

/// @brief Callback for editing variometer configuration event.
class SetConfigurationCallback : public BLECharacteristicCallbacks
{
private:
    BLE *_ble;

public:
    SetConfigurationCallback(BLE *bleInstance) : _ble(bleInstance) {}

    void onWrite(BLECharacteristic *pCharacteristic) override
    {
        String str = pCharacteristic->getValue();
        if (str.length() > 0)
        {
            float values[5];

#pragma region "Split values"

            int index = 0;
            int startIndex = 0;
            int endIndex = str.indexOf('&');

            while (endIndex != -1 && index < 4)
            {
                String part = str.substring(startIndex, endIndex);
                values[index] = part.toFloat();
                index++;

                startIndex = endIndex + 1;
                endIndex = str.indexOf('&', startIndex);
            }

            if (index < 4)
            {
                String part = str.substring(startIndex);
                values[index] = part.toFloat();
            }

#pragma endregion

#pragma region "Verify and assign parsed values, the save configuration"

            // Verify and assign new ascent alarm value
            // Make sure that its value is at least 0.1
            // Trasform to absolute value in order to avoid problems
            float absoluteAscentThreshold = fabs(values[0]);
            _ble->_variometerConfiguration->ascentAlarmThreshold = (absoluteAscentThreshold < 0.1) ? 0.1 : absoluteAscentThreshold;           
            
            // Verify and assing new sink alarm value
            // Make sure that its value is at least -0.5
            // Transform to absolute value in order to avoid problems
            float absoluteSinkThreshold = fabs(values[1]);
            _ble->_variometerConfiguration->sinkAlarmThreshold = (absoluteSinkThreshold < 0.5) ? -0.5 : -absoluteSinkThreshold;

            // Verify and assign new sensor detection interval value
            // It must be at least 750ms
            _ble->_variometerConfiguration->sensorDetectionInterval = (values[2] < 750) ? 750 : values[2];

            // Verify and assign new sound mode
            _ble->_variometerConfiguration->progressiveSoundEnabled = (values[3] == 1) ? true : false;

            Configuration::SaveConfiguration(_ble->_variometerConfiguration);

#pragma endregion

            Buzzer::Sound(COMMAND_SUCCEEDED);
        }
    }
};

/// @brief Callback for turning off BLE connection event.
class SetBleStatusCallback : public BLECharacteristicCallbacks
{
private:
    BLE *_ble;

public:
    SetBleStatusCallback(BLE *bleInstance) : _ble(bleInstance) {}

    void onWrite(BLECharacteristic *pCharacteristic) override
    {
        String value = pCharacteristic->getValue();
        value.toUpperCase();

        if (value.length() > 0 && value == "OFF")
        {
            _ble->StopAdvertising();
        }
    }
};

#pragma endregion

BLE::BLE(VARIOMETER_CONFIGURATION *variometerConfiguration)
    : _variometerConfiguration(variometerConfiguration),
    IsDeviceConnected(false),  
    pServer(nullptr),
    pRxCharacteristic(nullptr),
    pTxCharacteristic(nullptr),
    pGetSetConfCharacteristic(nullptr),
    pGetSetBleCharacteristic(nullptr) {};

void BLE::Initialize()
{
    // Initialize BLE connection
    pServer = NULL;
    BLEDevice::init(BLE_NAME);
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks(this));
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Create RX characteristic
    pRxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);
    pRxCharacteristic->setCallbacks(new MyCallbacks());

    // Create TX characteristic for XCTrack sentence
    pTxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);
    pTxCharacteristic->addDescriptor(new BLE2902());

    // Add reading/writing configuration characteristic and assign its callback
    pGetSetConfCharacteristic = pService->createCharacteristic(CONFIGURATION_CHAR_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);
    pGetSetConfCharacteristic->setCallbacks(new SetConfigurationCallback(this));
    pGetSetConfCharacteristic->setValue((String)_variometerConfiguration->ascentAlarmThreshold + "&" + (String)_variometerConfiguration->sinkAlarmThreshold + "&" + (String)_variometerConfiguration->sensorDetectionInterval + "&" + (_variometerConfiguration->progressiveSoundEnabled ? "1" : "0"));
    pGetSetConfCharacteristic->addDescriptor(new BLE2902());

    // Add reading/writing bluetooth characteristic
    pGetSetBleCharacteristic = pService->createCharacteristic(BT_STATUS_CHAR_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);
    pGetSetBleCharacteristic->setCallbacks(new SetBleStatusCallback(this));
    pGetSetBleCharacteristic->setValue("ON");
    pGetSetBleCharacteristic->addDescriptor(new BLE2902());

    // Disable security
    BLESecurity *pSecurity = new BLESecurity();
    pSecurity->setAuthenticationMode(ESP_LE_AUTH_NO_BOND);
    pSecurity->setCapability(ESP_IO_CAP_NONE);
    pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);

    pService->start();
    this->StartAdvertising();
};

void BLE::StartAdvertising()
{
    pServer->getAdvertising()->start();
}

void BLE::StopAdvertising()
{
    // Update device connected status
    IsDeviceConnected = false;
    Buzzer::Sound(COMMAND_FAILED);

#if DEBUG_MODE
    int connectedDevicesCount = pServer->getConnectedCount();
    if (connectedDevicesCount > 0)
    {
        Serial.println(F(""));
        Serial.print(String(connectedDevicesCount));
        Serial.println(F(" Bluetooth devices will be disconnected..."));
        Serial.println(F(""));
        pServer->disconnect(0);
    }
#else
    if (pServer->getConnectedCount() > 0)
    {
        pServer->disconnect(0);
    }
#endif

    BLEDevice::getAdvertising()->stop();
    BLEDevice::deinit();
};

void BLE::SendSentence_PRS(int currentPressure)
{
    char _outString[16];
    snprintf(_outString, sizeof(_outString), "PRS %X\n", currentPressure);
    this->SendSentence(_outString);
};

#if SEND_TIME_SINCE_STARTUP
void BLE::SendSentence_XCTOD(unsigned long workingTime)
{
    unsigned int totalMinutes = workingTime / 60000;
    unsigned int hours = totalMinutes / 60;
    unsigned int minutes = totalMinutes % 60;

    char _outString[20];  
    snprintf(_outString, sizeof(_outString), "$XCTOD,%uh%02um\n", hours, minutes);
    this->SendSentence(_outString);
}
#endif

void BLE::SendSentence(const char *str)
{
#if DEBUG_MODE
    Serial.println(str);
#endif
    pTxCharacteristic->setValue((uint8_t *)str, strlen(str));
    pTxCharacteristic->notify();
};