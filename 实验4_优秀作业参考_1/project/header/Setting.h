// Copyright 2024 lyp

#ifndef HEADER_SETTING_H_
#define HEADER_SETTING_H_

#include <QString>

#include "../header/NotificationManager.h"

class Setting {
 private:
    QString username;               // 用户名
    NotificationManager notificationManager;  // 通知管理器

 public:
    void manageUserSettings();
    void sendNotification(User* recipient, Task* task, Reminder* reminder);
};

#endif  // HEADER_SETTING_H_
