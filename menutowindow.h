#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <view.h>
class GameScene;
class View;
class MenuWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MenuWindow(QWidget *parent = nullptr);
    void setGameWindow(QWidget *gameWindow);
    void callRefreshTimer(int customTime);
    GameScene* getGameScene(); // 添加函数声明
private slots:
    void onStartGameClicked();
    void onExitClicked();
    void time_2_Clicked();
private:
    QPushButton *m_startButton;
    QWidget *m_gameWindow;
};

#endif // MENUWINDOW_H
