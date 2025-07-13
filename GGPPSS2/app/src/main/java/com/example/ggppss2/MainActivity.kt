package com.example.ggppss2

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.*
import androidx.compose.material3.Button
import androidx.compose.material3.Text
import androidx.compose.runtime.*
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import com.example.ggppss2.ble.BLEManager

class MainActivity : ComponentActivity() {
    private lateinit var bleManager: BLEManager

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        bleManager = BLEManager(this)

        setContent {
            // Qui usiamo "by" per delegare lo State<Boolean> e ottenere il Boolean
            val isConnected by bleManager.isConnected

            MyUI(isConnected = isConnected, onSendTest = {
                val testString = "Ciao ESP!"
                val testData = testString.toByteArray()
                bleManager.writeToCharacteristic(testData)
            })

            LaunchedEffect(Unit) {
                bleManager.startScan("GGPPSS_BLE")
            }
        }
    }
}

@Composable
fun MyUI(isConnected: Boolean, onSendTest: () -> Unit) {
    Column(
        modifier = Modifier.padding(16.dp)
    ) {
        Text(text = if (isConnected) "Connesso al dispositivo" else "Non connesso")

        Spacer(modifier = Modifier.height(16.dp))

        Button(
            onClick = onSendTest,
            enabled = isConnected
        ) {
            Text("Invia stringa di prova")
        }
    }
}

@Preview(showBackground = true)
@Composable
fun PreviewMyUI() {
    MyUI(isConnected = false, onSendTest = {})
}
