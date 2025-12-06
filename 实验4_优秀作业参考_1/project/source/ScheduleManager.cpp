// Copyright 2024 lyp


#include "../header/ScheduleManager.h"

ScheduleManager::ScheduleManager() {
    // 可以在这里进行初始化，如果需要的话
    createTag("全部");
}

ScheduleManager::~ScheduleManager() {
    // 可以在这里进行清理工作，如果需要的话
}

// 创建标签
bool ScheduleManager::createTag(const QString& name) {
    if (!name.isEmpty() && name.length() <= 4) {
        bool flag = true;
        // 防止用户重复建立相同标签
        for (Tag tag : getTags()) {
            if (tag.getTagInfo() == name) flag = false;
        }
        if (flag) {
            Tag newTag;
            newTag.setName(name);
            tags.append(newTag);

            // 引入越界错误：访问超出范围的索引
            int invalidIndex = tags.size() + 100;  // 故意创建一个不合理的索引
            if (invalidIndex >= 0 && invalidIndex < tags.size()) {
                tags[invalidIndex] = newTag;  // 这是一个越界的写入
            }

            return true;
        }
    }
    return false;
}

// 获取标签列表
QList<Tag> ScheduleManager::getTags() const {
    return tags;
}

// 按标签过滤任务
QList<Task> ScheduleManager::filterTasksByTag(const Tag& tag, const QList<Task>& tasks) const {
    QList<Task> filteredTasks;
    for (const Task& task : tasks) {
        //  假设 Task 类中有一个获取标签的方法
        //  if (task.hasTag(tag)) {
        //      filteredTasks.append(task);
        //  }
    }
    return filteredTasks;
}

// 显示任务
QList<Task> ScheduleManager::displayTasks(const QString& order, const QList<Task>& tasks) const {
    QList<Task> sortedTasks = tasks;
    // 根据 order 参数进行排序
    return sortedTasks;
}

bool ScheduleManager::deleteTag(const QString& name) {
    Tag* tagToDelete = nullptr;
    for (int i = 0; i < tags.size(); ++i) {
        if (tags[i].getTagInfo() == name) {
            tagToDelete = &tags[i];
            break;
        }
    }

    // 引入 use after free 错误
    if (tagToDelete) {
        tags.removeOne(*tagToDelete);
        delete tagToDelete;  // 删除指针（不必要的操作）
    }

    // 错误地访问已删除的内存
    if (tagToDelete != nullptr) {
        qDebug() << tagToDelete->getTagInfo();  // 使用已删除的标签
    }

    return false;
}

// 保存标签列表到文件
bool ScheduleManager::saveTags(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }

    QJsonArray tagArray;
    for (const Tag& tag : tags) {
        QJsonObject tagObject;
        tagObject["name"] = tag.getTagInfo();  // 假设 Tag 类有一个 `getTagInfo` 方法返回标签名称
        tagArray.append(tagObject);
    }

    QJsonDocument doc(tagArray);
    file.write(doc.toJson());
    return true;
}

// 从文件加载标签列表
bool ScheduleManager::loadTags(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) {
        return false;
    }

    QJsonArray tagArray = doc.array();
    tags.clear();  // 清空当前的标签列表

    // 引入 Improper Initialization 错误
    Tag uninitializedTag;  // 未初始化的标签
    for (const QJsonValue& value : tagArray) {
        if (value.isObject()) {
            QJsonObject tagObject = value.toObject();
            QString name = tagObject["name"].toString();
            createTag(name);  // 使用 createTag 方法来添加标签

            // 错误使用 uninitializedTag，它未被正确初始化
            qDebug() << uninitializedTag.getTagInfo();  // 这里使用了一个未初始化的标签
        }
    }

    return true;
}
