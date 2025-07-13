package com.example.ggppss

import android.bluetooth.*
import android.bluetooth.le.*
import android.content.Context
import android.content.pm.PackageManager
import android.os.Build
import android.os.Handler
import android.os.Looper
import android.util.Log
import androidx.core.content.ContextCompat
import java.util.*
import android.os.ParcelUuid

class BleClient(private val context: Context) {

    companion object {
        val NUS_SERVICE_UUID: UUID = UUID.fromString("6E400001-B5A3-F393-E0A9-E50E24DCCA9E")
        val NUS_CHAR_TX_UUID: UUID = UUID.fromString("6E400003-B5A3-F393-E0A9-E50E24DCCA9E") // notify from server to client
        val NUS_CHAR_RX_UUID: UUID = UUID.fromString("6E400002-B5A3-F393-E0A9-E50E24DCCA9E") // write from client to server
        val CLIENT_CHARACTERISTIC_CONFIG_UUID: UUID = UUID.fromString("00002902-0000-1000-8000-00805f9b34fb")
        private const val TAG = "BleClient"
        private const val MAX_MTU_SIZE = 20
    }

    private val bluetoothManager = context.getSystemService(Context.BLUETOOTH_SERVICE) as BluetoothManager
    private val bluetoothAdapter = bluetoothManager.adapter
    private var bluetoothGatt: BluetoothGatt? = null
    private var scanCallback: ScanCallback? = null
    private var deviceToConnect: BluetoothDevice? = null

    var onDataReceived: ((String) -> Unit)? = null
    var onConnected: (() -> Unit)? = null
    var onDisconnected: (() -> Unit)? = null
    var onWriteComplete: ((Boolean) -> Unit)? = null

    private val handler = Handler(Looper.getMainLooper())

    private fun hasPermissions(): Boolean {
        return if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            ContextCompat.checkSelfPermission(context, android.Manifest.permission.BLUETOOTH_SCAN) == PackageManager.PERMISSION_GRANTED &&
                    ContextCompat.checkSelfPermission(context, android.Manifest.permission.BLUETOOTH_CONNECT) == PackageManager.PERMISSION_GRANTED
        } else {
            ContextCompat.checkSelfPermission(context, android.Manifest.permission.ACCESS_FINE_LOCATION) == PackageManager.PERMISSION_GRANTED
        }
    }

    fun startScan() {
        if (!hasPermissions()) {
            Log.e(TAG, "Missing required permissions to start BLE scan")
            return
        }

        val scanner = bluetoothAdapter.bluetoothLeScanner ?: run {
            Log.e(TAG, "BluetoothLeScanner is null")
            return
        }

        val filters = listOf(
            ScanFilter.Builder()
                .setServiceUuid(ParcelUuid(NUS_SERVICE_UUID))
                .build()
        )
        val settings = ScanSettings.Builder()
            .setScanMode(ScanSettings.SCAN_MODE_LOW_LATENCY)
            .build()

        scanCallback = object : ScanCallback() {
            override fun onScanResult(callbackType: Int, result: ScanResult) {
                val device = result.device
                Log.d(TAG, "Found device: ${device.name} - ${device.address}")
                if (device.name != null && device.name.startsWith("ESP32")) {
                    Log.d(TAG, "Target device found, stopping scan and connecting")
                    stopScan()
                    connectToDevice(device)
                }
            }

            override fun onScanFailed(errorCode: Int) {
                Log.e(TAG, "Scan failed with code $errorCode")
            }
        }

        scanner.startScan(filters, settings, scanCallback)
        Log.d(TAG, "Started BLE scan")
    }

    fun stopScan() {
        bluetoothAdapter.bluetoothLeScanner?.stopScan(scanCallback)
        scanCallback = null
        Log.d(TAG, "Stopped BLE scan")
    }

    private fun connectToDevice(device: BluetoothDevice) {
        deviceToConnect = device
        bluetoothGatt = device.connectGatt(context, false, gattCallback)
        Log.d(TAG, "Connecting to GATT server...")
    }

    private val gattCallback = object : BluetoothGattCallback() {

        override fun onConnectionStateChange(gatt: BluetoothGatt, status: Int, newState: Int) {
            if (status == BluetoothGatt.GATT_SUCCESS) {
                if (newState == BluetoothProfile.STATE_CONNECTED) {
                    Log.d(TAG, "Connected to GATT server, discovering services...")
                    handler.post {
                        onConnected?.invoke()
                    }
                    gatt.discoverServices()
                } else if (newState == BluetoothProfile.STATE_DISCONNECTED) {
                    Log.d(TAG, "Disconnected from GATT server")
                    handler.post {
                        onDisconnected?.invoke()
                    }
                }
            } else {
                Log.e(TAG, "Connection state change error: $status")
                handler.post {
                    onDisconnected?.invoke()
                }
            }
        }

        override fun onServicesDiscovered(gatt: BluetoothGatt, status: Int) {
            if (status == BluetoothGatt.GATT_SUCCESS) {
                Log.d(TAG, "Services discovered")
                val service = gatt.getService(NUS_SERVICE_UUID)
                if (service != null) {
                    val txChar = service.getCharacteristic(NUS_CHAR_TX_UUID)
                    if (txChar != null) {
                        Log.d(TAG, "TX characteristic found, enabling notifications")
                        val notificationSet = gatt.setCharacteristicNotification(txChar, true)
                        if (!notificationSet) {
                            Log.e(TAG, "Failed to set characteristic notification")
                            return
                        }
                        val descriptor = txChar.getDescriptor(CLIENT_CHARACTERISTIC_CONFIG_UUID)
                        if (descriptor != null) {
                            descriptor.value = BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE
                            val writeDescStarted = gatt.writeDescriptor(descriptor)
                            if (!writeDescStarted) {
                                Log.e(TAG, "Failed to write descriptor to enable notifications")
                            }
                        } else {
                            Log.e(TAG, "Descriptor for notifications not found")
                        }
                    } else {
                        Log.e(TAG, "TX characteristic not found")
                    }
                } else {
                    Log.e(TAG, "NUS service not found")
                }
            } else {
                Log.e(TAG, "Service discovery failed with status $status")
            }
        }

        override fun onCharacteristicChanged(gatt: BluetoothGatt, characteristic: BluetoothGattCharacteristic) {
            if (characteristic.uuid == NUS_CHAR_TX_UUID) {
                val data = characteristic.value
                val message = data.toString(Charsets.UTF_8)
                Log.d(TAG, "Notification received: $message")
                handler.post {
                    onDataReceived?.invoke(message)
                }
            }
        }

        override fun onCharacteristicWrite(gatt: BluetoothGatt, characteristic: BluetoothGattCharacteristic, status: Int) {
            if (characteristic.uuid == NUS_CHAR_RX_UUID) {
                if (status == BluetoothGatt.GATT_SUCCESS) {
                    Log.d(TAG, "Characteristic write successful")
                    handler.post {
                        onWriteComplete?.invoke(true)
                    }
                } else {
                    Log.e(TAG, "Characteristic write failed with status $status")
                    handler.post {
                        onWriteComplete?.invoke(false)
                    }
                }
            }
        }

        override fun onDescriptorWrite(gatt: BluetoothGatt, descriptor: BluetoothGattDescriptor, status: Int) {
            if (status == BluetoothGatt.GATT_SUCCESS) {
                Log.d(TAG, "Descriptor write successful")
            } else {
                Log.e(TAG, "Descriptor write failed with status $status")
            }
        }
    }

    // Simple fragmentation to send data in chunks <= 20 bytes
    fun sendData(data: String) {
        val gatt = bluetoothGatt ?: run {
            Log.e(TAG, "BluetoothGatt is null, can't send data")
            return
        }
        val service = gatt.getService(NUS_SERVICE_UUID) ?: run {
            Log.e(TAG, "NUS Service not found, can't send data")
            return
        }
        val rxChar = service.getCharacteristic(NUS_CHAR_RX_UUID) ?: run {
            Log.e(TAG, "RX characteristic not found, can't send data")
            return
        }

        val properties = rxChar.properties
        if ((properties and BluetoothGattCharacteristic.PROPERTY_WRITE) == 0 &&
            (properties and BluetoothGattCharacteristic.PROPERTY_WRITE_NO_RESPONSE) == 0) {
            Log.e(TAG, "RX characteristic does not support write")
            return
        }

        val bytes = data.toByteArray(Charsets.UTF_8)
        if (bytes.size <= MAX_MTU_SIZE) {
            rxChar.value = bytes
            val writeStarted = gatt.writeCharacteristic(rxChar)
            if (!writeStarted) {
                Log.e(TAG, "writeCharacteristic() returned false")
            } else {
                Log.d(TAG, "Sent data: $data")
            }
        } else {
            Log.d(TAG, "Data length (${bytes.size}) exceeds $MAX_MTU_SIZE bytes, sending in chunks")
            var offset = 0
            while (offset < bytes.size) {
                val chunkSize = minOf(MAX_MTU_SIZE, bytes.size - offset)
                val chunk = bytes.copyOfRange(offset, offset + chunkSize)
                rxChar.value = chunk
                val writeStarted = gatt.writeCharacteristic(rxChar)
                if (!writeStarted) {
                    Log.e(TAG, "writeCharacteristic() returned false for chunk at offset $offset")
                    break
                }
                // Wait a bit between writes (could be improved with a queue + callback)
                Thread.sleep(50)
                offset += chunkSize
            }
        }
    }

    fun disconnect() {
        bluetoothGatt?.let {
            it.disconnect()
            it.close()
        }
        bluetoothGatt = null
        deviceToConnect = null
        Log.d(TAG, "Disconnected from device")
    }
}
