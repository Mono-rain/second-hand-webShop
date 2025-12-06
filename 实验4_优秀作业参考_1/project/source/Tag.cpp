// Copyright 2024 lyp

#include "../header/Tag.h"

// 设置标签名称
void Tag::setName(const QString& newName) { name = newName; }

// 获取标签信息
QString Tag::getTagInfo() const {
    return name;
}
