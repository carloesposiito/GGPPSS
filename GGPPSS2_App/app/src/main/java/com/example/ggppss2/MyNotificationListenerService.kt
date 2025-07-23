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
        if (sbn.packageName == MAPS_PACKAGE_NAME) {
            val extras = sbn.notification.extras

            val title = extras.getCharSequence("android.title")?.toString() ?: ""
            val text = extras.getCharSequence("android.text")?.toString() ?: ""
            val subtext = extras.getCharSequence("android.subText")?.toString() ?: ""

            val isNew = text != previousText ||
                    title != previousTitle ||
                    subtext != previousSubtext ||
                    sbn.postTime != previousPostTime

            if (isNew) {
                previousText = text
                previousTitle = title
                previousSubtext = subtext
                previousPostTime = sbn.postTime

                val nextDirectionDistance = title.replace("\\s+".toRegex(), "")
                val nextDirection = text

                //region Extract remaining data from subtext
                val separator = "·"
                val parts = subtext.split(separator).map { it.trim() }

                var timeLeft = ""
                var distanceLeft = ""
                var arrivalTime = ""
                var otherData = ""

                if (parts.size >= 3) {
                    timeLeft = parts[0]
                        .replace("\\s*min".toRegex(), "'")
                        .replace("\\s*hr".toRegex(), "h")
                        .replace("\\s*h".toRegex(), "h")

                    distanceLeft = parts[1].replace(" ", "")

                    val timeRegex = Regex("""\d{1,2}:\d{2}""")
                    arrivalTime = timeRegex.find(parts[2])?.value ?: ""

                    val left = "$distanceLeft ($timeLeft)"
                    otherData = "$arrivalTime, $left"
                } else {
                    Log.d(APP_TAG, "Parts = ${parts.size}")
                    otherData = subtext
                }
                //endregion

                Log.d(APP_TAG, "\"$nextDirection\" in $nextDirectionDistance")
                Log.d(APP_TAG, otherData)

                try {
                    val jsonMessage = """
                {
                    "nD": "$nextDirection",
                    "nDD": "$nextDirectionDistance",
                    "aT": "$arrivalTime",
                    "dL": "$distanceLeft",
                    "tL": "$timeLeft"
                }
                """.trimIndent()

                    val bleManager = BLEManager.getInstance(this)
                    val jsonToSend = if (jsonMessage.length > 180) jsonMessage.take(180) else jsonMessage
                    bleManager.writeToCharacteristic(jsonToSend.toByteArray())

                    Log.d(APP_TAG, "Sending JSON... OK!")
                } catch (e: Exception) {
                    Log.e(APP_TAG, "Sending JSON... ERROR!", e)
                }
            }
        }
    }

    override fun onNotificationRemoved(sbn: StatusBarNotification) { }
}
