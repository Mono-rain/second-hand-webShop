// Copyright 2024 lyp

#include "../header/FocusSession.h"

// 开始专注会话
void FocusSession::startSession() {
    // 开始会话的逻辑
}

// 结束专注会话
void FocusSession::endSession(const QString& status) {
    this->status = status;
}
