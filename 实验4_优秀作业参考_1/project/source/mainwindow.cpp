// Copyright 2024 lyp

#include <QApplication>
#include <QPushButton>
#include <QMessageBox>
#include <QWidget>
#include <QPainter>
#include <QRegularExpression>
#include <QPainterPath>
#include <QLabel>
#include <QLayout>

#include "../header/mainwindow.h"
#include "build\Desktop_Qt_6_6_3_MinGW_64_bit-Debug/ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    uiName("日程"),
    tagLeftSign(0), tagSelectSign(0), timelineViewTopSign(0),
    createTagWindow(nullptr), createTaskWindow(nullptr), editTaskWindow(nullptr) {

    ui->setupUi(this);
    setFixedSize(360, 780);  // 设置窗口大小为360x780像素
    std::fill(std::begin(taskTopSign), std::end(taskTopSign), 0);
}

MainWindow::~MainWindow() {
    delete createTagWindow;
    delete createTaskWindow;
    delete ui;
}

void MainWindow::onButtonClicked(QPushButton *clickedButton) {
    // 标签名字的集合
    QList<QString> tagNames;
    for (Tag tag : user.getTags())
        tagNames.append(tag.getTagInfo());

    if (clickedButton -> property("name").toString() == "创建任务") {
        if (!createTaskWindow) {  // 确保只创建一次
            createTaskWindow = new CreateTaskWindow(&user, nullptr);  // 创建对话框
        }
        assert(createTaskWindow != nullptr);
        createTaskWindow->exec();  // 使用 exec() 弹出模态对话框
        // 在关闭对话框时候会自动执行一次paintevent函数，所以需要把控件全部删除重新绘图
        QList<QWidget *> widgets = this->findChildren<QWidget *>();
        foreach(QWidget *widget, widgets) {
            widget->deleteLater();
        }
        /*QDateTime deadline = QDateTime::currentDateTime();
        user.createTask("写pa实验", "学习", "", deadline, -1, false, QDateTime::currentDateTime());
        user.createTask("写网络攻防实验", "学习", "", deadline, -1, false, QDateTime::currentDateTime());
        user.createTask("写软件工程实验", "学习", "", deadline, -1, false, QDateTime::currentDateTime());
        user.createTask("写智能计算系统实验", "学习", "", deadline, -1, false, QDateTime::currentDateTime());
        user.createTask("写软件分析实验", "学习", "", deadline, -1, false, QDateTime::currentDateTime());
        user.createTask("写概率论与数理统计", "学习", "", deadline, -1, false, QDateTime::currentDateTime());
        user.createTask("写数电实验", "学习", "", deadline, -1, false, QDateTime::currentDateTime());
        user.createTask("写软件质量保障", "学习", "", deadline, -1, false, QDateTime::currentDateTime());
        user.createTask("写期末卷子", "学习", "", deadline, -1, false, QDateTime::currentDateTime());
        user.createTask("写期中卷子", "学习", "", deadline, -1, false, QDateTime::currentDateTime());
        user.createTask("写lab实验", "学习", "", deadline, -1, false, QDateTime::currentDateTime());
        user.createTask("吃饭", "生活", "", deadline, -1, false, QDateTime::currentDateTime());
        user.createTask("参加广播操排练", "工作", "", deadline, -1, false, QDateTime::currentDateTime());
        user.createTask("参加组会", "学习", "", deadline, -1, false, QDateTime::currentDateTime());
        user.createTask("出去喝酒", "生活", "", deadline.addDays(1), -1, false, QDateTime::currentDateTime());
        user.createTask("参加入场式排练", "工作", "", deadline.addDays(2), -1, false, QDateTime::currentDateTime());
        user.createTask("参加节目审核", "工作", "", deadline.addDays(3), -1, false, QDateTime::currentDateTime());
        user.createTask("参加闭幕式", "工作", "", deadline, -1, false, QDateTime::currentDateTime());*/
        return;
    }

    if (clickedButton->property("type").toString() == "任务") {
        // 使用正则表达式提取数字
        QRegularExpression regex("(\\d+)");
        QRegularExpressionMatch match = regex.match(clickedButton->property("name").toString());
        int number = -1;
        if (match.hasMatch()) {
            // 提取匹配的数字字符串
            QString numberStr = match.captured(1);
            // 转换为 int 并返回
            number = numberStr.toInt();
        }
        assert(number != -1);

        if (!editTaskWindow) {
            editTaskWindow = new EditTaskWindow(&user, number, nullptr);  // 创建对话框
            qDebug() << user.getTasks()[number].getTitle();
        }
        editTaskWindow->exec();  // 使用 exec() 弹出模态对话框

        delete editTaskWindow;
        editTaskWindow = nullptr;

        QList<QWidget *> widgets = this->findChildren<QWidget *>();
        foreach(QWidget *widget, widgets) {
            widget->deleteLater();
        }

        return;
    }

    if (clickedButton->property("name").toString() == "更多功能") {
        if (!createTagWindow) {  // 确保只创建一次
            createTagWindow = new CreateTagWindow(&user, nullptr);  // 创建对话框
        }
        assert(createTagWindow != nullptr);
        createTagWindow->exec();  // 使用 exec() 弹出模态对话框
        QList<QWidget *> widgets = this->findChildren<QWidget *>();
        foreach(QWidget *widget, widgets) {
            widget->deleteLater();
        }
        return;
    }

    if (clickedButton -> property("type").toString() == "底部导航栏") {
        uiName = clickedButton -> property("name").toString();
        QList<QWidget *> widgets = this->findChildren<QWidget *>();
        foreach(QWidget *widget, widgets) {
            widget->deleteLater();
        }
        update();
        return;
    }

    if (clickedButton -> property("name").toString() == "任务向上移动" && clickedButton->property("type").toString() == "移动按键") {
        int tagTaskNum = 0;
        for (Task task : user.getTasks()) {
            if (tagNames[tagSelectSign] == "全部" ||task.getTag().getTagInfo() == tagNames[tagSelectSign])
                tagTaskNum++;
        }
        if (tagTaskNum > 9) {
            // 遍历所有子控件并删除它们
            QList<QPushButton *> buttons = this->findChildren<QPushButton *>();
            foreach(QPushButton *button, buttons) {
                if (button->property("type").toString() == "任务")
                    button->deleteLater();
            }
            for (int i = taskTopSign[tagSelectSign] - 1; i >= 0; i--) {
                if (tagNames[tagSelectSign] == "全部" || user.getTasks()[i].getTag().getTagInfo() == tagNames[tagSelectSign]) {
                    taskTopSign[tagSelectSign] = i;
                    break;
                }
            }
        }
        return;
    }

    if (clickedButton -> property("name").toString() == "任务向下移动" && clickedButton->property("type").toString() == "移动按键") {
        int tagTaskNum = 0;
        for (Task task : user.getTasks()) {
            if (tagNames[tagSelectSign] == "全部" ||task.getTag().getTagInfo() == tagNames[tagSelectSign])
                tagTaskNum++;
        }
        if (tagTaskNum > 9) {
            // 遍历所有子控件并删除它们
            QList<QPushButton *> buttons = this->findChildren<QPushButton *>();
            foreach(QPushButton *button, buttons) {
                if (button->property("type").toString() == "任务")
                    button->deleteLater();
            }
            for (int i = taskTopSign[tagSelectSign] + 1; i + 9 <= tagTaskNum; i++) {
                if (tagNames[tagSelectSign] == "全部" || user.getTasks()[i].getTag().getTagInfo() == tagNames[tagSelectSign]) {
                    taskTopSign[tagSelectSign] = i;
                    break;
                }
            }
        }
        return;
    }

    if (clickedButton -> property("name").toString() == "标签向左移动" && clickedButton->property("type").toString() == "移动按键") {
        // 遍历所有子控件并删除它们
        QList<QPushButton *> buttons = this->findChildren<QPushButton *>();
        foreach(QPushButton *button, buttons) {
            if (button->property("type").toString() == "标签")
                button->deleteLater();
        }
        if (tagLeftSign - 1 >= 0) {
            tagLeftSign--;
        }
        return;
    }

    if (clickedButton -> property("name").toString() == "标签向右移动" && clickedButton->property("type").toString() == "移动按键") {
        // 遍历所有子控件并删除它们
        QList<QPushButton *> buttons = this->findChildren<QPushButton *>();
        foreach(QPushButton *button, buttons) {
            if (button->property("type").toString() == "标签")
                button->deleteLater();
        }
        if (tagLeftSign + 4 < user.getTags().size()) {
            tagLeftSign++;
        }
        return;
    }

    if (clickedButton->property("type").toString() == "标签" && tagNames.contains(clickedButton->property("name").toString())) {
        // 删除所有任务控件
        QList<QPushButton *> buttons = this->findChildren<QPushButton *>();
        foreach(QPushButton *button, buttons) {
            if (button->property("type").toString() == "任务")
                button->deleteLater();
        }

        // 获取之前被选中的按钮
        QPushButton* selectedButton = nullptr;
        for (QPushButton *button : findChildren<QPushButton*>()) {
            if (button->property("type").toString() == "标签" &&
                tagNames[tagSelectSign] == button->property("name").toString()) {  // 如果是之前被选中的按钮
                selectedButton = button;
            }
        }

        if (selectedButton == nullptr || clickedButton->property("name").toString() != selectedButton->property("name").toString()) {
            // 设置按钮变大的大小和新的位置
            clickedButton->setGeometry(QRect(clickedButton->pos().x(), clickedButton->pos().y() - 5, clickedButton->size().width(), clickedButton->size().height() + 5));
            clickedButton->setStyleSheet("QPushButton { "
                                         "border: none; "
                                         "background-color: #6767AD; "
                                         "border-top-left-radius: 15px; "
                                         "border-top-right-radius: 15px; "
                                         "border-bottom-left-radius: 0; "
                                         "border-bottom-right-radius: 0; "
                                         "color: white; "
                                         "font-weight: bold; "
                                         "font-size: 20px; "
                                         "}"
                                         "QPushButton:hover { "
                                         "background-color: rgba(0, 0, 0, 0.1); "
                                         "}"
                                         "QPushButton:pressed { "
                                         "background-color: rgba(0, 0, 0, 0.2); "
                                         "}");
            for (QLabel* label : clickedButton->findChildren<QLabel*>()) {
                label->setGeometry(QRect(0, 0, 70, 30));
                label->setAlignment(Qt::AlignCenter);  // 居中对齐文本
            }
            // 修改被选中按钮的序号
            for (QString name : tagNames) {
                if (name == clickedButton->property("name").toString()) {
                    assert(tagSelectSign != tagNames.indexOf(name));  // 之前被选中按钮和现在的按钮不可能一样
                    tagSelectSign = tagNames.indexOf(name);
                }
            }
            if (selectedButton != nullptr) {
                // 设置按钮变小的大小和新的位置
                selectedButton->setGeometry(QRect(selectedButton->pos().x(), selectedButton->pos().y() + 5, selectedButton->size().width(), selectedButton->size().height() - 5));
                selectedButton->setStyleSheet("QPushButton {"
                                              "border: none; "
                                              "background-color: #BFBFDB; "
                                              "border-top-left-radius: 15px; "
                                              "border-top-right-radius: 15px; "
                                              "border-bottom-left-radius: 0; "
                                              "border-bottom-right-radius: 0; "
                                              "color: white; "
                                              "font-weight: bold; "
                                              "font-size: 20px;"
                                              "}"
                                              "QPushButton:hover {"
                                              "background-color: rgba(0, 0, 0, 0.1);"
                                              "}"
                                              "QPushButton:pressed {"
                                              "background-color: rgba(0, 0, 0, 0.2);"
                                              "}");
                for (QLabel* label : selectedButton->findChildren<QLabel*>()) {
                    label->setGeometry(QRect(0, 0, 70, 25));
                    label->setAlignment(Qt::AlignCenter);  // 居中对齐文本
                }
            }
        }
        return;
    }

    if (clickedButton -> property("name").toString() == "视图向上移动" && clickedButton->property("type").toString() == "移动按键") {
        QList<QWidget *> widgets = this->findChildren<QWidget *>();
        foreach(QWidget *widget, widgets) {
            widget->deleteLater();
        }
        if (timelineViewTopSign - 1 >= 0)
            timelineViewTopSign--;
    }

    if (clickedButton -> property("name").toString() == "视图向下移动" && clickedButton->property("type").toString() == "移动按键") {
        QList<QWidget *> widgets = this->findChildren<QWidget *>();
        foreach(QWidget *widget, widgets) {
            widget->deleteLater();
        }
        if (timelineViewTopSign + 1 < user.getTasks().size())
            timelineViewTopSign++;
    }
}

void MainWindow::paintButton(
    const QString& name,
    const QString& type,
    int x, int y, int w, int h,
    const QList<QString>& buttonTexts = {},
    const QList<QString>& formats = {},
    const QList<QRect>& labelRects = {},
    const QString& iconPath = "",
    const QString& style = "",
    std::function<void(QPushButton *)> onClick = [](QPushButton *) {},
    const QList<Qt::Alignment>& alignments = {Qt::AlignCenter}) {

    bool flag = false;
    QList<QWidget *> widgets = this->findChildren<QWidget *>();
    foreach(QWidget *widget, widgets) {
        if (widget->property("name").toString() + widget->property("type").toString() == name + type)
            flag = true;  // 找到匹配的控件
        if (widget->property("type").toString() == "创建任务")
            widget->raise();
    }

    if (!flag) {  // 如果按钮没有被找到
        QPushButton *button = new QPushButton(this);
        button->setGeometry(QRect(x, y, w, h));  // 设置按钮位置和大小
        button->setStyleSheet(style);  // 设置按钮样式
        button->setProperty("type", type);
        button->setProperty("name", name);

        // 检查图片路径是否有效
        if (!iconPath.isEmpty()) {
            QPixmap pixmap(iconPath);
            if (!pixmap.isNull()) {
                button->setIcon(pixmap);
                button->setIconSize(QSize(w, h));
            } else {
                qDebug() << "Failed to load image2!";
            }
        }

        // 连接按钮点击信号到传入的 onClick 函数
        connect(button, &QPushButton::clicked, [onClick, button]() {
            onClick(button);  // 调用 onClick 函数并传递参数
        });

        QString buttonText;
        for (int i = 0; i < buttonTexts.size(); ++i) {
            // 创建 QLabel 来显示文本
            QLabel* label = new QLabel(buttonTexts[i], button);
            label->setStyleSheet(formats[i]);
            label->setGeometry(labelRects[i]);  // 使用 QRect 设置位置和大小
            label->setAttribute(Qt::WA_TransparentForMouseEvents);
            if (i < alignments.size()) {
                label->setAlignment(alignments[i]);
            } else {
                label->setAlignment(Qt::AlignCenter);  // 默认对齐方式
            }
        }

        layout()->addWidget(button);  // 将按钮添加到布局中
    }
}

void MainWindow::paintNavigationBar() {
    QPainter painter(this);

    painter.setBrush(QColor(244, 243, 248));  // 设置矩形颜色
    painter.setPen(Qt::NoPen);  // 禁用边框
    QPainterPath rectPath;  // 创建一个路径对象用于绘制带圆角的矩形

    // 设置矩形的左上角坐标
    int x = 0;
    int y = 700;
    int width = 360;
    int height = 70;

    // 绘制带圆角的矩形
    rectPath.moveTo(x + 40, y);  // 左上角圆弧的起始点，位置在圆弧的底部中心
    rectPath.arcTo(QRectF(x, y, 80, 80), 90, 90);  // 左上角圆弧
    rectPath.lineTo(x, y + height);  // 直线到左下角
    rectPath.lineTo(x + width, y + height);  // 直线到右下角
    rectPath.lineTo(x + width, y + 40);  // 直线到右上角圆弧的起始点
    rectPath.arcTo(QRectF(x + width - 80, y, 80, 80), 0, 90);  // 右上角圆弧
    rectPath.closeSubpath();  // 关闭路径，回到起始点
    painter.fillPath(rectPath, painter.brush());  // 填充路径

    // 创建并配置按钮
    paintButton("日程", "底部导航栏",
                35, y + 20, 25, 35, {}, {}, {},
                "../../picture/schedule.png",
                "QPushButton {"
                "background-color: transparent; "
                "border: none; "
                "color: black; "
                "font-size: 16px;"
                "padding: 5px; "
                "}"
                "QPushButton:hover {"
                "background-color: rgba(0, 0, 0, 0.1); "
                "border-radius: 5px; "
                "color: blue; "
                "text-decoration: underline;"
                "}"
                "QPushButton:pressed {"
                "background-color: rgba(0, 0, 0, 0.2); "
                "}",
                [this](QPushButton * button) {
                    this->onButtonClicked(button);
                });

    paintButton("视图", "底部导航栏",
                123, y + 21, 25, 35, {}, {}, {},
                "../../picture/view.png",
                "QPushButton {"
                "background-color: transparent; "
                "border: none; "
                "color: black; "
                "font-size: 16px;"
                "padding: 5px; "
                "}"
                "QPushButton:hover {"
                "background-color: rgba(0, 0, 0, 0.1); "
                "border-radius: 5px; "
                "color: blue; "
                "text-decoration: underline;"
                "}"
                "QPushButton:pressed {"
                "background-color: rgba(0, 0, 0, 0.2); "
                "}",
                [this](QPushButton * button) {
                    this->onButtonClicked(button);
                });

    paintButton("专注", "底部导航栏",
                210, y + 21, 25, 35, {}, {}, {},
                "../../picture/focus.png",
                "QPushButton {"
                "background-color: transparent; "
                "border: none; "
                "color: black; "
                "font-size: 16px;"
                "padding: 5px; "
                "}"
                "QPushButton:hover {"
                "background-color: rgba(0, 0, 0, 0.1); "
                "border-radius: 5px; "
                "color: blue; "
                "text-decoration: underline;"
                "}"
                "QPushButton:pressed {"
                "background-color: rgba(0, 0, 0, 0.2); "
                "}",
                [this](QPushButton * button) {
                    this->onButtonClicked(button);
                });

    paintButton("我的", "底部导航栏",
                297, y + 21, 25, 35, {}, {}, {},
                "../../picture/setting.png",
                "QPushButton {"
                "background-color: transparent; "
                "border: none; "
                "color: black; "
                "font-size: 16px;"
                "padding: 5px; "
                "}"
                "QPushButton:hover {"
                "background-color: rgba(0, 0, 0, 0.1); "
                "border-radius: 5px; "
                "color: blue; "
                "text-decoration: underline;"
                "}"
                "QPushButton:pressed {"
                "background-color: rgba(0, 0, 0, 0.2); "
                "}",
                [this](QPushButton * button) {
                    this->onButtonClicked(button);
                });
}

void MainWindow::paintScheduleTaskList() {
    // 绘制任务列表白色底板
    QPainter painter(this);
    painter.setBrush(Qt::white);  // 设置填充颜色为白色
    painter.setPen(Qt::NoPen);  // 设置无边框
    painter.drawRect(0, 120, 360, 640);  // 在(0, 120)位置绘制宽360高640的矩形

    // 绘制任务列表中的任务
    QList<Task> userTasks = user.getTasks();
    if (userTasks.isEmpty()) {  // 没有任务时候展示南大校徽
        QPainter painter(this);
        QPixmap pixmap("../../picture/badge.png");
        if (!pixmap.isNull()) {
            QRect targetRect(110, 310, 140, 170);  // 在(50, 50)位置绘制，宽高为100
            painter.drawPixmap(targetRect, pixmap.scaled(targetRect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            qDebug() << "Failed to load image1!";
        }
    } else {  // 有任务时候打印任务，绘画逻辑就是，从taskTopSign[tagSelectSign]开始，画完或者画够9个为止
        int task_num = 0;
        for (int i = taskTopSign[tagSelectSign]; i < userTasks.size() && task_num < 9; i++) {
            if (tagSelectSign == 0 || userTasks[i].getTag() == user.getTags()[tagSelectSign]) {  // 如果是被选中的类型
                if (userTasks[i].getTag().getTagInfo() == "全部") {
                    paintButton(userTasks[i].getTitle() + QString::number(i), "任务",
                                20, 140 + 60 * task_num, 320, 60,
                                {userTasks[i].getDeadline().toString("yyyy-MM-dd HH:mm"), userTasks[i].getTitle(),
                                 "────────────────────────────────────────────────────────────────────────────────────────────────"},
                                {"font-size: 15px; font-weight: 200; color: black;",
                                 "font-size: 15px; font-weight: 600; color: black;",
                                 "font-size: 5px; color: black; "},
                                {QRect(0, 5, 120, 20), QRect(0, 30, 320, 20), QRect(0, 55, 320, 5)},
                                "",
                                "QPushButton {"
                                "border: none;"
                                "background-color: white;"
                                "border-radius: 5px;"
                                "font-weight: bold;"
                                "font-size: 20px;"
                                "}"
                                "QPushButton:hover {"
                                "background-color: rgba(0, 0, 0, 0.1);"
                                "}"
                                "QPushButton:pressed {"
                                "background-color: rgba(0, 0, 0, 0.2);"
                                "}",
                                [this](QPushButton * button) {
                                    this->onButtonClicked(button);
                                },
                                {Qt::AlignLeft, Qt::AlignLeft, Qt::AlignLeft});
                } else {
                    paintButton(userTasks[i].getTitle() + QString::number(i), "任务",
                                20, 140 + 60 * task_num, 320, 60,
                                {userTasks[i].getDeadline().toString("yyyy-MM-dd HH:mm"), userTasks[i].getTitle(), userTasks[i].getTag().getTagInfo(),
                                 "────────────────────────────────────────────────────────────────────────────────────────────────"},
                                {"font-size: 15px; font-weight: 200; color: black;",
                                 "font-size: 15px; font-weight: 600; color: black;",
                                 "font-size: 10px; background-color: #CBAEA0; color: white; border-radius: 5px;",
                                 "font-size: 5px; color: black; "},
                                {QRect(0, 5, 120, 20), QRect(0, 30, 320, 20), QRect(120, 5, 10 * (userTasks[i].getTag().getTagInfo().length() + 1), 20), QRect(0, 55, 320, 5)},
                                "",
                                "QPushButton {"
                                "border: none;"
                                "background-color: white;"
                                "border-radius: 5px;"
                                "font-weight: bold;"
                                "font-size: 20px;"
                                "}"
                                "QPushButton:hover {"
                                "background-color: rgba(0, 0, 0, 0.1);"
                                "}"
                                "QPushButton:pressed {"
                                "background-color: rgba(0, 0, 0, 0.2);"
                                "}",
                                [this](QPushButton * button) {
                                    this->onButtonClicked(button);
                                },
                                {Qt::AlignLeft, Qt::AlignLeft, Qt::AlignCenter, Qt::AlignLeft});
                }
                task_num++;
            }
        }
    }
    // 移动任务按钮
    paintButton("任务向上移动",  "移动按键",
                160, 120, 40, 20,
                {"∧"},  {"font-size: 20px; color: #BFBFDB;"}, {QRect(0, 0, 40, 20)},
                "",
                "QPushButton {"
                "background-color: transparent; "
                "border: none; "
                "color: black; "
                "font-size: 16px;"
                "padding: 5px; "
                "}"
                "QPushButton:hover {"
                "background-color: rgba(0, 0, 0, 0.1); "
                "border-radius: 5px; "
                "color: blue; "
                "text-decoration: underline;"
                "}"
                "QPushButton:pressed {"
                "background-color: rgba(0, 0, 0, 0.2); "
                "}",
                [this](QPushButton * button) {
                    this->onButtonClicked(button);
                });
    paintButton("任务向下移动",  "移动按键",
                160, 680, 40, 20,
                {"∨"},  {"font-size: 20px; color: #BFBFDB;"}, {QRect(0, 0, 40, 20)},
                "",
                "QPushButton {"
                "background-color: transparent; "
                "border: none; "
                "color: black; "
                "font-size: 16px;"
                "padding: 5px; "
                "}"
                "QPushButton:hover {"
                "background-color: rgba(0, 0, 0, 0.1); "
                "border-radius: 5px; "
                "color: blue; "
                "text-decoration: underline;"
                "}"
                "QPushButton:pressed {"
                "background-color: rgba(0, 0, 0, 0.2); "
                "}",
                [this](QPushButton * button) {
                    this->onButtonClicked(button);
                });
    // 绘制任务创建按钮
    paintButton("创建任务",  "创建任务",
                290, 625, 50, 50,
                {"+"}, {"font-size: 26px; color: white;"}, {QRect(0, 0, 50, 50)},
                "",
                "QPushButton {"
                "border: none;"  // 去掉边框
                "background-color: #6767AD;"  // 按钮背景颜色
                "border-radius: 25px;"  // 设置圆角半径，使按钮变为圆形
                "color: white;"  // 字体颜色
                "font-weight: bold; "  // 加粗文本
                "font-size: 20px;"  // 字体大小
                "}"
                "QPushButton:hover {"
                "background-color: rgba(0, 0, 0, 0.1); "  // 悬停时背景颜色
                "}"
                "QPushButton:pressed {"
                "background-color: rgba(0, 0, 0, 0.2); "  // 点击时背景颜色
                "}",
                [this](QPushButton * button) {
                    this->onButtonClicked(button);
                });
}

void MainWindow::paintScheduleTop() {
    // 选择日期按钮
    paintButton("选择日期",  "",
                20, 25, 85, 32,
                {"今天▾"}, {"font-size: 25px; font-weight: 200;color: black; letter-spacing: 3px;"}, {QRect(0, 0, 85, 32)},
                "",
                "QPushButton {"
                "background-color: transparent; "
                "border: none; "
                "padding: 5px; "
                "}"
                "QPushButton:hover {"
                "background-color: white; "
                "border-radius: 10px; "
                "color: blue; "
                "text-decoration: underline;"
                "}"
                "QPushButton:pressed {"
                "background-color: rgba(0, 0, 0, 0.2); "
                "}",
                [this](QPushButton * button) {
                    this->onButtonClicked(button);
                });
    // 更多功能按钮
    paintButton("更多功能",  "",
                330, 90, 25, 25,
                {}, {}, {},
                "../../picture/more.png",
                "QPushButton {"
                "background-color: transparent; "
                "border: none; "
                "color: black; "
                "font-size: 16px;"
                "padding: 5px; "
                "}"
                "QPushButton:hover {"
                "background-color: rgba(0, 0, 0, 0.1); "
                "border-radius: 5px; "
                "color: blue; "
                "text-decoration: underline;"
                "}"
                "QPushButton:pressed {"
                "background-color: rgba(0, 0, 0, 0.2); "
                "}",
                [this](QPushButton * button) {
                    this->onButtonClicked(button);
                });
    // 标签按钮
    QList<Tag> userTags = user.getTags();
    for (int i = tagLeftSign; i < userTags.size() && i < tagLeftSign + 4; i++) {
        if (i == tagSelectSign) {
            paintButton(userTags[i].getTagInfo(), "标签",
                        20 + 70 * (i-tagLeftSign), 90, 70, 30,
                        {userTags[i].getTagInfo()}, {"font-size: 15px; font-weight: 200; color: white;"}, {QRect(0, 0, 70, 30)},
                        "",
                        "QPushButton {"
                        "border: none;"
                        "background-color: #6767AD;"
                        "border-top-left-radius: 15px;"
                        "border-top-right-radius: 15px;"
                        "border-bottom-left-radius: 0;"
                        "border-bottom-right-radius: 0;"
                        "color: white;"
                        "font-weight: bold;"
                        "font-size: 20px;"
                        "}"
                        "QPushButton:hover {"
                        "background-color: rgba(0, 0, 0, 0.1);"
                        "}"
                        "QPushButton:pressed {"
                        "background-color: rgba(0, 0, 0, 0.2);"
                        "}",
                        [this](QPushButton * button) {
                            this->onButtonClicked(button);
                        });
        } else {
            paintButton(userTags[i].getTagInfo(), "标签",
                        20 + 70 * (i-tagLeftSign), 95, 70, 25,
                        {userTags[i].getTagInfo()}, {"font-size: 15px; font-weight: 200; color: white;"}, {QRect(0, 0, 70, 25)},
                        "",
                        "QPushButton {"
                        "border: none;"
                        "background-color: #BFBFDB;"
                        "border-top-left-radius: 15px;"
                        "border-top-right-radius: 15px;"
                        "border-bottom-left-radius: 0;"
                        "border-bottom-right-radius: 0;"
                        "color: white;"
                        "font-weight: bold;"
                        "font-size: 20px;"
                        "}"
                        "QPushButton:hover {"
                        "background-color: rgba(0, 0, 0, 0.1);"
                        "}"
                        "QPushButton:pressed {"
                        "background-color: rgba(0, 0, 0, 0.2);"
                        "}",
                        [this](QPushButton * button) {
                            this->onButtonClicked(button);
                        });
        }
        // 移动标签按钮
        if (userTags.size() >= 4) {
            paintButton("标签向左移动",  "移动按键",
                        0, 100, 20, 20,
                        {"<"},  {"font-size: 20px; color: #BFBFDB;"}, {QRect(0, 0, 20, 20)},
                        "",
                        "QPushButton {"
                        "background-color: transparent; "
                        "border: none; "
                        "color: black; "
                        "font-size: 16px;"
                        "padding: 5px; "
                        "}"
                        "QPushButton:hover {"
                        "background-color: rgba(0, 0, 0, 0.1); "
                        "border-radius: 5px; "
                        "color: blue; "
                        "text-decoration: underline;"
                        "}"
                        "QPushButton:pressed {"
                        "background-color: rgba(0, 0, 0, 0.2); "
                        "}",
                        [this](QPushButton * button) {
                            this->onButtonClicked(button);
                        });
            paintButton("标签向右移动",  "移动按键",
                        300, 100, 20, 20,
                        {">"},  {"font-size: 20px; color: #BFBFDB;"}, {QRect(0, 0, 20, 20)},
                        "",
                        "QPushButton {"
                        "background-color: transparent; "
                        "border: none; "
                        "color: black; "
                        "font-size: 16px;"
                        "padding: 5px; "
                        "}"
                        "QPushButton:hover {"
                        "background-color: rgba(0, 0, 0, 0.1); "
                        "border-radius: 5px; "
                        "color: blue; "
                        "text-decoration: underline;"
                        "}"
                        "QPushButton:pressed {"
                        "background-color: rgba(0, 0, 0, 0.2); "
                        "}",
                        [this](QPushButton * button) {
                            this->onButtonClicked(button);
                        });
        }
    }
}

void MainWindow::paintTimelineTaskList() {
    // 绘制任务列表白色底板，带圆角
    QPainter painter(this);
    painter.setBrush(Qt::white);  // 设置填充颜色为白色
    painter.setPen(Qt::NoPen);    // 设置无边框
    painter.drawRoundedRect(80, 120, 280, 640, 20, 20);  // 绘制带圆角的矩形

    // 绘制任务列表中的任务
    QList<Task> userTasks = user.getTasks("截止时间");
    if (userTasks.isEmpty()) {  // 没有任务时候展示南大校徽
        QPainter painter(this);
        QPixmap pixmap("../../picture/badge.png");
        if (!pixmap.isNull()) {
            QRect targetRect(150, 310, 140, 170);
            painter.drawPixmap(targetRect, pixmap.scaled(targetRect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            qDebug() << "Failed to load image1!";
        }
    } else {
        int y = 120;  // 每个标签或按钮的开始高度
        for (int i = timelineViewTopSign; i < userTasks.size() && y <= 620; i++) {  // 从当前（timelineViewTopSign）任务开始，画完或者画不下为止
            if (i == timelineViewTopSign || userTasks[i].getDeadline().date() != userTasks[i-1].getDeadline().date()) {
                QDate date = userTasks[i].getDeadline().date();
                QString month = QString::number(date.month()) + "月";
                QString day = QString::number(date.day()) + "日";
                QString dayName;
                switch (date.dayOfWeek()) {
                case 1: dayName = "(星期一)"; break;
                case 2: dayName = "(星期二)"; break;
                case 3: dayName = "(星期三)"; break;
                case 4: dayName = "(星期四)"; break;
                case 5: dayName = "(星期五)"; break;
                case 6: dayName = "(星期六)"; break;
                case 7: dayName = "(星期日)"; break;
                }
                bool flag = false;
                QList<QWidget *> widgets = this->findChildren<QWidget *>();
                foreach(QWidget *widget, widgets) {
                    if (widget->property("name").toString() == month+day+"\n"+dayName)
                        flag = true;  // 找到匹配的控件
                }
                if (!flag) {
                    QLabel *label = new QLabel(month+day+"\n"+dayName, this);
                    label->setStyleSheet("font-size: 15px; font-weight: 200; color: black");
                    label->setGeometry(QRect(10, y, 60, 40));  // 使用 QRect 设置位置和大小
                    label->setAlignment(Qt::AlignCenter);
                    label->setProperty("name", month+day+"\n"+dayName);
                    layout()->addWidget(label);
                }
                y += 20;
            }
            if (userTasks[i].getTag().getTagInfo() == "全部") {
                paintButton(userTasks[i].getTitle() + QString::number(i), "时间线视图任务",
                            100, y, 240, 60,
                            {userTasks[i].getDeadline().toString("yyyy-MM-dd HH:mm"), userTasks[i].getTitle()},
                            {"font-size: 15px; font-weight: 200; color: black;",
                             "font-size: 15px; font-weight: 600; color: black;"},
                            {QRect(0, 5, 120, 20), QRect(0, 30, 320, 20)},
                            "",
                            "QPushButton {"
                            "border: none;"
                            "background-color: white;"
                            "border-radius: 5px;"
                            "font-weight: bold;"
                            "font-size: 20px;"
                            "}"
                            "QPushButton:hover {"
                            "background-color: rgba(0, 0, 0, 0.1);"
                            "}"
                            "QPushButton:pressed {"
                            "background-color: rgba(0, 0, 0, 0.2);"
                            "}",
                            [this](QPushButton * button) {
                                this->onButtonClicked(button);
                            },
                            {Qt::AlignLeft, Qt::AlignLeft});
            } else {
                paintButton(userTasks[i].getTitle() + QString::number(i), "时间线视图任务",
                            100, y, 240, 60,
                            {userTasks[i].getDeadline().toString("yyyy-MM-dd HH:mm"), userTasks[i].getTitle(), userTasks[i].getTag().getTagInfo()},
                            {"font-size: 15px; font-weight: 200; color: black;",
                             "font-size: 15px; font-weight: 600; color: black;",
                             "font-size: 10px; background-color: #CBAEA0; color: white; border-radius: 5px;"},
                            {QRect(0, 5, 120, 20), QRect(0, 30, 320, 20), QRect(120, 5, 10 * (userTasks[i].getTag().getTagInfo().length() + 1), 20)},
                            "",
                            "QPushButton {"
                            "border: none;"
                            "background-color: white;"
                            "border-radius: 5px;"
                            "font-weight: bold;"
                            "font-size: 20px;"
                            "}"
                            "QPushButton:hover {"
                            "background-color: rgba(0, 0, 0, 0.1);"
                            "}"
                            "QPushButton:pressed {"
                            "background-color: rgba(0, 0, 0, 0.2);"
                            "}",
                            [this](QPushButton * button) {
                                this->onButtonClicked(button);
                            },
                            {Qt::AlignLeft, Qt::AlignLeft, Qt::AlignCenter});
            }
            y += 60;
        }
    }

    // 移动任务按钮
    paintButton("视图向上移动",  "移动按键",
                200, 120, 40, 20,
                {"∧"},  {"font-size: 20px; color: #BFBFDB;"}, {QRect(0, 0, 40, 20)},
                "",
                "QPushButton {"
                "background-color: transparent; "
                "border: none; "
                "color: black; "
                "font-size: 16px;"
                "padding: 5px; "
                "}"
                "QPushButton:hover {"
                "background-color: rgba(0, 0, 0, 0.1); "
                "border-radius: 5px; "
                "color: blue; "
                "text-decoration: underline;"
                "}"
                "QPushButton:pressed {"
                "background-color: rgba(0, 0, 0, 0.2); "
                "}",
                [this](QPushButton * button) {
                    this->onButtonClicked(button);
                });
    paintButton("视图向下移动",  "移动按键",
                200, 680, 40, 20,
                {"∨"},  {"font-size: 20px; color: #BFBFDB;"}, {QRect(0, 0, 40, 20)},
                "",
                "QPushButton {"
                "background-color: transparent; "
                "border: none; "
                "color: black; "
                "font-size: 16px;"
                "padding: 5px; "
                "}"
                "QPushButton:hover {"
                "background-color: rgba(0, 0, 0, 0.1); "
                "border-radius: 5px; "
                "color: blue; "
                "text-decoration: underline;"
                "}"
                "QPushButton:pressed {"
                "background-color: rgba(0, 0, 0, 0.2); "
                "}",
                [this](QPushButton * button) {
                    this->onButtonClicked(button);
                });
    // 绘制任务创建按钮
    paintButton("创建任务",  "创建任务",
                290, 625, 50, 50,
                {"+"}, {"font-size: 26px; color: white;"}, {QRect(0, 0, 50, 50)},
                "",
                "QPushButton {"
                "border: none;"  // 去掉边框
                "background-color: #6767AD;"  // 按钮背景颜色
                "border-radius: 25px;"  // 设置圆角半径，使按钮变为圆形
                "color: white;"  // 字体颜色
                "font-weight: bold; "  // 加粗文本
                "font-size: 20px;"  // 字体大小
                "}"
                "QPushButton:hover {"
                "background-color: rgba(0, 0, 0, 0.1); "  // 悬停时背景颜色
                "}"
                "QPushButton:pressed {"
                "background-color: rgba(0, 0, 0, 0.2); "  // 点击时背景颜色
                "}",
                [this](QPushButton * button) {
                    this->onButtonClicked(button);
                });
}

void MainWindow::paintTimelineTop() {
    paintButton("选择任务状态",  "",
                20, 25, 85, 32,
                {"全部▾"}, {"font-size: 25px; font-weight: 200;color: black; letter-spacing: 3px;"}, {QRect(0, 0, 85, 32)},
                "",
                "QPushButton {"
                "background-color: transparent; "
                "border: none; "
                "padding: 5px; "
                "}"
                "QPushButton:hover {"
                "background-color: white; "
                "border-radius: 10px; "
                "color: blue; "
                "text-decoration: underline;"
                "}"
                "QPushButton:pressed {"
                "background-color: rgba(0, 0, 0, 0.2); "
                "}",
                [this](QPushButton * button) {
                    this->onButtonClicked(button);
                });
    paintButton("选择视图模式",  "",
                250, 27, 100, 30,
                {"时间轴模式▾"}, {"font-size: 15px; font-weight: 300;color: black;"}, {QRect(0, 0, 100, 30)},
                "",
                "QPushButton {"
                "background-color: transparent; "
                "border: none; "
                "padding: 5px; "
                "}"
                "QPushButton:hover {"
                "background-color: white; "
                "border-radius: 10px; "
                "color: blue; "
                "text-decoration: underline;"
                "}"
                "QPushButton:pressed {"
                "background-color: rgba(0, 0, 0, 0.2); "
                "}",
                [this](QPushButton * button) {
                    this->onButtonClicked(button);
                });
    paintButton("编辑时间线视图",  "",
                180, 30, 70, 24,
                {"编辑"}, {"font-size: 15px; font-weight: 300;color: #6767AD;"}, {QRect(0, 0, 70, 24)},
                "",
                "QPushButton {"
                "background-color: transparent; "
                "border: 1px solid #6767AD; "
                "border-radius: 12px; "
                "padding: 5px; "
                "}"
                "QPushButton:hover {"
                "background-color: white; "
                "border-radius: 12px; "
                "color: blue; "
                "text-decoration: underline;"
                "}"
                "QPushButton:pressed {"
                "background-color: rgba(0, 0, 0, 0.2); "
                "}",
                [this](QPushButton * button) {
                    this->onButtonClicked(button);
                });
}

void MainWindow::paintScheduleManager() {
    paintScheduleTop();  // 初始化顶部导航栏
    paintScheduleTaskList();  // 初始化任务列表
    paintNavigationBar();  // 初始化底部导航栏
}

void MainWindow::paintTimelineView() {
    paintTimelineTop();  // 初始化顶部导航栏
    paintTimelineTaskList();  // 绘制任务列表
    paintNavigationBar();  // 初始化底部导航栏并绘制按钮
}

void MainWindow::paintEvent(QPaintEvent *event) {
    if (uiName == "日程") {
        paintScheduleManager();
    } else if (uiName == "视图") {
        paintTimelineView();
    } else if (uiName == "专注") {
        // TODO(lyp):
        paintNavigationBar();
    } else if (uiName == "我的") {
        // TODO(lyp):
        paintNavigationBar();
    }
    else assert(0);
}

