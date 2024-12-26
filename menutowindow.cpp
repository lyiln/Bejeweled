#include "menutowindow.h"
#include <QPixmap>
#include "gamescene.h"

MenuWindow::MenuWindow(QWidget *parent)
    : QWidget(parent), m_gameWindow(nullptr)
{
    connect(&m2_timerTime, &QTimer::timeout, this, &MenuWindow::hideAndShow);//内置计时器以现时切换界面
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
     QPixmap pixmap(":/images/puTongModel.png"); // 设置普通模式按钮图片
    //!!!!

    m_startButton->setIcon(pixmap);
    m_startButton->setIconSize(pixmap.size());

    // 连接信号和槽
    connect(m_startButton, &QPushButton::clicked, this, &MenuWindow::onStartGameClicked);

    //限时模式一
    QPushButton *timeButton1 = new QPushButton(this);
    timeButton1->setGeometry(280, 190, 149, 61);
    QPixmap t_pixmap1(":/images/xianShiModel1.png");// 设置现时1模式按钮图片
    timeButton1->setIcon(t_pixmap1);
    timeButton1->setIconSize(t_pixmap1.size());

    //限时模式二
    QPushButton *timeButton2 = new QPushButton(this);
    timeButton2->setGeometry(280, 260, 149, 57);
    QPixmap t_pixmap2(":/images/xianShiModel2.png");// 设置现时2模式按钮图片
    timeButton2->setIcon(t_pixmap2);
    timeButton2->setIconSize(t_pixmap2.size());

    // 创建退出按钮并设置位置和大小
    QPushButton *exitButton = new QPushButton(this);
    exitButton->setGeometry(280, 330, 149, 57);
    QPixmap e_pixmap(":/images/exit.png");// 设置退出模式按钮图片
    exitButton->setIcon(e_pixmap);
    exitButton->setIconSize(e_pixmap.size());

    // 连接选项按钮的点击信号到相应的槽函数
    connect(timeButton1, &QPushButton::clicked, this, &MenuWindow::time_1_Clicked);

    // 连接选项按钮的点击信号到相应的槽函数
    connect(timeButton2, &QPushButton::clicked, this, &MenuWindow::time_2_Clicked);

    // 连接退出按钮的点击信号到相应的槽函数
    connect(exitButton, &QPushButton::clicked, this, &MenuWindow::onExitClicked);
}
//获取view中的gamescene
GameScene* MenuWindow::getGameScene()
{
    View* view = qobject_cast<View*>(m_gameWindow);
    if(view)
    {
        return view->m_gameScene;
    }
    return nullptr;
}
//重新设置gamescene中的计时器时长
void MenuWindow::callRefreshTimer(int customTime)
{
    GameScene *gameScene = getGameScene();
    if(gameScene)
    {
        gameScene->refreshTimer(customTime);
    }
}

void MenuWindow::setGameWindow(QWidget *gameWindow)
{
    m_gameWindow = gameWindow;
}
//启动普通模式
void MenuWindow::onStartGameClicked()
{
    if (m_gameWindow) {
        this->hide();
        callRefreshTimer(9999); // 假设传入9999秒

        m_gameWindow->show();
        timeleft2=9999 ;
        m2_timerTime.start(1000); // 每秒触发一次
    }
}
//限时1
void MenuWindow::time_1_Clicked()
{
    if (m_gameWindow) {
        this->hide();
        callRefreshTimer(61); // 假设传入60秒

        m_gameWindow->show();
        timeleft2=60 ;
        m2_timerTime.start(1000); // 每秒触发一次
    }
}
//限时2
void MenuWindow::time_2_Clicked()
{
    if (m_gameWindow) {
        this->hide();
        callRefreshTimer(31); // 假设传入30秒

        m_gameWindow->show();
        timeleft2=30 ;
        m2_timerTime.start(1000); // 每秒触发一次
    }
}
//由菜单界面退出游戏
void MenuWindow::onExitClicked()
{
    // 处理退出按钮点击事件
    // 退出应用程序
    QApplication::quit();
}
void MenuWindow::hideAndShow()
{
    //内置计时器以在到时间时直接切换界面
    if (timeleft2 > 0) // 如果倒计时未结束
    {
        timeleft2--; // 倒计时减一
    }
    else
    {
         this->show();
        m_gameWindow->hide();
    }
}
