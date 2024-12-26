#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QElapsedTimer>
#include <qpushbutton.h>

#include "game.h"
#include "database.h"

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GameScene(QObject *parent = nullptr);
    int m_tmpScore;
    Game m_game;
    void setMenuWindow(QWidget *menuWindow);
    void setTimeLeft(int timeLeft);
    void refreshTimer(int newTime);


signals:
private slots:
    void loop();
    void updateTimer();

private:

    QWidget *m_menuWindow;
    Database db;  // 声明数据库对象

    int m_suggestCount;
    int m_timeLeft; // 倒计时剩余时间（秒）
    QTimer m_timerTime; // 倒计时计时器
    QGraphicsTextItem *m_timerText; // 倒计时文本显示
    QGraphicsTextItem *m_scoreText;
    bool ifstart = false;

    std::string shuying;

    void loadPixmap();
    void init();
    void draw();
    void mouseClick();
    void matchFinding();
    void movingAnimation();
    void removeAnimation();
    void swapBackIfNotMatch();
    void updateGrid();
    void removePixmapItems();
    void drawScore();
    void resetGrid();
    void boom();
    bool suggestSwap();
    bool checkMatchAfterSwap(int x1, int y1, int x2, int y2);
    bool dfs(int x, int y, bool visited[9][9]);


    QPixmap m_BgPixmap;
    QPixmap m_GemsPixmap;
    QPixmap m_framePixmap;

    int m_x0, m_y0, m_x, m_y;
    int m_click;
    QPoint m_pos, m_pos1;
    bool m_isSwap, m_isMoving;
    QGraphicsPixmapItem m_pixmapItems[10][10];
    QGraphicsPixmapItem unityPartScoreItem, decimalPartScoreItem, hundredthPartScoreItem;


    QElapsedTimer m_elapsedTimer;
    QTimer m_timer;
    float m_deltaTime;
    float m_animationTime;
    const float m_animationSpeed;
    QGraphicsPixmapItem m_button1, m_button2;

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // GAMESCENE_H
