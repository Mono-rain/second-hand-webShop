// Copyright 2024 lyp

#ifndef HEADER_MAINWINDOW_H_
#define HEADER_MAINWINDOW_H_

#include <QMainWindow>
#include <QPushButton>
#include <QJsonObject>

#include "../header/User.h"
#include "../header/CreateTagWindow.h"
#include "../header/CreateTaskWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

 public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

 private:
    Ui::MainWindow *ui;

    QString uiName;
    User user;  // 创建 User 类的实例(因为是跟mainwindow的生命周期保持一致，所以就不使用指针了)

    CreateTagWindow* createTagWindow;  // 创建标签的界面
    CreateTaskWindow* createTaskWindow;  // 创建任务的界面
    EditTaskWindow* editTaskWindow;  // 修改任务的界面
    int tagLeftSign, tagSelectSign;  // 显示标签最左边按钮的序号，被选中标签的序号
    int taskTopSign[100];  // 显示任务最上边按钮的序号(最多创建100个标签)
    int timelineViewTopSign;

    // 其他功能函数可以在这里声明

 private:
    void onButtonClicked(QPushButton *clickedButton);
    void paintButton(
        const QString& name,
        const QString& type,
        int x, int y, int w, int h,
        const QList<QString>& buttonTexts,
        const QList<QString>& formats,
        const QList<QRect>& labelRects,
        const QString& iconPath,
        const QString& style,
        std::function<void(QPushButton *)> onClick,
        const QList<Qt::Alignment>& alignments);

    void paintNavigationBar();

    void paintScheduleTaskList();
    void paintScheduleTop();
    void paintScheduleManager();

    void paintTimelineTaskList();
    void paintTimelineTop();
    void paintTimelineView();

    void paintEvent(QPaintEvent *event) override;  // 重写 paintEvent 方法
};
#endif  // HEADER_MAINWINDOW_H_
