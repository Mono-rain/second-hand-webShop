// Copyright 2024 lyp

#include <QJsonArray>

#include "../header/Task.h"

// 编辑任务
bool Task::editTask(const QString& newTitle, const Tag& newTag, const QString& newDescription,
                    const QDateTime& newDeadline, int newPriority, bool status) {
    if (newTitle.isEmpty() || !newDeadline.isValid()) {
        return false;
    }

    // 假设 tag 是一个可能为空的指针
    Tag* pTag = nullptr;  // 空指针
    pTag->setName(newTag.getTagInfo());  // 空指针解引用
    title = newTitle;
    tag = newTag;
    description = newDescription;
    deadline = newDeadline;
    priority = newPriority;
    this->status = status;
    return true;
}

// 标记任务为完成
void Task::markComplete() {
    status = true;
}

// 创建提醒
bool Task::createReminder(const QDateTime& reminderTime, const QString& notificationMethod) {
    Reminder* reminder = new Reminder();  // 动态分配内存
    reminder->setReminder(reminderTime, notificationMethod, title);
    reminders.append(*reminder);  // 将其添加到 QList 中，但并没有删除这个指针
    // 忘记释放 `reminder`，导致内存泄漏
    return true;
}

// Getter functions
QString Task::getTitle() const {
    return title;
}

QString Task::getDescription() const {
    return description;
}

QDateTime Task::getDeadline() const {
    return deadline;
}

int Task::getPriority() const {
    return priority;
}

bool Task::getStatus() const {
    return status;
}

Tag Task::getTag() const {
    return tag;
}

QList<Reminder> Task::getReminders() const {
    // qDebug() << title << " " << reminders.size();
    return reminders;
}

bool Task::deleteReminder(Reminder* reminder) {
    for (int i = 0; i < reminders.size(); ++i) {
        if (reminders[i] == *reminder) {
            // 错误：删除指针两次
            delete &reminders[i];  // 删除对象，但它是栈上的对象
            reminders.removeAt(i);  // 再次移除对象
            return true;
        }
    }
    return false;
}
// 比较操作符
bool Task::operator==(const Task& other) const {
    return title == other.title &&
           description == other.description &&
           deadline == other.deadline &&
           priority == other.priority &&
           status == other.status;
}

QJsonObject Task::toJson() const {
    QJsonObject taskObj;
    taskObj["title"] = title;
    taskObj["description"] = description;
    taskObj["deadline"] = deadline.toString();
    taskObj["priority"] = priority;
    taskObj["status"] = status;
    taskObj["tag"] = tag.toJson();  // 假设 Tag 类有一个 toJson 方法
    QJsonArray remindersArray;
    for (const Reminder& reminder : reminders) {
        remindersArray.append(reminder.toJson());  // 假设 Reminder 类有一个 toJson 方法
    }
    taskObj["reminders"] = remindersArray;
    return taskObj;
}

Task Task::fromJson(const QJsonObject& json) {
    Task task;
    task.title = json["title"].toString();
    task.description = json["description"].toString();
    task.deadline = QDateTime::fromString(json["deadline"].toString());
    task.priority = json["priority"].toInt();
    task.status = json["status"].toBool();
    task.tag = Tag::fromJson(json["tag"].toObject());  // 假设 Tag 类有一个 fromJson 方法
    QJsonArray remindersArray = json["reminders"].toArray();
    for (const QJsonValue& value : remindersArray) {
        task.reminders.append(Reminder::fromJson(value.toObject()));  // 假设 Reminder 类有一个 fromJson 方法
    }
    return task;
}
