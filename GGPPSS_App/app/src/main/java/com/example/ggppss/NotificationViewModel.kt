package com.example.ggppss

import androidx.compose.runtime.mutableStateListOf
import androidx.lifecycle.ViewModel

class NotificationViewModel : ViewModel() {
    val notifications = mutableStateListOf<String>()

    fun addNotification(notification: String) {
        notifications.add(0, notification) // aggiungi in cima
    }
}