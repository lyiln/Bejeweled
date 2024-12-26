#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <view.h>
#include <QApplication>

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
    void time_1_Clicked();
    void time_2_Clicked();
    void hideAndShow();

private:
    int timeleft2;
    QTimer m2_timerTime;

    QPushButton *m_startButton;
    QWidget *m_gameWindow;
};

#endif // MENUWINDOW_H
