package com.example.ggppss

import android.bluetooth.*
import android.bluetooth.le.AdvertiseCallback
import android.bluetooth.le.AdvertiseData
import android.bluetooth.le.AdvertiseSettings
import android.content.Context
import android.os.ParcelUuid
import android.util.Log
import java.util.UUID

class BleServer private constructor() {
    private val SERVICE_UUID_STRING = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
    private val CHARACTERISTIC_UUID_TX_STRING = "6E400003-B5A3-F393-E0A9-E50E24DCCA9E" // notify
    private val CHARACTERISTIC_UUID_RX_STRING = "6E400002-B5A3-F393-E0A9-E50E24DCCA9E" // write

    private val serviceUUID: UUID = UUID.fromString(SERVICE_UUID_STRING)
    private val characteristicUUIDTx: UUID = UUID.fromString(CHARACTERISTIC_UUID_TX_STRING)
    private val characteristicUUIDRx: UUID = UUID.fromString(CHARACTERISTIC_UUID_RX_STRING)

    private var gattServer: BluetoothGattServer? = null
    private var notifyCharacteristic: BluetoothGattCharacteristic? = null
    private var writeCharacteristic: BluetoothGattCharacteristic? = null
    private var connectedDevice: BluetoothDevice? = null

    fun startServer(context: Context) {
        val bluetoothManager = context.getSystemService(Context.BLUETOOTH_SERVICE) as BluetoothManager
        val bluetoothAdapter = bluetoothManager.adapter

        // Creo il servizio
        val gattService = BluetoothGattService(serviceUUID, BluetoothGattService.SERVICE_TYPE_PRIMARY)

        // Caratteristica Notify (TX)
        notifyCharacteristic = BluetoothGattCharacteristic(
            characteristicUUIDTx,
            BluetoothGattCharacteristic.PROPERTY_NOTIFY,
            BluetoothGattCharacteristic.PERMISSION_READ
        )

        // Caratteristica Write (RX)
        writeCharacteristic = BluetoothGattCharacteristic(
            characteristicUUIDRx,
            BluetoothGattCharacteristic.PROPERTY_WRITE or BluetoothGattCharacteristic.PROPERTY_WRITE_NO_RESPONSE,
            BluetoothGattCharacteristic.PERMISSION_WRITE
        )

        gattService.addCharacteristic(notifyCharacteristic)
        gattService.addCharacteristic(writeCharacteristic)

        // Callback server GATT
        gattServer = bluetoothManager.openGattServer(context, object : BluetoothGattServerCallback() {
            override fun onConnectionStateChange(device: BluetoothDevice, status: Int, newState: Int) {
                if (newState == BluetoothProfile.STATE_CONNECTED) {
                    connectedDevice = device
                    Log.d("BLE", "Device connected: ${device.address}")
                } else if (newState == BluetoothProfile.STATE_DISCONNECTED) {
                    connectedDevice = null
                    Log.d("BLE", "Device disconnected: ${device.address}")
                }
            }

            override fun onCharacteristicWriteRequest(
                device: BluetoothDevice?,
                requestId: Int,
                characteristic: BluetoothGattCharacteristic?,
                preparedWrite: Boolean,
                responseNeeded: Boolean,
                offset: Int,
                value: ByteArray?
            ) {
                super.onCharacteristicWriteRequest(device, requestId, characteristic, preparedWrite, responseNeeded, offset, value)
                if (characteristic?.uuid == characteristicUUIDRx && value != null) {
                    val receivedString = String(value)
                    Log.d("BLE", "Received data via RX characteristic: $receivedString")

                    // Qui puoi gestire i dati ricevuti dall'app

                    if (responseNeeded) {
                        gattServer?.sendResponse(device, requestId, BluetoothGatt.GATT_SUCCESS, 0, null)
                    }
                }
            }

            override fun onNotificationSent(device: BluetoothDevice?, status: Int) {
                Log.d("BLE", "Notification sent: status $status")
            }
        })

        gattServer?.addService(gattService)

        // Start advertising
        bluetoothAdapter.bluetoothLeAdvertiser.startAdvertising(
            AdvertiseSettings.Builder()
                .setAdvertiseMode(AdvertiseSettings.ADVERTISE_MODE_LOW_LATENCY)
                .setConnectable(true)
                .build(),
            AdvertiseData.Builder()
                .setIncludeDeviceName(true)
                .addServiceUuid(ParcelUuid(serviceUUID))
                .build(),
            object : AdvertiseCallback() {
                override fun onStartSuccess(settingsInEffect: AdvertiseSettings) {
                    Log.d("BLE", "Advertising started")
                }

                override fun onStartFailure(errorCode: Int) {
                    Log.e("BLE", "Advertising failed: $errorCode")
                }
            }
        )
    }

    fun sendNotification(data: String) {
        val value = data.toByteArray()
        notifyCharacteristic?.value = value
        connectedDevice?.let {
            gattServer?.notifyCharacteristicChanged(it, notifyCharacteristic, false)
        }
    }

    fun stopServer() {
        gattServer?.close()
        gattServer = null
    }

    companion object {
        val instance: BleServer by lazy { BleServer() }
    }
}
