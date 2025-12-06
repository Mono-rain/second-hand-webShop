// Copyright 2024 lyp

#ifndef  HEADER_CREATETAGWINDOW_H_
#define  HEADER_CREATETAGWINDOW_H_

#include <QDialog>  // 使用 QDialog 而不是 QWidget
#include <QLineEdit>

#include "../header/User.h"

class CreateTagWindow : public QDialog {
    Q_OBJECT

 public:
    explicit CreateTagWindow(User* user, QWidget *parent);
     ~CreateTagWindow() override;

 private:
    User* m_user;  // 存储传入的 User 实例指针
    QLineEdit* lineEdit;
    void closeEvent(QCloseEvent* event);
};

#endif  //  HEADER_CREATETAGWINDOW_H_
