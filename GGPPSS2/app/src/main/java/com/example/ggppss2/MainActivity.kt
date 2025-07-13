package com.example.ggppss2

import android.content.Intent
import android.os.Bundle
import android.provider.Settings
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.*
import androidx.compose.material3.Button
import androidx.compose.material3.Text
import androidx.compose.runtime.*
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.core.app.NotificationManagerCompat
import com.example.ggppss2.ble.BLEManager

class MainActivity : ComponentActivity() {

    private lateinit var bleManager: BLEManager

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        bleManager = BLEManager.getInstance(applicationContext)

        setContent {
            val isConnected by bleManager.isConnected
            var isNotificationAccessGranted by remember { mutableStateOf(false) }

            // Controllo accesso notifiche ogni volta che il composable viene ricostruito
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

            // Avvio scansione una sola volta all'avvio
            LaunchedEffect(Unit) {
                bleManager.startScan("GGPPSS_BLE")
            }
        }
    }

    override fun onResume() {
        super.onResume()
        // Aggiorna lo stato accesso notifiche in onResume (utile se l'utente torna dalla schermata impostazioni)
        val accessGranted = NotificationManagerCompat
            .getEnabledListenerPackages(this)
            .contains(packageName)
        // Per aggiornare lo stato in Compose in modo sicuro, puoi usare un side effect
        // oppure gestire questo stato dentro Compose come fatto sopra (LaunchedEffect)
    }

    override fun onDestroy() {
        super.onDestroy()
        // Ferma scansione e disconnetti per evitare leak e consumi inutili
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
