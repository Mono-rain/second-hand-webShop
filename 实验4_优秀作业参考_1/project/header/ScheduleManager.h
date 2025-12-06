// Copyright 2024 lyp

#ifndef HEADER_SCHEDULEMANAGER_H_
#define HEADER_SCHEDULEMANAGER_H_

#include <QList>
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>

#include "../header/Tag.h"
#include "../header/Task.h"

class ScheduleManager {
 private:
    QList<Tag> tags;  // 标签列表

 public:
    ScheduleManager();  // 构造函数
    ~ScheduleManager();  // 析构函数

    bool createTag(const QString& name);
    bool deleteTag(const QString& name);
    QList<Tag> getTags() const;
    QList<Task> filterTasksByTag(const Tag& tag, const QList<Task>& tasks) const;
    QList<Task> displayTasks(const QString& order, const QList<Task>& tasks) const;

    bool saveTags(const QString& filename);  // 保存标签列表到文件
    bool loadTags(const QString& filename);  // 从文件加载标签列表
};

#endif  // HEADER_SCHEDULEMANAGER_H_
