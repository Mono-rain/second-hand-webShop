// Copyright 2024 lyp

#ifndef HEADER_FOCUSSESSION_H_
#define HEADER_FOCUSSESSION_H_

#include <QDateTime>

class FocusSession {
private:
    QDateTime startTime;  // 开始时间
    int duration;         // 持续时间
    int reward;           // 奖励
    QString status;       // 状态

public:
    void startSession();
    void endSession(const QString& status);
};

#endif  // HEADER_FOCUSSESSION_H_
