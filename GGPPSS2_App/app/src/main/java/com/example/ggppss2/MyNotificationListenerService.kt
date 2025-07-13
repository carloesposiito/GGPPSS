package com.example.ggppss2

import android.service.notification.NotificationListenerService
import android.service.notification.StatusBarNotification
import android.util.Log
import com.example.ggppss2.ble.BLEManager

class MyNotificationListenerService : NotificationListenerService() {

    companion object {
        private const val TAG = "MAPS_NOTIFY"
        private const val mapsPackageName = "com.google.android.apps.maps"

        private var previousText: String? = ""
        private var previousTitle: String? = ""
        private var previousSubtext: String? = ""
        private var previousPostTime: Long = 0
    }

    override fun onNotificationPosted(sbn: StatusBarNotification) {
        if (sbn.packageName != mapsPackageName) return

        val extras = sbn.notification.extras
        val text = extras.getCharSequence("android.text")?.toString() ?: ""
        val title = extras.getCharSequence("android.title")?.toString() ?: ""
        val subtext = extras.getCharSequence("android.subText")?.toString() ?: ""

        Log.d("${TAG}_RAW", "Text: $text | Title: $title | SubText: $subtext")
        Log.d("${TAG}_RAW", "PostTime: ${sbn.postTime} | Previous: $previousPostTime")

        val isNew = text != previousText ||
                title != previousTitle ||
                subtext != previousSubtext ||
                sbn.postTime != previousPostTime

        if (!isNew) return

        previousText = text
        previousTitle = title
        previousSubtext = subtext
        previousPostTime = sbn.postTime

        // Parse distanza (es: 0 m)
        val distanceMatch = Regex("""([\d.,]+)\s*([a-zA-Z]+)""").find(title)
        val distance = distanceMatch?.groupValues?.get(1) ?: ""
        val unit = distanceMatch?.groupValues?.get(2) ?: ""

        // Parse orario di arrivo (es: Arrivo 01:31)
        val arrivalMatch = Regex("""Arrivo\s+(\d{1,2}:\d{2})""").find(subtext)
        val arrival = arrivalMatch?.groupValues?.get(1) ?: ""

        // Parse distanza rimanente (es: 13 km)
        val leftMatch = Regex("""(\d+)\s*(km|m)""").find(subtext)
        val left = leftMatch?.let { "${it.groupValues[1]}${it.groupValues[2]}" } ?: ""

        // Parse tempo rimanente (es: 21 min -> 0h 21m)
        val timeMatch = Regex("""(?:(\d+)\s*h)?\s*(?:(\d+)\s*min)?""").find(subtext)
        val hours = timeMatch?.groupValues?.get(1)?.toIntOrNull() ?: 0
        val minutes = timeMatch?.groupValues?.get(2)?.toIntOrNull() ?: 0
        val time = "${hours}h ${minutes}m"

        Log.d(TAG, "\"$text\" in $distance $unit")
        Log.d(TAG, "Arrival $arrival - $left left ($time)")

        // Costruzione JSON già fatta qui
        val jsonMessage = """
    {
        "text": "$text",
        "distance": "$distance",
        "unit": "$unit",
        "arrival": "$arrival",
        "left": "$left",
        "time": "$time"
    }
""".trimIndent()

        Log.d("BLE_NOTIFY", "Sending BLE JSON:")
        Log.d("BLE_NOTIFY", jsonMessage)

        try {
            val bleManager = BLEManager.getInstance(this)

            // Taglio la stringa JSON a max 180 byte, se serve
            val jsonToSend = if (jsonMessage.length > 180) jsonMessage.take(180) else jsonMessage

            val success = bleManager.writeToCharacteristic(jsonToSend.toByteArray())

            Log.d("MyNotificationListener", "Inviato via BLE: $jsonToSend - Success: $success")
        } catch (e: Exception) {
            Log.e("BLE_NOTIFY", "Failed to send BLE notification", e)
        }
    }

    override fun onNotificationRemoved(sbn: StatusBarNotification) {
        // Facoltativo
    }
}
