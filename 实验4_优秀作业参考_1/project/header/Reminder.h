// Copyright 2024 lyp

#ifndef HEADER_REMINDER_H_
#define HEADER_REMINDER_H_

#include <QString>
#include <QDateTime>
#include <QJsonObject>

class Reminder {
 private:
    QDateTime reminderTime;      // 提醒时间
    QString notificationMethod;  // 通知方式
    QString message;             // 提醒消息

 public:
    void setReminder(const QDateTime& time, const QString& method, const QString& msg);
    bool checkAndTriggerReminder();  // 定时检查是否触发提醒
    QDateTime getReminderTime();
    bool operator==(const Reminder& other) const {
        return reminderTime == other.reminderTime &&
               notificationMethod == other.notificationMethod &&
               message == other.message;
    }
    // 转换为 QJsonObject (序列化)
    QJsonObject toJson() const {
        QJsonObject json;
        json["reminderTime"] = reminderTime.toString();  // QDateTime 转换为字符串
        json["notificationMethod"] = notificationMethod;
        json["message"] = message;
        return json;
    }
    // 从 QJsonObject 创建 Reminder 对象 (反序列化)
    static Reminder fromJson(const QJsonObject& json) {
        Reminder reminder;
        reminder.reminderTime = QDateTime::fromString(json["reminderTime"].toString());
        reminder.notificationMethod = json["notificationMethod"].toString();
        reminder.message = json["message"].toString();
        return reminder;
    }
};

#endif  // HEADER_REMINDER_H_
