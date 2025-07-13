package com.example.ggppss

import android.Manifest
import android.bluetooth.BluetoothAdapter
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Build
import android.os.Bundle
import android.provider.Settings
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.result.contract.ActivityResultContracts
import androidx.activity.result.contract.ActivityResultContracts.RequestMultiplePermissions
import androidx.activity.viewModels
import androidx.compose.foundation.layout.*
import androidx.compose.material3.*
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.core.content.ContextCompat
import com.example.ggppss.ui.theme.GGPPSSTheme

class MainActivity : ComponentActivity() {

    private val viewModel: NotificationViewModel by viewModels()

    private val bluetoothAdapter: BluetoothAdapter? = BluetoothAdapter.getDefaultAdapter()

    private lateinit var bleClient: BleClient

    private val requestPermissionsLauncher = registerForActivityResult(
        RequestMultiplePermissions()
    ) { permissions ->
        val granted = permissions.entries.all { it.value }
        if (granted) {
            startBleClientIfBluetoothEnabled()
        } else {
            // Qui potresti mostrare un messaggio o una UI che spiega perché servono i permessi
        }
    }

    private val enableBluetoothLauncher = registerForActivityResult(
        ActivityResultContracts.StartActivityForResult()
    ) {
        if (bluetoothAdapter?.isEnabled == true) {
            startBleClientIfBluetoothEnabled()
        } else {
            // Bluetooth ancora disabilitato, gestisci se vuoi
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        bleClient = BleClient(applicationContext)

        bleClient.onDataReceived = { message ->
            runOnUiThread {
                viewModel.addNotification(message)
            }
        }

        bleClient.onConnected = {
            // Puoi aggiornare UI o notificare l’utente
        }

        bleClient.onDisconnected = {
            // Gestisci la disconnessione (es. riconnessione automatica)
        }

        checkPermissionsAndStart()

        setContent {
            GGPPSSTheme {
                Scaffold(modifier = Modifier.fillMaxSize()) { innerPadding ->
                    Column(
                        modifier = Modifier
                            .padding(innerPadding)
                            .padding(16.dp)
                            .fillMaxSize(),
                        verticalArrangement = Arrangement.Top,
                        horizontalAlignment = Alignment.CenterHorizontally
                    ) {
                        Button(onClick = {
                            val intent = Intent(Settings.ACTION_NOTIFICATION_LISTENER_SETTINGS)
                            startActivity(intent)
                        }) {
                            Text("Apri impostazioni notifiche")
                        }

                        Spacer(modifier = Modifier.height(16.dp))

                        Text("Notifiche da Google Maps:", style = MaterialTheme.typography.titleMedium)

                        NotificationList(viewModel = viewModel)
                    }
                }
            }
        }
    }

    private fun checkPermissionsAndStart() {
        val permissionsNeeded = mutableListOf<String>()

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            permissionsNeeded.addAll(
                listOf(
                    Manifest.permission.BLUETOOTH_SCAN,
                    Manifest.permission.BLUETOOTH_CONNECT,
                    Manifest.permission.BLUETOOTH_ADVERTISE
                )
            )
        } else {
            permissionsNeeded.add(Manifest.permission.ACCESS_FINE_LOCATION)
        }

        val permissionsToRequest = permissionsNeeded.filter {
            ContextCompat.checkSelfPermission(this, it) != PackageManager.PERMISSION_GRANTED
        }

        if (permissionsToRequest.isNotEmpty()) {
            requestPermissionsLauncher.launch(permissionsToRequest.toTypedArray())
            return
        }

        startBleClientIfBluetoothEnabled()
    }

    private fun startBleClientIfBluetoothEnabled() {
        if (bluetoothAdapter == null) {
            // Bluetooth non supportato dal dispositivo
            return
        }
        if (!bluetoothAdapter.isEnabled) {
            val enableBtIntent = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
            enableBluetoothLauncher.launch(enableBtIntent)
            return
        }
        bleClient.startScan()
    }

    override fun onDestroy() {
        super.onDestroy()
        bleClient.disconnect()
    }
}

@Composable
fun NotificationList(viewModel: NotificationViewModel) {
    Column {
        for (notification in viewModel.notifications) {
            Text(text = notification, modifier = Modifier.padding(4.dp))
        }
    }
}

@Composable
fun NotificationPermissionScreen() {
    val context = LocalContext.current

    Column(
        modifier = Modifier
            .fillMaxSize()
            .padding(24.dp),
        verticalArrangement = Arrangement.Center
    ) {
        Text(
            text = "Concedi l'accesso alle notifiche per leggere Google Maps.",
            style = MaterialTheme.typography.bodyLarge
        )
        Spacer(modifier = Modifier.height(16.dp))
        Button(
            onClick = {
                val intent = Intent(Settings.ACTION_NOTIFICATION_LISTENER_SETTINGS)
                context.startActivity(intent)
            }
        ) {
            Text("Apri impostazioni notifiche")
        }
    }
}

@Preview(showBackground = true)
@Composable
fun NotificationPermissionScreenPreview() {
    GGPPSSTheme {
        NotificationPermissionScreen()
    }
}
