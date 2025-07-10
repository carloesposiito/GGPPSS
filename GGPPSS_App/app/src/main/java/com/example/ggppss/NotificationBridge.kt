package com.example.ggppss

object NotificationBridge {
    var onNotificationReceived: ((String) -> Unit)? = null
}
