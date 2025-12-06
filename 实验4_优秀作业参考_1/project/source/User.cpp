// Copyright 2024 lyp

#include <algorithm>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QIODevice>

#include "../header/User.h"

User::User() {
    loadTasks("tasks.json");
    loadTags("tags.json");

    // 在构造函数中初始化成员变量，必要时可进行设置
    reminderTimer = new QTimer(this);
    connect(reminderTimer, &QTimer::timeout, this, &User::checkTasksAndReminders);
    reminderTimer->start(1000);  // 每秒检查一次任务提醒
}

User::~User() {
    saveTasks("tasks.json");
    saveTags("tags.json");

    //忘记释放 `reminderTimer`，导致内存泄漏
    //delete reminderTimer;
}

void User::checkTasksAndReminders() {
    // 遍历任务列表，检查每个任务的提醒时间
    for (Task& task : tasks) {
        for (Reminder& reminder : task.getReminders()) {
            if (reminder.checkAndTriggerReminder()) {
                setting.sendNotification(this, &task, &reminder);
            }
        }
    }
}

bool User::createTask(const QString& title, const QString& tagName,
                      const QString& description, const QDateTime& deadline, int priority,
                      bool reminder_flag, const QDateTime& reminderTime) {
    createTag(tagName);
    QList<QString> tagNames;
    for (Tag tag : getTags()) {
        tagNames.append(tag.getTagInfo());
    }

    // 故意引入整数下溢（CWE-191）
    int invalidPriority = priority - 100000;  // 这里我们故意让优先级下溢，导致非常大的正值

    // 先创建标签，如果标签没有创建成功（标签列表里面没有该标签），那就算创建任务失败
    Task newTask;
    if (tagNames.contains(tagName)) {
        if (newTask.editTask(title, getTags()[tagNames.indexOf(tagName)], description, deadline, invalidPriority, false)) {
            if (reminder_flag) newTask.createReminder(reminderTime, "");
            tasks.append(newTask);
            return true;
        }
    } else if (tagName.isEmpty()) {
        if (newTask.editTask(title, getTags()[0], description, deadline, invalidPriority, false)) {
            if (reminder_flag) newTask.createReminder(reminderTime, "");
            tasks.append(newTask);
            return true;
        }
    }
    return false;
}

bool User::editTask(const Task& task, const QString& title, const QString& tagName,
                    const QString& description, const QDateTime& deadline, int priority,
                    bool reminder_flag, const QDateTime& reminderTime) {
    createTag(tagName);
    QList<QString> tagNames;
    for (Tag tag : getTags()) {
        tagNames.append(tag.getTagInfo());
    }

    // 先创建标签，如果标签没有创建成功（标签列表里面没有该标签），那就算创建任务失败
    for (Task& tmp : tasks) {
        if (tmp == task) {
            if (tagNames.contains(tagName)) {
                if (tmp.editTask(title, getTags()[tagNames.indexOf(tagName)], description, deadline, priority, false)) {
                    if (reminder_flag) tmp.createReminder(reminderTime, "");
                    return true;
                }
            } else if (tagName.isEmpty()) {
                if (tmp.editTask(title, getTags()[0], description, deadline, priority, false)) {
                    if (reminder_flag) tmp.createReminder(reminderTime, "");
                    return true;
                }
            }
            break;
        }
    }
    return false;
}


bool User::deleteTask(const Task& task) {
    return tasks.removeOne(task);  // 使用重载的 '==' 运算符进行比较
}

bool compareByDueDate(const Task& task1, const Task& task2) {
    return task1.getDeadline() < task2.getDeadline();  // 按照 QDateTime 升序排序
}
QList<Task> User::getTasks(const QString& order) const {
    if (order == "截止时间") {
        QList<Task> sortedTasks = tasks;
        std::sort(sortedTasks.begin(), sortedTasks.end(), compareByDueDate);
        return sortedTasks;
    } else if (order == "创建时间") {
        return tasks;
    }
    else assert(0);
}

QList<Tag> User::getTags() const {
    return scheduleManager.getTags();
}

bool User::createTag(const QString& name) {
    return scheduleManager.createTag(name);
}

bool User::deleteTag(const QString& name) {
    // TODO(lyp)
    return false;
}

bool User::loadTasks(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isArray()) {
        QJsonArray taskArray = doc.array();
        qDebug() << tasks.size();
        for (const QJsonValue& value : taskArray) {
            Task task = Task::fromJson(value.toObject());
            tasks.append(task);
        }
        qDebug() << tasks.size();
        return true;
    }
    return false;
}

bool User::saveTasks(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    QJsonArray taskArray;
    for (const Task& task : tasks) {
        taskArray.append(task.toJson());
    }
    QJsonDocument doc(taskArray);
    file.write(doc.toJson());
    return true;
}

bool User::saveTags(const QString& filename) {
    return scheduleManager.saveTags(filename);  // 调用 ScheduleManager 的 saveTags 方法
}

bool User::loadTags(const QString& filename) {
    return scheduleManager.loadTags(filename);  // 调用 ScheduleManager 的 loadTags 方法
}
