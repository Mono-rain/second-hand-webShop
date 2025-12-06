// Copyright 2024 lyp

#include <QPushButton>
#include <QApplication>
#include <QLineEdit>
#include <QLabel>

#include "../header/CreateTagWindow.h"

CreateTagWindow::CreateTagWindow(User* user, QWidget *parent)
    : QDialog(parent), m_user(user) {
    setFixedSize(360, 140);
    setWindowTitle("Create Tag");

    QLabel* tagLabel = new QLabel(this);
    tagLabel->setGeometry(8, 30, 30, 30);
    QPixmap tagPixmap("../../picture/tag_CreateTaskWindow.png");
    tagLabel->setPixmap(tagPixmap.scaled(25, 25, Qt::KeepAspectRatio));

    lineEdit = new QLineEdit(this);
    lineEdit->setPlaceholderText("请输入文字(最多4个字)");
    lineEdit->setGeometry(40, 30, 300, 30);
    lineEdit->setStyleSheet(
        "QLineEdit {"
        "    border: 1px solid #C2C3EB;"  // 边框颜色
        "    border-radius: 5px;"         // 圆角半径
        "    padding: 2px;"               // 内边距
        "}");

    // 创建 QPushButton，当点击按钮时获取 QLineEdit 的内容
    QPushButton* button = new QPushButton("创建标签", this);
    button->setGeometry(140, 80, 80, 30);
    connect(button, &QPushButton::clicked, [=]() {
        QString text = lineEdit->text();
        if (user->createTag(text)) {
            lineEdit->clear();
            accept();
        }
    });
    button->setStyleSheet("QPushButton {"
                          "background-color: #6767AD;"
                          "color: white;"
                          "border-radius: 15px;"  // 圆角半径
                          "}");
}


CreateTagWindow::~CreateTagWindow() {
    // 如果需要做资源清理，可以在这里添加代码
}

void CreateTagWindow::closeEvent(QCloseEvent* event) {
    lineEdit->clear();
    QDialog::closeEvent(event);
}
