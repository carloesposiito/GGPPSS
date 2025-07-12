
package com.example.ggppss

import android.service.notification.NotificationListenerService
import android.service.notification.StatusBarNotification
import android.util.Log
import android.os.Bundle

class MyNotificationListenerService : NotificationListenerService() {

    companion object {
        val mapsPackageName = "com.google.android.apps.maps"

        var previousText: String? = ""
        var previousTitleData: String? = ""
        var previousSubtextData: String? = ""
        var previousPostTime: Long = 0
    }

    object NotificationBridge {
        var onNotificationReceived: ((String) -> Unit)? = null
    }

    override fun onNotificationPosted(sbn: StatusBarNotification) {
        if (sbn.packageName == mapsPackageName) {

            val currentNotificationData = sbn.notification.extras

            val currentText = currentNotificationData.getCharSequence("android.text")?.toString() ?: ""
            val currentTitleData = currentNotificationData.getCharSequence("android.title")?.toString() ?: ""
            val currentSubtextData = currentNotificationData.getCharSequence("android.subText")?.toString() ?: ""

            Log.d("MAPS_NOTIFY_RAW", "Text: $currentText | Title: $currentTitleData | SubText: $currentSubtextData")
            Log.d("MAPS_NOTIFY_RAW", "PostTime: ${sbn.postTime} | Previous: $previousPostTime")

            if (
                currentText != previousText ||
                currentTitleData != previousTitleData ||
                currentSubtextData != previousSubtextData ||
                sbn.postTime != previousPostTime
            ) {
                previousText = currentText
                previousTitleData = currentTitleData
                previousSubtextData = currentSubtextData
                previousPostTime = sbn.postTime

                //region Parsing current direction distance
                var currentTextDistance: String? = ""
                var currentTextDistanceUnit: String? = ""
                val distanceRegex = Regex("""([\d.,]+)\s*([a-zA-Z]+)""")
                distanceRegex.find(currentTitleData)?.let {
                    currentTextDistance = it.groupValues[1]
                    currentTextDistanceUnit = it.groupValues[2]
                }
                //endregion

                //region Parsing arrival time
                var arrivalTime: String? = ""
                val arrivalTimeRegex = Regex("""Arrivo\s+(\d{1,2}:\d{2})""")
                arrivalTimeRegex.find(currentSubtextData)?.let {
                    arrivalTime = it.groupValues[1]
                }
                //endregion

                //region Parsing total distance left
                var totalDistanceLeft: String? = ""
                val distanceLeftRegex = Regex("""(\d+)\s*(km|m)""")
                distanceLeftRegex.find(currentSubtextData)?.let {
                    val distanceLeft = it.groupValues[1]
                    val distanceLeftUnit = it.groupValues[2]
                    totalDistanceLeft = "$distanceLeft$distanceLeftUnit"
                }
                //endregion

                //region Parsing time left
                var timeLeft: String? = ""
                val timeLeftRegex = Regex("""(?:(\d+)\s*h)?\s*(?:(\d+)\s*min)?""")
                timeLeftRegex.find(currentSubtextData)?.let {
                    val hours = it.groupValues[1].takeIf { it.isNotBlank() }?.toInt() ?: 0
                    val minutes = it.groupValues[2].takeIf { it.isNotBlank() }?.toInt() ?: 0
                    timeLeft = "${hours}h ${minutes}m"
                }
                //endregion

                // Log final data
                Log.d("MAPS_NOTIFY", "\"$currentText\" in $currentTextDistance $currentTextDistanceUnit")
                Log.d("MAPS_NOTIFY", "Arrival $arrivalTime - $totalDistanceLeft left ($timeLeft)")
                Log.d("MAPS_NOTIFY", "************************************************")
            }
        }
    }

    override fun onNotificationRemoved(sbn: StatusBarNotification) {
        Log.d("NotificationReader", "Notification removed: ${sbn.packageName}")
    }
}


/*
package com.example.ggppss

import android.service.notification.NotificationListenerService
import android.service.notification.StatusBarNotification
import android.util.Log
import android.os.Bundle

class MyNotificationListenerService : NotificationListenerService() {

    companion object {
        const val mapsPackageName = "com.google.android.apps.maps"

        var previousTitleData: String? = ""
        var previousText: String? = ""
        var previousSubtextData: String? = ""
    }

    object NotificationBridge {
        var onNotificationReceived: ((String) -> Unit)? = null
    }

    override fun onNotificationPosted(sbn: StatusBarNotification) {
        if (sbn.packageName == mapsPackageName) {
            val data = sbn.notification.extras

            val currentText = data.getCharSequence("android.text")?.toString() ?: ""
            val currentTitleData = data.getCharSequence("android.title")?.toString() ?: ""
            val currentSubtextData = data.getCharSequence("android.subText")?.toString() ?: ""

            if (currentText != previousText || currentTitleData != previousTitleData || currentSubtextData != previousSubtextData) {
                previousText = currentText
                previousTitleData = currentTitleData
                previousSubtextData = currentSubtextData

                val distanceRegex = Regex("""([\d.,]+)\s*([a-zA-Z]+)""")
                val match = distanceRegex.find(currentTitleData)
                val currentTextDistance = match?.groupValues?.get(1) ?: ""
                val currentTextDistanceUnit = match?.groupValues?.get(2) ?: ""

                val arrivalTimeRegex = Regex("""Arrivo\s+(\d{1,2}:\d{2})""")
                val arrivalTime = arrivalTimeRegex.find(currentSubtextData)?.groupValues?.get(1)

                val distanceLeftRegex = Regex("""(\d+)\s*(km|m)""")
                val totalDistanceLeft = distanceLeftRegex.find(currentSubtextData)?.let {
                    "${it.groupValues[1]} ${it.groupValues[2]}"
                }

                val timeLeftRegex = Regex("""(?:(\d+)\s*h)?\s*(?:(\d+)\s*min)?""")
                val timeLeft = timeLeftRegex.find(currentSubtextData)?.let {
                    val hours = it.groupValues[1].takeIf { it.isNotBlank() }?.toInt() ?: 0
                    val minutes = it.groupValues[2].takeIf { it.isNotBlank() }?.toInt() ?: 0
                    "${hours}h ${minutes}m"
                }

                Log.d("MAPS_NOTIFY", "\"$currentText\" in $currentTextDistance $currentTextDistanceUnit")
                Log.d("MAPS_NOTIFY", "Arrival $arrivalTime - $totalDistanceLeft left ($timeLeft)")
                Log.d("MAPS_NOTIFY", "************************************************")
            }
        }
    }

    override fun onNotificationRemoved(sbn: StatusBarNotification) {
        Log.d("NotificationReader", "Notification removed: ${sbn.packageName}")
    }
}
*/