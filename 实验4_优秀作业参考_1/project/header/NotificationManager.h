// Copyright 2024 lyp

#ifndef HEADER_NOTIFICATIONMANAGER_H_
#define HEADER_NOTIFICATIONMANAGER_H_

#include <QString>
#include <QMap>
#include <QMessageBox>
#include <QWidget>  // 确保你能访问 QWidget 类型

#include "../header/Task.h"
#include "../header/Reminder.h"

class User;  // 前向声明

class NotificationManager : public QObject {
    Q_OBJECT

 private:
    QMap<QString, QString> notificationSettings;  // 通知设置

 public:
    void sendNotification(User* recipient, Task* task, Reminder* reminder);
    void customizeNotification(const QMap<QString, QString>& settings);

 private slots:
    void onMessageBoxClosed(int result, Task* task, Reminder* reminder);
};

#endif  // HEADER_NOTIFICATIONMANAGER_H_
