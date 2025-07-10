package com.example.ggppss

import android.service.notification.NotificationListenerService
import android.service.notification.StatusBarNotification
import android.util.Log

class MyNotificationListenerService : NotificationListenerService() {

    companion object
    {
        // All notification data
        var currentNotificationData: Bundle? = null
        var previousNotificationData: Bundle? = null

        // Distance related
        val distanceRegex = Regex("""([\d.,]+)\s*([a-zA-Z]+)""")
        var currentDistanceValue: String? = ""
        var previousDistanceValue: String? = ""
        var currentDistanceUnit: String? = ""
        var previousDistanceUnit: String? = ""

        // Next direction related
        var currentDirection: String? = ""
        var previousDirection: String? = ""

        // Other details related
        val timeLeftRegex = Regex("""(?:(\d+)\s*h)?\s*(?:(\d+)\s*min)?""")
        val distanceLeftRegex = Regex("""(\d+)\s*(km|m)""")
        val arrivalTimeRegex = Regex("""Arrivo\s+(\d{1,2}:\d{2})""")
        var currentOtherDetails: String? = ""
        var previousOtherDetails: String? = ""
        var currentTimeLeft: String? = ""
        var previousTimeLeft: String? = ""
        var currentDistanceLeft: String? = ""
        var previousDistanceLeft: String? = ""
        var currentDistanceLeftUnit: String? = ""
        var previousDistanceLeftUnit: String? = ""
        var currentArrivalTime: String? = ""
        var previousArrivalTime: String? = ""
    }

    object NotificationBridge {
        var onNotificationReceived: ((String) -> Unit)? = null
    }

    override fun onNotificationPosted(sbn: StatusBarNotification) {
        val packageName = sbn.packageName
        if (packageName == "com.google.android.apps.maps")
        {
            // Full notification content
            currentNotificationData = sbn.notification.extras
            //if (currentNotificationData != previousNotificationData)
            //{
                // Read useful data

                // 1 - Distance
                // Use a REGEX to extract details
                val distance = currentNotificationData.getCharSequence("android.title")?.toString() ?: ""
                val match = distanceRegex.find(distance)
                if (match != null) {

                    // This contains distance value
                    currentDistanceValue = match.groupValues[1]
                    currentDistanceUnit = match.groupValues[2]

                    Log.d("MAPS_NOTIFY", "Distance:\n")
                    Log.d("MAPS_NOTIFY", "$value [$unit]\n")
                    Log.d("MAPS_NOTIFY", "\n")
                }

                // 2 - Direction of the next move
                currentDirection = currentNotificationData.getCharSequence("android.text")?.toString() ?: ""
                Log.d("MAPS_NOTIFY", "Next direction:\n")
                Log.d("MAPS_NOTIFY", "${currentDirection.uppercase()}\n")
                Log.d("MAPS_NOTIFY", "\n")

                // 3 - Other details
                currentOtherDetails = currentNotificationData.getCharSequence("android.subText")?.toString() ?: ""

                // Time left parsing
                timeLeftRegex.find(currentOtherDetails)?.let {
                    var hours = it.groupValues[1].takeIf { it.isNotBlank() }?.toInt() ?: 0
                    var minutes = it.groupValues[2].takeIf { it.isNotBlank() }?.toInt() ?: 0
                    currentTimeLeft = "${hours} [h] ${minutes} [min]"

                    Log.d("MAPS_NOTIFY", "Time left:\n")
                    Log.d("MAPS_NOTIFY", "${currentTimeLeft}\n")
                    Log.d("MAPS_NOTIFY", "\n")
                }

                // Distance left parsing
                distanceLeftRegex.find(currentOtherDetails)?.let {
                    currentDistanceLeft = it.groupValues[1]
                    currentDistanceLeftUnit = it.groupValues[2]

                    Log.d("MAPS_NOTIFY", "Total distance left:\n")
                    Log.d("MAPS_NOTIFY", "${currentDistanceLeft} [${currentDistanceLeftUnit}]\n")
                    Log.d("MAPS_NOTIFY", "\n")
                }

                // Arrival time parsing
                arrivalTimeRegex.find(currentOtherDetails)?.let {
                    currentArrivalTime = it.groupValues[1]

                    Log.d("MAPS_NOTIFY", "Arrival time:\n")
                    Log.d("MAPS_NOTIFY", "${currentArrivalTime}")
                    Log.d("MAPS_NOTIFY", "\n")
                }

                // Compare








                previousNotificationData = currentNotificationData
            }

/*

            val distance = extras.getCharSequence("android.title")?.toString()   // ← Usa questo!
            val direction = extras.getCharSequence("android.text")?.toString()
            val subText = extras.getCharSequence("android.subText")?.toString()

            val messageBuilder = StringBuilder()

            distance?.let { messageBuilder.append("📏 Distanza: $it\n") }
            direction?.let { messageBuilder.append("🧭 Direzione: $it\n") }
            subText?.let { messageBuilder.append("⏱️ Info: $it\n") }

            val finalMessage = messageBuilder.toString().trim()

            Log.d("MAPS_NOTIFY", finalMessage)
 */
            NotificationBridge.onNotificationReceived?.invoke(finalMessage)
        }
    }

    override fun onNotificationRemoved(sbn: StatusBarNotification) {
        Log.d("NotificationReader", "Notification removed: ${sbn.packageName}")
    }
}
