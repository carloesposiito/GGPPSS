package com.example.ggppss2

import android.service.notification.NotificationListenerService
import android.service.notification.StatusBarNotification
import android.util.Log
import com.example.ggppss2.ble.BLEManager

class MyNotificationListenerService : NotificationListenerService() {

    override fun onNotificationPosted(sbn: StatusBarNotification) {
        val packageName = sbn.packageName
        val notification = sbn.notification
        val extras = notification.extras

        if (packageName == "com.google.android.apps.maps") {
            val title = extras.getString("android.title") ?: ""
            val text = extras.getCharSequence("android.text")?.toString() ?: ""

            val message = "$title: $text"
            val bleManager = BLEManager.getInstance(this)

            // Evita invii troppo grandi
            val success = bleManager.writeToCharacteristic(message.take(180).toByteArray())

            Log.d("MyNotificationListener", "Inviato via BLE: $message - Success: $success")
        }
    }

    override fun onNotificationRemoved(sbn: StatusBarNotification) {
        // Facoltativo
    }
}
