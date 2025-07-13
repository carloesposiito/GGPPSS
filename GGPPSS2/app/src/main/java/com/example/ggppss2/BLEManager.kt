package com.example.ggppss2.ble

import android.bluetooth.*
import android.bluetooth.le.*
import android.content.Context
import android.os.Handler
import android.os.Looper
import android.util.Log
import androidx.compose.runtime.MutableState
import androidx.compose.runtime.State
import androidx.compose.runtime.mutableStateOf
import java.util.*

class BLEManager(private val context: Context) {

    private var bluetoothGatt: BluetoothGatt? = null
    private val bluetoothAdapter: BluetoothAdapter? = BluetoothAdapter.getDefaultAdapter()
    private val bluetoothLeScanner: BluetoothLeScanner? = bluetoothAdapter?.bluetoothLeScanner

    // Stato interno mutabile, non visibile all'esterno
    private val _isConnected: MutableState<Boolean> = mutableStateOf(false)
    // Stato pubblico solo lettura, da usare in Compose
    val isConnected: State<Boolean> get() = _isConnected

    private var scanning = false
    private val handler = Handler(Looper.getMainLooper())

    private val SCAN_PERIOD: Long = 10000 // 10 sec scan

    private var targetDeviceName: String? = null

    private val SERVICE_UUID = UUID.fromString("6E400001-B5A3-F393-E0A9-E50E24DCCA9E")
    private val CHARACTERISTIC_UUID_RX = UUID.fromString("6E400002-B5A3-F393-E0A9-E50E24DCCA9E")

    private var rxCharacteristic: BluetoothGattCharacteristic? = null

    private val scanCallback = object : ScanCallback() {
        override fun onScanResult(callbackType: Int, result: ScanResult?) {
            super.onScanResult(callbackType, result)
            result?.device?.let { device ->
                Log.d("BLEManager", "Found device: ${device.name} - ${device.address}")
                if (device.name != null && device.name == targetDeviceName) {
                    stopScan()
                    connect(device)
                }
            }
        }

        override fun onScanFailed(errorCode: Int) {
            Log.e("BLEManager", "Scan failed with error: $errorCode")
        }
    }

    fun startScan(deviceName: String) {
        if (scanning) return
        targetDeviceName = deviceName

        bluetoothLeScanner ?: run {
            Log.e("BLEManager", "BluetoothLeScanner non disponibile")
            return
        }

        handler.postDelayed({
            if (scanning) {
                stopScan()
                Log.d("BLEManager", "Scan timeout, dispositivo non trovato")
            }
        }, SCAN_PERIOD)

        scanning = true

        val scanFilter = ScanFilter.Builder()
            .setDeviceName(deviceName)
            .build()

        val scanSettings = ScanSettings.Builder()
            .setScanMode(ScanSettings.SCAN_MODE_LOW_LATENCY)
            .build()

        bluetoothLeScanner?.startScan(listOf(scanFilter), scanSettings, scanCallback)
        Log.d("BLEManager", "Scan started for device: $deviceName")
    }

    fun stopScan() {
        if (!scanning) return
        bluetoothLeScanner?.stopScan(scanCallback)
        scanning = false
        Log.d("BLEManager", "Scan stopped")
    }

    fun connect(device: BluetoothDevice) {
        bluetoothGatt = device.connectGatt(context, false, gattCallback)
        Log.d("BLEManager", "Connecting to device: ${device.name}")
    }

    fun disconnect() {
        bluetoothGatt?.apply {
            disconnect()
            close()
        }
        bluetoothGatt = null
        _isConnected.value = false
        Log.d("BLEManager", "Disconnected from device")
    }

    private val gattCallback = object : BluetoothGattCallback() {

        override fun onConnectionStateChange(gatt: BluetoothGatt, status: Int, newState: Int) {
            super.onConnectionStateChange(gatt, status, newState)
            when (newState) {
                BluetoothProfile.STATE_CONNECTED -> {
                    Log.d("BLEManager", "Connected to GATT server.")
                    _isConnected.value = true
                    gatt.discoverServices()
                }
                BluetoothProfile.STATE_DISCONNECTED -> {
                    Log.d("BLEManager", "Disconnected from GATT server.")
                    _isConnected.value = false
                    bluetoothGatt?.close()
                    bluetoothGatt = null
                }
            }
        }

        override fun onServicesDiscovered(gatt: BluetoothGatt, status: Int) {
            super.onServicesDiscovered(gatt, status)
            if (status == BluetoothGatt.GATT_SUCCESS) {
                Log.d("BLEManager", "Services discovered")
                val service = gatt.getService(SERVICE_UUID)
                if (service == null) {
                    Log.e("BLEManager", "Servizio BLE non trovato!")
                    return
                }
                rxCharacteristic = service.getCharacteristic(CHARACTERISTIC_UUID_RX)
                if (rxCharacteristic == null) {
                    Log.e("BLEManager", "Caratteristica RX non trovata!")
                    return
                }
                Log.d("BLEManager", "RX characteristic pronta per scrittura")
            } else {
                Log.e("BLEManager", "onServicesDiscovered fallito con status: $status")
            }
        }
    }

    fun writeToCharacteristic(data: ByteArray): Boolean {
        val characteristic = rxCharacteristic ?: run {
            Log.e("BLEManager", "Caratteristica RX non inizializzata")
            return false
        }

        characteristic.value = data
        val writeResult = bluetoothGatt?.writeCharacteristic(characteristic) ?: false

        Log.d("BLEManager", "Scrittura caratteristica: $writeResult - dati: ${data.contentToString()}")
        return writeResult
    }
}
