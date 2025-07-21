package com.example.ggppss2

import android.service.notification.NotificationListenerService
import android.service.notification.StatusBarNotification
import android.util.Log
import com.example.ggppss2.ble.BLEManager

class MyNotificationListenerService : NotificationListenerService() {

    companion object {
        private const val MAPS_PACKAGE_NAME = "com.google.android.apps.maps"
        private const val APP_TAG = "GGPPSS"
        private var previousText: String? = ""
        private var previousTitle: String? = ""
        private var previousSubtext: String? = ""
        private var previousPostTime: Long = 0
    }

    override fun onNotificationPosted(sbn: StatusBarNotification) {
        if (sbn.packageName == MAPS_PACKAGE_NAME)
        {
            val extras = sbn.notification.extras

            // Title holds distance from next direction
            val title = extras.getCharSequence("android.title")?.toString() ?: ""

            // Text holds next direction
            val text = extras.getCharSequence("android.text")?.toString() ?: ""

            // Subtext holds:
            // 1 - Time remaining to destination
            // 2 - Distance remaining to destination
            // 3 - Arrival time
            val subtext = extras.getCharSequence("android.subText")?.toString() ?: ""

            // Check if current notification data is different from previous one
            val isNew = text != previousText ||
                    title != previousTitle ||
                    subtext != previousSubtext ||
                    sbn.postTime != previousPostTime

            // If so...
            if (isNew)
            {
                previousText = text
                previousTitle = title
                previousSubtext = subtext
                previousPostTime = sbn.postTime

                //region Extract distance to next direction from title

                val nextDirectionDistance = title.replace("\\s+".toRegex(), "")

                //endregion

                //region Extract next direction from text

                val nextDirection = text

                //endregion

                //region Extract remaining data from subtext

                // Split spring according to separator
                val separator = "·"
                val parts = subtext.split(separator).map { it.trim() }

                // 1 - Extract left time
                val timeLeft = parts.elementAt(0)
                    .replace("\\s*min".toRegex(), "'")
                    .replace("\\s*hr".toRegex(), "h")
                    .replace("\\s*h".toRegex(), "h")

                // 2 - Extract left distance
                val distanceLeft = parts.elementAt(1).replace(" ", "")

                // 3 - Extract arrival time from last string part
                val timeRegex = Regex("""\d{1,2}:\d{2}""")
                val arrivalTime = timeRegex.find(parts.last())?.value ?: ""

                // Build result string
                val left = "$distanceLeft ($timeLeft)"
                val otherData = "$arrivalTime, $left"

                //endregion

                // Print read data
                Log.d(APP_TAG, "\"$nextDirection\" in $nextDirectionDistance")
                Log.d(APP_TAG, otherData)

                // Send data via BLE
                try {

                    val jsonMessage = """
                    {
                        "nD": "$nextDirection",
                        "nDD": "$nextDirectionDistance",
                        "aT": "$arrivalTime",
                        "l": "$left"
                    }
                    """.trimIndent()

                    val bleManager = BLEManager.getInstance(this)
                    val jsonToSend = if (jsonMessage.length > 180) jsonMessage.take(180) else jsonMessage

                    // Send data via BLE
                    bleManager.writeToCharacteristic(jsonToSend.toByteArray())

                    Log.d(APP_TAG, "Sending JSON... OK!")
                }
                catch (e: Exception)
                {
                    Log.e(APP_TAG, "Sending JSON... ERROR!", e)
                }
            }
        }
    }

    override fun onNotificationRemoved(sbn: StatusBarNotification) { }
}
