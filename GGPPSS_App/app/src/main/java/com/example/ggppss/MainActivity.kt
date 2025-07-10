package com.example.ggppss

import android.content.Intent
import android.os.Bundle
import android.provider.Settings
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.*
import androidx.compose.material3.*
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import com.example.ggppss.ui.theme.GGPPSSTheme
import androidx.activity.viewModels
import androidx.compose.material3.Button
import androidx.compose.material3.Text
import androidx.compose.material3.MaterialTheme
import androidx.compose.ui.Alignment

class MainActivity : ComponentActivity() {

    val viewModel: NotificationViewModel by viewModels()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()

        NotificationBridge.onNotificationReceived = { message ->
            runOnUiThread {
                viewModel.addNotification(message)
            }
        }

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
                            val intent = Intent("android.settings.ACTION_NOTIFICATION_LISTENER_SETTINGS")
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
                // Apre le impostazioni per concedere il permesso
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

@Composable
fun NotificationList(viewModel: NotificationViewModel) {
    Column {
        for (notification in viewModel.notifications) {
            Text(text = notification, modifier = Modifier.padding(4.dp))
        }
    }
}