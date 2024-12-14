#include "menutowindow.h"
#include <QPixmap>

MenuWindow::MenuWindow(QWidget *parent)
    : QWidget(parent), m_gameWindow(nullptr)
{

    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/images/04.png")));
    this->setPalette(palette);

    //设置窗口大小
    this->resize(740, 480);
    // 创建按钮并设置位置和大小
    m_startButton = new QPushButton(this);
    m_startButton->setGeometry(280, 120, 149, 57);

    // 加载图片并设置为按钮的图标
     QPixmap pixmap(":/images/puTongModel.png"); // 替换为您的图片路径
    //!!!!

    m_startButton->setIcon(pixmap);
    m_startButton->setIconSize(pixmap.size()); // 可以根据需要调整图标大小

    // 连接信号和槽
    connect(m_startButton, &QPushButton::clicked, this, &MenuWindow::onStartGameClicked);


    //限时模式
    QPushButton *timeButton = new QPushButton(this);
    timeButton->setGeometry(280, 190, 149, 61);
    QPixmap t_pixmap(":/images/xianShiModel.png");
    timeButton->setIcon(t_pixmap);
    timeButton->setIconSize(t_pixmap.size());



    // 创建选项按钮并设置位置和大小
    QPushButton *optionsButton = new QPushButton("选项", this);
    optionsButton->setGeometry(280, 260, 149, 57);

    // 创建退出按钮并设置位置和大小
    QPushButton *exitButton = new QPushButton("退出", this);
    exitButton->setGeometry(280, 330, 149, 57);

    // 连接选项按钮的点击信号到相应的槽函数
    connect(optionsButton, &QPushButton::clicked, this, &MenuWindow::onOptionsClicked);

    // 连接退出按钮的点击信号到相应的槽函数
    connect(exitButton, &QPushButton::clicked, this, &MenuWindow::onExitClicked);


}

void MenuWindow::setGameWindow(QWidget *gameWindow)
{
    m_gameWindow = gameWindow;
}

void MenuWindow::onStartGameClicked()
{
    if (m_gameWindow) {
        this->hide();
        m_gameWindow->show();
    }
}
void MenuWindow::onOptionsClicked()
{
    // 处理选项按钮点击事件
}

void MenuWindow::onExitClicked()
{
    // 处理退出按钮点击事件
    // 退出应用程序
}
