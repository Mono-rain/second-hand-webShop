// Copyright 2024 lyp

#include <QMessageBox>
#include <QDebug>
#include <QPushButton>
#include <QDateTime>
#include <QIcon>

#include "../header/NotificationManager.h"

void NotificationManager::sendNotification(User* recipient, Task* task, Reminder* reminder) {
    // 创建消息框并显示通知
    QDateTime currentTime = QDateTime::currentDateTime();
    QDateTime reminderTime = task->getDeadline();  // 获取提醒时间

    // 计算时间差的秒数
    qint64 secondsDifference = currentTime.secsTo(reminderTime);

    // 计算天数、小时、分钟、秒数
    int days = secondsDifference / (60 * 60 * 24);  // 计算天数
    int hours = (secondsDifference % (60 * 60 * 24)) / (60 * 60);  // 计算小时
    int minutes = (secondsDifference % (60 * 60)) / 60;  // 计算分钟
    int seconds = secondsDifference % 60;  // 计算秒数

    // 格式化为字符串
    QString intervalStr = QString("%1天 %2小时 %3分钟 %4秒")
                              .arg(days)
                              .arg(hours)
                              .arg(minutes)
                              .arg(seconds);

    QString message = "你的任务“" + task->getTitle() + "”还没有完成！<br><br>距离截止日期还差" + intervalStr + "<br><br>(不开始是无法关闭的哦)";
    QMessageBox msgBox;
    msgBox.setText(message);  // 设置多行文本

    // 设置按钮的文本为更贴切的内容
    msgBox.setStandardButtons(QMessageBox::Ok);  // 只显示 OK 按钮
    QAbstractButton* okButton = msgBox.button(QMessageBox::Ok);
    okButton->setText("马上开始");  // 修改按钮文本

    // 设置消息框的标题（可选）
    msgBox.setWindowTitle("任务提醒");

    // 自定义样式，可以增强视觉效果
    msgBox.setStyleSheet("QMessageBox {"
                         "background-color: white; "
                         "font-size: 14px; "
                         "color: #2b2b2b; "
                         "border: 2px solid #6767ad; "
                         "border-radius: 10px; "
                         "padding: 10px; "
                         "text-align: center;"  // 让消息框的文本居中
                         "}"
                         "QMessageBox QLabel {"
                         "text-align: center;"  // 确保 QLabel 中的文本居中
                         "}"
                         "QMessageBox QPushButton {"
                         "background-color: #6767ad; "
                         "color: white; "
                         "font-size: 14px; "
                         "border-radius: 5px; "
                         "min-width: 100px; "
                         "text-align: center;"  // 让按钮上的文本居中
                         "}"
                         "QMessageBox QPushButton:hover {"
                         "background-color: #5f5f9a;"  // 鼠标悬停时按钮变色
                         "}");


    // 连接消息框的 finished 信号到 onMessageBoxClosed 槽
    connect(&msgBox, &QMessageBox::finished, this, [this, task, reminder](int result) {
        onMessageBoxClosed(result, task, reminder);  // 在这里将 task 和 reminder 传递给槽
    });

    // 显示消息框
    msgBox.exec();
}

void NotificationManager::customizeNotification(const QMap<QString, QString>& settings) {
    notificationSettings = settings;
}

// 处理消息框关闭后的动作
void NotificationManager::onMessageBoxClosed(int result, Task* task, Reminder* reminder) {
    // 处理任务和提醒
    assert(task && reminder);
    if (result == QMessageBox::Ok) {
        // 用户点击了 OK 按钮
        qDebug() << "用户点击了 OK 按钮";
        task->deleteReminder(reminder);
    }
    // 在消息框关闭后执行其他代码
    qDebug() << "消息框已关闭，继续执行其他操作...";
}
