package com.example.ggppss2

import android.Manifest
import android.content.Context
import android.content.Intent
import android.location.LocationManager
import android.os.Build
import android.os.Bundle
import android.provider.Settings
import android.widget.Toast
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.result.contract.ActivityResultContracts
import androidx.compose.foundation.layout.*
import androidx.compose.material3.Button
import androidx.compose.material3.Text
import androidx.compose.runtime.*
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.core.app.NotificationManagerCompat
import androidx.core.content.ContextCompat
import com.example.ggppss2.ble.BLEManager

class MainActivity : ComponentActivity() {

    private lateinit var bleManager: BLEManager

    // Launcher per richiesta permesso location (Android 11 e precedenti)
    private val requestLocationPermissionLauncher =
        registerForActivityResult(ActivityResultContracts.RequestPermission()) { isGranted ->
            if (isGranted) {
                checkLocationEnabledAndStartScan()
            } else {
                Toast.makeText(
                    this,
                    "Permesso Location richiesto per usare Bluetooth BLE su Android 11 e versioni precedenti",
                    Toast.LENGTH_LONG
                ).show()
            }
        }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        bleManager = BLEManager.getInstance(applicationContext)

        setContent {
            val isConnected by bleManager.isConnected
            var isNotificationAccessGranted by remember { mutableStateOf(false) }

            LaunchedEffect(Unit) {
                isNotificationAccessGranted = NotificationManagerCompat
                    .getEnabledListenerPackages(applicationContext)
                    .contains(packageName)
            }

            MyUI(
                isConnected = isConnected,
                isNotificationAccessGranted = isNotificationAccessGranted,
                onSendTest = {
                    val testString = "Ciao ESP!"
                    val testData = testString.toByteArray()
                    bleManager.writeToCharacteristic(testData)
                },
                onOpenNotificationSettings = {
                    val intent = Intent(Settings.ACTION_NOTIFICATION_LISTENER_SETTINGS)
                    startActivity(intent)
                }
            )

            // Avvio scansione dopo permessi e location
            LaunchedEffect(Unit) {
                checkAndRequestPermissions()
            }
        }
    }

    private fun checkAndRequestPermissions() {
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.S) { // Android 11 e precedenti
            val hasLocationPermission = ContextCompat.checkSelfPermission(
                this,
                Manifest.permission.ACCESS_FINE_LOCATION
            ) == android.content.pm.PackageManager.PERMISSION_GRANTED

            if (hasLocationPermission) {
                checkLocationEnabledAndStartScan()
            } else {
                // Richiedi permesso location
                requestLocationPermissionLauncher.launch(Manifest.permission.ACCESS_FINE_LOCATION)
            }
        } else {
            // Android 12+ (S) e superiori - gestisci permessi diversi se serve
            startBleScanDirect()
        }
    }

    private fun checkLocationEnabledAndStartScan() {
        val locationManager = getSystemService(Context.LOCATION_SERVICE) as LocationManager
        val isLocationEnabled = locationManager.isProviderEnabled(LocationManager.GPS_PROVIDER) ||
                locationManager.isProviderEnabled(LocationManager.NETWORK_PROVIDER)

        if (isLocationEnabled) {
            startBleScanDirect()
        } else {
            Toast.makeText(
                this,
                "Attiva la posizione per usare il Bluetooth BLE su Android 11 e versioni precedenti",
                Toast.LENGTH_LONG
            ).show()
            // Puoi anche aprire direttamente le impostazioni Location:
            val intent = Intent(Settings.ACTION_LOCATION_SOURCE_SETTINGS)
            startActivity(intent)
        }
    }

    private fun startBleScanDirect() {
        bleManager.startScan("GGPPSS_BLE")
    }

    override fun onResume() {
        super.onResume()
        // Rieffettua controllo accesso notifiche
        val accessGranted = NotificationManagerCompat
            .getEnabledListenerPackages(this)
            .contains(packageName)
        // Se vuoi aggiornare UI, gestisci dentro Compose o tramite MutableState
    }

    override fun onDestroy() {
        super.onDestroy()
        bleManager.stopScan()
        bleManager.disconnect()
    }
}

@Composable
fun MyUI(
    isConnected: Boolean,
    isNotificationAccessGranted: Boolean,
    onSendTest: () -> Unit,
    onOpenNotificationSettings: () -> Unit
) {
    Column(
        modifier = Modifier
            .fillMaxSize()
            .padding(16.dp)
    ) {
        Text(text = if (isConnected) "Connesso al dispositivo" else "Non connesso")

        Spacer(modifier = Modifier.height(16.dp))

        Button(
            onClick = onSendTest,
            enabled = isConnected
        ) {
            Text("Invia stringa di prova")
        }

        Spacer(modifier = Modifier.height(32.dp))

        Text(
            text = if (isNotificationAccessGranted)
                "Accesso notifiche abilitato"
            else
                "Accesso notifiche NON abilitato"
        )

        Spacer(modifier = Modifier.height(8.dp))

        Button(onClick = onOpenNotificationSettings) {
            Text("Abilita accesso notifiche")
        }
    }
}

@Preview(showBackground = true)
@Composable
fun PreviewMyUI() {
    MyUI(
        isConnected = false,
        isNotificationAccessGranted = false,
        onSendTest = {},
        onOpenNotificationSettings = {}
    )
}
