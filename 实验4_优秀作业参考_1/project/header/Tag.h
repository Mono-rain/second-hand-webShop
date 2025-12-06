// Copyright 2024 lyp

#ifndef  HEADER_TAG_H_
#define  HEADER_TAG_H_

#include <QString>
#include <QJsonObject>

class Tag {
 private:
    QString name;  // 标签名称

 public:
    void setName(const QString& newName);
    QString getTagInfo() const;
    bool operator==(const Tag& other) const {
        return this->name == other.name;  // 比较 name 字段
    }
    // 转换为 QJsonObject (序列化)
    QJsonObject toJson() const {
        QJsonObject json;
        json["name"] = name;
        return json;
    }
    // 从 QJsonObject 创建 Tag 对象 (反序列化)
    static Tag fromJson(const QJsonObject& json) {
        Tag tag;
        tag.name = json["name"].toString();
        return tag;
    }
};

#endif  // HEADER_TAG_H_
