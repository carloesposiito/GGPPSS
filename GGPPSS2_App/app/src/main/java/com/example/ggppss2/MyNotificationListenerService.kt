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
        if (sbn.packageName == mapsPackageName)
        {
            val extras = sbn.notification.extras
            val text = extras.getCharSequence("android.text")?.toString() ?: ""
            val title = extras.getCharSequence("android.title")?.toString() ?: ""
            val subtext = extras.getCharSequence("android.subText")?.toString() ?: ""

            val isNew = text != previousText ||
                    title != previousTitle ||
                    subtext != previousSubtext ||
                    sbn.postTime != previousPostTime

            if (isNew)
            {
                previousText = text
                previousTitle = title
                previousSubtext = subtext
                previousPostTime = sbn.postTime

                // Parse distance to next direction
                val distanceMatch = Regex("""([\d.,]+)\s*([a-zA-Z]+)""").find(title)
                val distance = distanceMatch?.groupValues?.get(1) ?: ""
                val unit = distanceMatch?.groupValues?.get(2) ?: ""

                // Parse arrival time
                val arrivalMatch = Regex("""Arrivo\s+(\d{1,2}:\d{2})""").find(subtext)
                val arrival = arrivalMatch?.groupValues?.get(1) ?: ""

                // Parse total left distance
                val leftDistanceMatch = Regex("""(?<!\bmin\s)(?<!\bh\s)(\d+(?:[.,]\d+)?)\s*(km|m)\b""").find(subtext)
                val left = leftDistanceMatch?.let { "${it.groupValues[1]}${it.groupValues[2]}" } ?: ""

                // Parse left timer
                val timeMatch = Regex("""(?:(\d+)\s*h)?\s*(?:(\d+)\s*min)?""").find(subtext)
                val hours = timeMatch?.groupValues?.get(1)?.toIntOrNull() ?: 0
                val minutes = timeMatch?.groupValues?.get(2)?.toIntOrNull() ?: 0
                val time = "${hours}h ${minutes}'"

                // Print read data
                Log.d("GGPPSS", "\"$text\" in $distance $unit")
                Log.d("GGPPSS", "Arrival $arrival - $left left ($time)")

                // Send data via BLE
                try {
                    val jsonMessage = """
                    {
                        "nD": "$text",
                        "nDD": "$distance",
                        "nDDU": "$unit",
                        "a": "$arrival",
                        "dL": "$left",
                        "tL": "$time"
                    }
                    """.trimIndent()

                    val bleManager = BLEManager.getInstance(this)
                    val jsonToSend = if (jsonMessage.length > 180) jsonMessage.take(180) else jsonMessage
                    val success = bleManager.writeToCharacteristic(jsonToSend.toByteArray())

                    Log.d("GGPPSS", "JSON send successfully")
                }
                catch (e: Exception)
                {
                    Log.e("GGPPSS", "Failed to send BLE notification", e)
                }
            }
        }
    }

    override fun onNotificationRemoved(sbn: StatusBarNotification) { }
}
