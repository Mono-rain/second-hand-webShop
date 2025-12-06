// Copyright 2024 lyp

#include <QPushButton>
#include <QApplication>
#include <QLineEdit>
#include <QLabel>
#include <QDateTimeEdit>

#include "../header/CreateTaskWindow.h"

EditTaskWindow::EditTaskWindow(User* user, int number, QWidget *parent):
    QDialog(parent),
    m_user(user),
    titleEdit(new QLineEdit(this)),
    tagEdit(new QLineEdit(this)),
    deadlineEdit(new QDateTimeEdit(this)),
    reminderEdit(new QDateTimeEdit(this)),
    reminder_flag(false),
    taskNumber(number) {
    setFixedSize(360, 240);
    setWindowTitle("Edit Task");
    setStyleSheet("background-color: white;");
    Task task = m_user->getTasks()[taskNumber];

    titleEdit->setPlaceholderText(task.getTitle());
    titleEdit->setGeometry(40, 20, 300, 30);
    QLabel* titleLabel = new QLabel(this);
    titleLabel->setGeometry(10, 20, 30, 30);
    QPixmap titlePixmap("../../picture/title_CreateTaskWindow.png");
    titleLabel->setPixmap(titlePixmap.scaled(25, 25, Qt::KeepAspectRatio));  // 将图片缩放到适当的大小

    tagEdit->setPlaceholderText(task.getTag().getTagInfo() == "全部" ? "": task.getTag().getTagInfo());
    tagEdit->setGeometry(40, 60, 300, 30);
    QLabel* tagLabel = new QLabel(this);
    tagLabel->setGeometry(8, 60, 30, 30);
    QPixmap tagPixmap("../../picture/tag_CreateTaskWindow.png");
    tagLabel->setPixmap(tagPixmap.scaled(25, 25, Qt::KeepAspectRatio));  // 将图片缩放到适当的大小

    deadlineEdit->setCalendarPopup(true);
    deadlineEdit->setGeometry(40, 100, 300, 30);
    deadlineEdit->setDateTime(task.getDeadline());
    QLabel* deadlineLabel = new QLabel(this);
    deadlineLabel->setGeometry(5, 100, 30, 30);
    QPixmap deadlinePixmap("../../picture/Icon_Fill_clock.png");
    deadlineLabel->setPixmap(deadlinePixmap.scaled(30, 30, Qt::KeepAspectRatio));  // 将图片缩放到适当的大小

    reminderEdit->setCalendarPopup(true);
    reminderEdit->setGeometry(40, 140, 300, 30);
    reminderEdit->setDateTime(task.getDeadline());
    QLabel* reminderLabel = new QLabel(this);
    reminderLabel->setGeometry(8, 140, 30, 30);
    QPixmap reminderPixmap("../../picture/alarm_clock.png");
    reminderLabel->setPixmap(reminderPixmap.scaled(25, 25, Qt::KeepAspectRatio));  // 将图片缩放到适当的大小

    ToggleSwitch *toggleSwitch = new ToggleSwitch(this);
    toggleSwitch->move(6, 180);
    // 连接滑动开关的状态变化信号
    connect(toggleSwitch, &ToggleSwitch::toggled, this, [this](bool checked) {
        reminder_flag = checked;
    });

    QPushButton* editButton = new QPushButton("修改任务", this);
    editButton->setGeometry(60, 190, 80, 30);
    connect(editButton, &QPushButton::clicked, [=]() {
        QString titleText = titleEdit->text();
        QString tagText = tagEdit->text();
        QDateTime deadline = deadlineEdit->dateTime();
        QDateTime reminder = reminderEdit->dateTime();

        if (m_user->editTask(m_user->getTasks()[taskNumber], titleText, tagText, "", deadline, -1, reminder_flag, reminder)) {
            accept();
        }
    });

    QPushButton* deleteButton = new QPushButton("删除任务", this);
    deleteButton->setGeometry(220, 190, 80, 30);
    connect(deleteButton, &QPushButton::clicked, [=]() {
        QString titleText = titleEdit->text();
        QString tagText = tagEdit->text();
        QDateTime deadline = deadlineEdit->dateTime();
        QDateTime reminder = reminderEdit->dateTime();

        m_user->deleteTask(m_user->getTasks()[taskNumber]);
        accept();
    });

    titleEdit->setStyleSheet(
        "QLineEdit {"
        "    border: 1px solid #C2C3EB;"  // 边框颜色
        "    border-radius: 5px;"         // 圆角半径
        "    padding: 2px;"               // 内边距
        "}");

    tagEdit->setStyleSheet(
        "QLineEdit {"
        "    border: 1px solid #C2C3EB;"  // 边框颜色
        "    border-radius: 5px;"         // 圆角半径
        "    padding: 2px;"               // 内边距
        "}");
    deadlineEdit->setStyleSheet(
        "QDateTimeEdit {"
        "    border: 1px solid #C2C3EB;"
        "    border-radius: 5px;"
        "    padding: 2px;"
        "}"
        "QDateTimeEdit::drop-down {"
        "    subcontrol-origin: padding;"
        "    subcontrol-position: top right;"
        "    width: 20px;"
        "    border-left: 1px solid #6767AD;"
        "}"
        "QDateTimeEdit::down-arrow {"
        "    image: url(../../picture/down_CreateTaskWindow.png);"  // 设置图标（需将图标路径改为实际路径）
        "    width: 10px;"
        "    height: 10px;"
        "}"
        "QCalendarWidget {"
        "    background-color: white;"
        "    border: none;"  // 去掉整体边框
        "    border-radius: 5px;"
        "}"
        "QCalendarWidget QToolButton {"
        "    height: 30px;"
        "    width: 90px;"
        "    color: white;"
        "    background-color: #6767AD;"
        "    font-size: 14px;"
        "    icon-size: 20px, 20px;"
        "    border: none;"  // 去掉按钮边框，避免边框重叠
        "}"
        "QCalendarWidget QToolButton:hover {"
        "    background-color: rgb(83, 83, 140);"  // 鼠标悬浮时按钮颜色
        "}"
        "QCalendarWidget QMenu {"
        "    width: 150px;"
        "    color: black;"
        "    background-color: white;"
        "    border: 1px solid #6767AD;"
        "}"
        "QCalendarWidget QSpinBox {"
        "    width: 50px;"
        "    font-size: 14px;"
        "    color: #6767AD;"
        "    background: white;"
        "    selection-background-color: #6767AD;"
        "    selection-color: white;"
        "}"
        "QCalendarWidget QAbstractItemView:enabled {"
        "    font-size: 14px;"
        "    color: black;"
        "    background-color: white;"
        "    selection-background-color: #6767AD;"
        "    selection-color: white;"
        "    outline: none;"  // 去掉选中框的边框线
        "}"
        "QCalendarWidget QAbstractItemView:disabled {"
        "    color: gray;"
        "}");
    reminderEdit->setStyleSheet(
        "QDateTimeEdit {"
        "    border: 1px solid #C2C3EB;"
        "    border-radius: 5px;"
        "    padding: 2px;"
        "}"
        "QDateTimeEdit::drop-down {"
        "    subcontrol-origin: padding;"
        "    subcontrol-position: top right;"
        "    width: 20px;"
        "    border-left: 1px solid #6767AD;"
        "}"
        "QDateTimeEdit::down-arrow {"
        "    image: url(../../picture/down_CreateTaskWindow.png);"  // 设置图标（需将图标路径改为实际路径）
        "    width: 10px;"
        "    height: 10px;"
        "}"
        "QCalendarWidget {"
        "    background-color: white;"
        "    border: none;"  // 去掉整体边框
        "    border-radius: 5px;"
        "}"
        "QCalendarWidget QToolButton {"
        "    height: 30px;"
        "    width: 90px;"
        "    color: white;"
        "    background-color: #6767AD;"
        "    font-size: 14px;"
        "    icon-size: 20px, 20px;"
        "    border: none;"  // 去掉按钮边框，避免边框重叠
        "}"
        "QCalendarWidget QToolButton:hover {"
        "    background-color: rgb(83, 83, 140);"  // 鼠标悬浮时按钮颜色
        "}"
        "QCalendarWidget QMenu {"
        "    width: 150px;"
        "    color: black;"
        "    background-color: white;"
        "    border: 1px solid #6767AD;"
        "}"
        "QCalendarWidget QSpinBox {"
        "    width: 50px;"
        "    font-size: 14px;"
        "    color: #6767AD;"
        "    background: white;"
        "    selection-background-color: #6767AD;"
        "    selection-color: white;"
        "}"
        "QCalendarWidget QAbstractItemView:enabled {"
        "    font-size: 14px;"
        "    color: black;"
        "    background-color: white;"
        "    selection-background-color: #6767AD;"
        "    selection-color: white;"
        "    outline: none;"  // 去掉选中框的边框线
        "}"
        "QCalendarWidget QAbstractItemView:disabled {"
        "    color: gray;"
        "}");
    editButton->setStyleSheet("QPushButton {"
                          "background-color: #6767AD;"
                          "color: white;"
                          "border-radius: 15px;"  // 圆角半径
                          "}");
    deleteButton->setStyleSheet("QPushButton {"
                              "background-color: #FF0000;"
                              "color: white;"
                              "border-radius: 15px;"  // 圆角半径
                              "}");
}


EditTaskWindow::~EditTaskWindow() {
    // 如果需要做资源清理，可以在这里添加代码
}

void EditTaskWindow::closeEvent(QCloseEvent* event) {
    titleEdit->clear();
    tagEdit->clear();
    deadlineEdit->clear();
    reminderEdit->clear();
    QDialog::closeEvent(event);
}

void EditTaskWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);  // 抗锯齿

    // 绘制圆角矩形
    QPen pen(QColor(194, 195, 235), 1);  // 黑色边框，线宽为2
    painter.setPen(pen);
    painter.setBrush(Qt::white);  // 白色填充
    QRectF rect(6, 180, 30, 10);  // 边框大小
    painter.drawRoundedRect(rect, 5, 5);  // 圆角半径为5
}

CreateTaskWindow::CreateTaskWindow(User* user, QWidget *parent):
    QDialog(parent),
    m_user(user),
    titleEdit(new QLineEdit(this)),
    tagEdit(new QLineEdit(this)),
    deadlineEdit(new QDateTimeEdit(this)),
    reminderEdit(new QDateTimeEdit(this)),
    reminder_flag(false) {
    setFixedSize(360, 240);
    setWindowTitle("Create Task");
    setStyleSheet("background-color: white;");

    titleEdit->setPlaceholderText("请输入标题(不可为空)");
    titleEdit->setGeometry(40, 20, 300, 30);
    QLabel* titleLabel = new QLabel(this);
    titleLabel->setGeometry(10, 20, 30, 30);
    QPixmap titlePixmap("../../picture/title_CreateTaskWindow.png");
    titleLabel->setPixmap(titlePixmap.scaled(25, 25, Qt::KeepAspectRatio));  // 将图片缩放到适当的大小

    tagEdit->setPlaceholderText("请输入标签(最多输入4个字)");
    tagEdit->setGeometry(40, 60, 300, 30);
    QLabel* tagLabel = new QLabel(this);
    tagLabel->setGeometry(8, 60, 30, 30);
    QPixmap tagPixmap("../../picture/tag_CreateTaskWindow.png");
    tagLabel->setPixmap(tagPixmap.scaled(25, 25, Qt::KeepAspectRatio));  // 将图片缩放到适当的大小

    deadlineEdit->setCalendarPopup(true);
    deadlineEdit->setGeometry(40, 100, 300, 30);
    deadlineEdit->setDateTime(QDateTime::currentDateTime());
    QLabel* deadlineLabel = new QLabel(this);
    deadlineLabel->setGeometry(5, 100, 30, 30);
    QPixmap deadlinePixmap("../../picture/Icon_Fill_clock.png");
    deadlineLabel->setPixmap(deadlinePixmap.scaled(30, 30, Qt::KeepAspectRatio));  // 将图片缩放到适当的大小

    reminderEdit->setCalendarPopup(true);
    reminderEdit->setGeometry(40, 140, 300, 30);
    reminderEdit->setDateTime(QDateTime::currentDateTime());
    QLabel* reminderLabel = new QLabel(this);
    reminderLabel->setGeometry(8, 140, 30, 30);
    QPixmap reminderPixmap("../../picture/alarm_clock.png");
    reminderLabel->setPixmap(reminderPixmap.scaled(25, 25, Qt::KeepAspectRatio));  // 将图片缩放到适当的大小

    ToggleSwitch *toggleSwitch = new ToggleSwitch(this);
    toggleSwitch->move(6, 180);
    // 连接滑动开关的状态变化信号
    connect(toggleSwitch, &ToggleSwitch::toggled, this, [this](bool checked) {
        reminder_flag = checked;
    });

    QPushButton* button = new QPushButton("创建任务", this);
    button->setGeometry(140, 190, 80, 30);
    connect(button, &QPushButton::clicked, [=]() {
        QString titleText = titleEdit->text();
        QString tagText = tagEdit->text();
        QDateTime deadline = deadlineEdit->dateTime();
        QDateTime reminder = reminderEdit->dateTime();

        if (m_user->createTask(titleText, tagText, "", deadline, -1, reminder_flag, reminder)) {
            titleEdit->clear();
            tagEdit->clear();
            deadlineEdit->clear();
            reminderEdit->clear();
            accept();
        }
    });

    titleEdit->setStyleSheet(
        "QLineEdit {"
        "    border: 1px solid #C2C3EB;"  // 边框颜色
        "    border-radius: 5px;"         // 圆角半径
        "    padding: 2px;"               // 内边距
        "}");
    tagEdit->setStyleSheet(
        "QLineEdit {"
        "    border: 1px solid #C2C3EB;"  // 边框颜色
        "    border-radius: 5px;"         // 圆角半径
        "    padding: 2px;"               // 内边距
        "}");
    deadlineEdit->setStyleSheet(
        "QDateTimeEdit {"
        "    border: 1px solid #C2C3EB;"
        "    border-radius: 5px;"
        "    padding: 2px;"
        "}"
        "QDateTimeEdit::drop-down {"
        "    subcontrol-origin: padding;"
        "    subcontrol-position: top right;"
        "    width: 20px;"
        "    border-left: 1px solid #6767AD;"
        "}"
        "QDateTimeEdit::down-arrow {"
        "    image: url(../../picture/down_CreateTaskWindow.png);"
        "    width: 10px;"
        "    height: 10px;"
        "}"
        "QCalendarWidget {"
        "    background-color: white;"
        "    border: none;"  // 去掉整体边框
        "    border-radius: 5px;"
        "}"
        "QCalendarWidget QToolButton {"
        "    height: 30px;"
        "    width: 90px;"
        "    color: white;"
        "    background-color: #6767AD;"
        "    font-size: 14px;"
        "    icon-size: 20px, 20px;"
        "    border: none;"  // 去掉按钮边框，避免边框重叠
        "}"
        "QCalendarWidget QToolButton:hover {"
        "    background-color: rgb(83, 83, 140);"  // 鼠标悬浮时按钮颜色
        "}"
        "QCalendarWidget QMenu {"
        "    width: 150px;"
        "    color: black;"
        "    background-color: white;"
        "    border: 1px solid #6767AD;"
        "}"
        "QCalendarWidget QSpinBox {"
        "    width: 50px;"
        "    font-size: 14px;"
        "    color: #6767AD;"
        "    background: white;"
        "    selection-background-color: #6767AD;"
        "    selection-color: white;"
        "}"
        "QCalendarWidget QAbstractItemView:enabled {"
        "    font-size: 14px;"
        "    color: black;"
        "    background-color: white;"
        "    selection-background-color: #6767AD;"
        "    selection-color: white;"
        "    outline: none;"  // 去掉选中框的边框线
        "}"
        "QCalendarWidget QAbstractItemView:disabled {"
        "    color: gray;"
        "}");
    reminderEdit->setStyleSheet(
        "QDateTimeEdit {"
        "    border: 1px solid #C2C3EB;"
        "    border-radius: 5px;"
        "    padding: 2px;"
        "}"
        "QDateTimeEdit::drop-down {"
        "    subcontrol-origin: padding;"
        "    subcontrol-position: top right;"
        "    width: 20px;"
        "    border-left: 1px solid #6767AD;"
        "}"
        "QDateTimeEdit::down-arrow {"
        "    image: url(../../picture/down_CreateTaskWindow.png);"
        "    width: 10px;"
        "    height: 10px;"
        "}"
        "QCalendarWidget {"
        "    background-color: white;"
        "    border: none;"  // 去掉整体边框
        "    border-radius: 5px;"
        "}"
        "QCalendarWidget QToolButton {"
        "    height: 30px;"
        "    width: 90px;"
        "    color: white;"
        "    background-color: #6767AD;"
        "    font-size: 14px;"
        "    icon-size: 20px, 20px;"
        "    border: none;"  // 去掉按钮边框，避免边框重叠
        "}"
        "QCalendarWidget QToolButton:hover {"
        "    background-color: rgb(83, 83, 140);"  // 鼠标悬浮时按钮颜色
        "}"
        "QCalendarWidget QMenu {"
        "    width: 150px;"
        "    color: black;"
        "    background-color: white;"
        "    border: 1px solid #6767AD;"
        "}"
        "QCalendarWidget QSpinBox {"
        "    width: 50px;"
        "    font-size: 14px;"
        "    color: #6767AD;"
        "    background: white;"
        "    selection-background-color: #6767AD;"
        "    selection-color: white;"
        "}"
        "QCalendarWidget QAbstractItemView:enabled {"
        "    font-size: 14px;"
        "    color: black;"
        "    background-color: white;"
        "    selection-background-color: #6767AD;"
        "    selection-color: white;"
        "    outline: none;"  // 去掉选中框的边框线
        "}"
        "QCalendarWidget QAbstractItemView:disabled {"
        "    color: gray;"
        "}");
    button->setStyleSheet("QPushButton {"
                          "background-color: #6767AD;"
                          "color: white;"
                          "border-radius: 15px;"  // 圆角半径
                          "}");
}


CreateTaskWindow::~CreateTaskWindow() {
    // 如果需要做资源清理，可以在这里添加代码
}

void CreateTaskWindow::closeEvent(QCloseEvent* event) {
    titleEdit->clear();
    tagEdit->clear();
    deadlineEdit->clear();
    reminderEdit->clear();
    QDialog::closeEvent(event);
}

void CreateTaskWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);  // 抗锯齿

    // 绘制圆角矩形
    QPen pen(QColor(194, 195, 235), 1);  // 黑色边框，线宽为2
    painter.setPen(pen);
    painter.setBrush(Qt::white);  // 白色填充
    QRectF rect(6, 180, 30, 10);  // 边框大小
    painter.drawRoundedRect(rect, 5, 5);  // 圆角半径为5
}

// ToggleSwitch 实现
ToggleSwitch::ToggleSwitch(QWidget *parent) :
    QPushButton(parent),
    m_checked(false) {
    setCheckable(true);
    setFixedSize(20, 10);
    setStyleSheet(getStyle());
    connect(this, &QPushButton::clicked, this, &ToggleSwitch::toggleState);
}

bool ToggleSwitch::isChecked() const {
    return m_checked;
}

void ToggleSwitch::setChecked(bool checked) {
    m_checked = checked;
    setStyleSheet(getStyle());
}

void ToggleSwitch::toggleState() {
    setChecked(!m_checked);
    animate();
    emit toggled(m_checked);
}

QString ToggleSwitch::getStyle() {
    return m_checked
               ? "QPushButton { background-color: #4CAF50; border-radius: 5px; }"
                 "QPushButton::indicator { background-color: white; border-radius: 5px; width: 30px; height: 30px; margin-left: 30px; }"
               : "QPushButton { background-color: #ccc; border-radius: 5px; }"
                 "QPushButton::indicator { background-color: white; border-radius: 5px; width: 30px; height: 30px; }";
}

void ToggleSwitch::animate() {
    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(150);
    QRect startGeometry = geometry();
    QRect endGeometry = startGeometry;
    endGeometry.moveLeft(m_checked ? startGeometry.left() + 10 : startGeometry.left() - 10);
    animation->setStartValue(startGeometry);
    animation->setEndValue(endGeometry);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}
