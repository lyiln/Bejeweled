#include "menutowindow.h"
#include <QPixmap>
#include "gamescene.h"

MenuWindow::MenuWindow(QWidget *parent)
    : QWidget(parent), m_gameWindow(nullptr)
{

    GameScene *gameScene = new GameScene();
    gameScene->setMenuWindow(this); // 将菜单界面的指针传递给 GameScene

    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QPixmap(":/images/background.png")));
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


    //限时模式一
    QPushButton *timeButton1 = new QPushButton(this);
    timeButton1->setGeometry(280, 190, 149, 61);
    QPixmap t_pixmap1(":/images/xianShiModel1.png");
    timeButton1->setIcon(t_pixmap1);
    timeButton1->setIconSize(t_pixmap1.size());



    //限时模式二
    QPushButton *timeButton2 = new QPushButton(this);
    timeButton2->setGeometry(280, 260, 149, 57);
    QPixmap t_pixmap2(":/images/xianShiModel2.png");
    timeButton2->setIcon(t_pixmap2);
    timeButton2->setIconSize(t_pixmap2.size());

    // 创建退出按钮并设置位置和大小
    QPushButton *exitButton = new QPushButton(this);
    exitButton->setGeometry(280, 330, 149, 57);
    QPixmap e_pixmap(":/images/exit.png");
    exitButton->setIcon(e_pixmap);
    exitButton->setIconSize(e_pixmap.size());

    // 连接选项按钮的点击信号到相应的槽函数
    connect(timeButton2, &QPushButton::clicked, this, &MenuWindow::time_2_Clicked);

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
        //增加时间设置
        // 获取 GameScene 对象
        GameScene *gameScene = qobject_cast<GameScene*>(m_gameWindow);
        if (gameScene) {
            // 设置倒计时时间（例如 60 秒）
            gameScene->setTimeLeft(9999);
        }

        m_gameWindow->show();
    }
}
void MenuWindow::time_2_Clicked()
{
    if (m_gameWindow) {
        this->hide();
        //增加时间设置
        // 获取 GameScene 对象
        GameScene *gameScene = qobject_cast<GameScene*>(m_gameWindow);
        if (gameScene) {
            // 设置倒计时时间（例如 60 秒）
            gameScene->setTimeLeft(60);
        }

        m_gameWindow->show();
    }
}

void MenuWindow::onExitClicked()
{
    // 处理退出按钮点击事件
    // 退出应用程序
}
