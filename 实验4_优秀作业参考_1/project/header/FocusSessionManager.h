// Copyright 2024 lyp

#ifndef HEADER_FOCUSSESSIONMANAGER_H_
#define HEADER_FOCUSSESSIONMANAGER_H_

#include <QList>

#include "../header/FocusSession.h"

class FocusSessionManager {
 private:
    FocusSession currentSession;       // 当前会话
    QList<FocusSession> focusSessions;  // 专注会话列表
    int rewardBalance;                  // 奖励余额

 public:
    FocusSession startFocusSession(int duration);
    void endFocusSession(const QString& feedback);
    void addReward(int points);
};

#endif  // HEADER_FOCUSSESSIONMANAGER_H_
