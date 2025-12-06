// Copyright 2024 lyp

#include "../header/Setting.h"

// 管理用户设置
void Setting::manageUserSettings() {
    // 管理设置的逻辑
}

void Setting::sendNotification(User* recipient, Task* task, Reminder* reminder) {
    notificationManager.sendNotification(recipient, task, reminder);
}
