// Copyright 2024 lyp

#ifndef HEADER_TIMELINEVIEW_H_
#define HEADER_TIMELINEVIEW_H_

#include <QString>

class TimelineView {
 private:
    QString timeSpan;  // 时间跨度

 public:
    void displayTimeline();
    void switchView(const QString& viewType);
};

#endif  // HEADER_TIMELINEVIEW_H_
