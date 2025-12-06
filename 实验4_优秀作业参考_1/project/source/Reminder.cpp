// Copyright 2024 lyp

#include "../header/Reminder.h"

void Reminder::setReminder(const QDateTime& time, const QString& method, const QString& msg) {
    reminderTime = time;
    notificationMethod = method;
    message = msg;
}

bool Reminder::checkAndTriggerReminder() {
    if (QDateTime::currentDateTime() >= reminderTime) {
        return true;
    }
    return false;
}

QDateTime Reminder::getReminderTime() {
    return reminderTime;
}
