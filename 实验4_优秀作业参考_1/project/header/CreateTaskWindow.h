// Copyright 2024 lyp

#ifndef  HEADER_CREATETASKWINDOW_H_
#define  HEADER_CREATETASKWINDOW_H_

#include <QDialog>  // 使用 QDialog 而不是 QWidget
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QPushButton>
#include <QPainter>
#include <QPropertyAnimation>

#include "../header/User.h"

class ToggleSwitch : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(bool checked READ isChecked WRITE setChecked)

 public:
    explicit ToggleSwitch(QWidget *parent = nullptr);
    bool isChecked() const;
    void setChecked(bool checked);

 signals:
    void toggled(bool checked);

 private slots:
    void toggleState();

 private:
    bool m_checked;
    QString getStyle();
    void animate();
};

class EditTaskWindow : public QDialog {
    Q_OBJECT

 public:
    explicit EditTaskWindow(User* user, int number, QWidget *parent);
    ~EditTaskWindow() override;

 private:
    User* m_user;  // 存储传入的 User 实例指针
    QLineEdit *titleEdit, *tagEdit;
    QDateTimeEdit *deadlineEdit, *reminderEdit;
    bool reminder_flag;
    int taskNumber;

    void closeEvent(QCloseEvent* event);
    void paintEvent(QPaintEvent *event) override;
};

class CreateTaskWindow : public QDialog {
    Q_OBJECT

 public:
    explicit CreateTaskWindow(User* user, QWidget *parent);
    ~CreateTaskWindow() override;

 private:
    User* m_user;  // 存储传入的 User 实例指针
    QLineEdit *titleEdit, *tagEdit;
    QDateTimeEdit *deadlineEdit, *reminderEdit;
    bool reminder_flag;

    void closeEvent(QCloseEvent* event);
    void paintEvent(QPaintEvent *event) override;
};

#endif  //  HEADER_CREATETASKWINDOW_H_
