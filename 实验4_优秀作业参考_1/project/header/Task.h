// Copyright 2024 lyp

#ifndef HEADER_TASK_H_
#define HEADER_TASK_H_

#include <QString>
#include <QDateTime>
#include <QList>
#include <QJsonObject>

#include "../header/Tag.h"
#include "../header/Reminder.h"

class Task {
 private:
    QString title;              // 任务标题
    QString description;        // 任务描述
    QDateTime deadline;         // 截止日期
    int priority;               // 优先级
    bool status;                // 任务状态
    Tag tag;                    // 标签列表
    QList<Reminder> reminders;  // 提醒列表

 public:
    // 编辑任务
    bool editTask(const QString& newTitle, const Tag& newTag, const QString& newDescription,
                  const QDateTime& newDeadline, int newPriority, bool status);
    void markComplete();
    bool createReminder(const QDateTime& reminderTime, const QString& notificationMethod);

    // Getter functions
    QString getTitle() const;
    QString getDescription() const;
    QDateTime getDeadline() const;
    int getPriority() const;
    bool getStatus() const;
    Tag getTag() const;
    QList<Reminder> getReminders() const;
    bool deleteReminder(Reminder* reminder);

    // 比较操作符
    bool operator==(const Task& other) const;
    // 序列化 Task 对象
    QJsonObject toJson() const;
    // 反序列化 Task 对象
    static Task fromJson(const QJsonObject& json);
};

#endif  // HEADER_TASK_H_
