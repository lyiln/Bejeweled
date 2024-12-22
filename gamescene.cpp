#include "gamescene.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QPainter>
#include <QDir>
#include <QDebug>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene{parent}, m_game(), m_click(0), m_isSwap(false), m_isMoving(false), m_tmpScore(0)
    , m_deltaTime(0.f), m_animationTime(0.f), m_animationSpeed(20.f), m_timeLeft(60) // 初始化倒计时为60秒
{
    // 连接倒计时计时器
    connect(&m_timerTime, &QTimer::timeout, this, &GameScene::updateTimer);
    m_timerTime.start(1000); // 每秒触发一次

    loadPixmap();
    setSceneRect(0, 0, Game::RESOLUTION.width(), Game::RESOLUTION.height());
    setBackgroundBrush(QBrush(QColor(153, 255, 51)));
    init();
    draw();
    connect(&m_timer, &QTimer::timeout, this, &GameScene::loop);
    m_timer.start(Game::ITERATION_VALUE);
    m_elapsedTimer.start();

}

void GameScene::loop()
{
    if (m_timeLeft <= 0) {
        // 游戏已结束，不再进行游戏循环
        return;
    }
    m_deltaTime = m_elapsedTimer.elapsed();
    m_elapsedTimer.restart();

    m_animationTime += m_deltaTime;

    if (m_animationTime > m_animationSpeed)
    {
        m_animationTime -= m_animationSpeed;
        mouseClick();
        matchFinding();
        movingAnimation();
        removeAnimation();
        swapBackIfNotMatch();
        updateGrid();
        draw();
        drawScore();
    }
}

void GameScene::loadPixmap()
{
    if(m_BgPixmap.load(Game::PATH_TO_BG_PIXMAP))
    {
        qDebug() << "BgPixmap is loaded successfully";
    }
    else
    {
        qDebug() << "BgPixmap is not loaded successfully";
    }

    if(m_GemsPixmap.load(Game::PATH_TO_GEMS_PIXMAP))
    {
        qDebug() << "GemsPixmap is loaded successfully";
    }
    else
    {
        qDebug() << "GemsPixmap is not loaded successfully";
    }

    if(m_framePixmap.load(Game::PATH_TO_FRAME_PIXMAP))
    {
        qDebug() << "FramePixmap is loaded successfully";
    }
    else
    {
        qDebug() << "FramePixmap is not loaded successfully";
    }

    if(m_scorePixmap.load(Game::PATH_TO_SCORE_PIXMAP))
    {
        qDebug() << "ScorePixmap is loaded successfully";
    }
    else
    {
        qDebug() << "ScorePixmap is not loaded successfully";
    }

    if(m_numbersPixmap.load(Game::PATH_TO_NUMBERS_PIXMAP))
    {
        qDebug() << "NumbersPixmap is loaded successfully";
    }
    else
    {
        qDebug() << "NumbersPixmap is not loaded successfully";
    }
}

void GameScene::init()
{
    clear();
    QGraphicsPixmapItem *bgItem = new QGraphicsPixmapItem(m_BgPixmap.scaled(Game::RESOLUTION.width(), Game::RESOLUTION.height()));
    addItem(bgItem);

    QGraphicsPixmapItem *frameItem = new QGraphicsPixmapItem(m_framePixmap);
    frameItem->setPos(Game::OFFSET.x() - 2, Game::OFFSET.y());
    addItem(frameItem);

    for(int i = 0; i < 10; ++i)
    {
        for(int j = 0; j < 10; ++j)
        {
            addItem(&m_pixmapItems[i][j]);
        }
    }

    QGraphicsPixmapItem *scoreItem = new QGraphicsPixmapItem(m_scorePixmap);
    scoreItem->setPos(Game::RESOLUTION.width() - scoreItem->boundingRect().width() - 85, 100);
    addItem(scoreItem);

    unityPartScoreItem.moveBy(Game::RESOLUTION.width()-32, 100);
    addItem(&unityPartScoreItem);

    decimalPartScoreItem.moveBy(Game::RESOLUTION.width()-2*32, 100);
    addItem(&decimalPartScoreItem);

    hundredthPartScoreItem.moveBy(Game::RESOLUTION.width()-3*32, 100);
    addItem(&hundredthPartScoreItem);
    // 添加倒计时文本显示
    m_timerText = new QGraphicsTextItem();
    m_timerText->setPos(565, 50); // 设置位置
    m_timerText->setDefaultTextColor(Qt::white); // 设置文字颜色
    m_timerText->setFont(QFont("Arial", 24)); // 设置字体
    m_timerText->setPlainText(QString::number(m_timeLeft) + "s");
    addItem(m_timerText);

}

void GameScene::updateTimer()
{
    if (m_timeLeft > 0) {
        m_timeLeft--;
        m_timerText->setPlainText(QString::number(m_timeLeft) + "s");
    } else {
        m_timerTime.stop();
        // 显示游戏结束信息
        QGraphicsTextItem *gameOverText = new QGraphicsTextItem();
        gameOverText->setPos(Game::RESOLUTION.width()/2 - 100, Game::RESOLUTION.height()/2 - 50);
        gameOverText->setDefaultTextColor(Qt::red);
        gameOverText->setFont(QFont("Arial", 48));
        gameOverText->setPlainText("Game Over!");
        addItem(gameOverText);

        // 停止游戏循环
        m_timer.stop();
    }
}


void GameScene::draw()
{
    for (int i=1;i<=8;i++)
    {
        for (int j=1;j<=8;j++)
        {
            Piece p = m_game.m_grid[i][j];
            QImage image = m_GemsPixmap.copy(p.kind*49, 0, 49, 49).toImage().convertToFormat(QImage::Format_ARGB32);
//            for (int x(0); x != image.width(); ++x)
//            {
//                for (int y(0); y != image.height(); ++y)
//                {
//                    QColor color(image.pixel(x,y));
//                    color.setAlpha(p.alpha);
//                    image.setPixel(x, y, color.rgba());
//                }
//            }
            m_pixmapItems[i][j].setPixmap(QPixmap::fromImage(image));
            m_pixmapItems[i][j].setPos(p.x, p.y);
            m_pixmapItems[i][j].moveBy(Game::OFFSET.x() - Game::TILE_SIZE, Game::OFFSET.y() - Game::TILE_SIZE);

        }
    }
}

void GameScene::mouseClick()
{
    if (m_click == 1)
    {
        m_x0 = m_pos.x() / Game::TILE_SIZE+1;
        m_y0 = m_pos.y() / Game::TILE_SIZE+1;
    }
    if (m_click == 2)
    {
        m_x = m_pos.x() / Game::TILE_SIZE+1;
        m_y = m_pos.y() / Game::TILE_SIZE+1;
        if ( abs(m_x-m_x0)+abs(m_y-m_y0)==1)
        {
            m_game.swap(m_game.m_grid[m_y0][m_x0],m_game.m_grid[m_y][m_x]);
            m_isSwap = true;
            m_click = 0;
        }
        else
        {
            m_click = 1;
        }
    }
}

void GameScene::matchFinding()
{
    for(int i=1;i<=8;i++)
    {
        for(int j=1;j<=8;j++)
        {
            if (m_game.m_grid[i][j].kind == m_game.m_grid[i+1][j].kind)
            {
                if ( m_game.m_grid[i][j].kind == m_game.m_grid[i-1][j].kind)
                {
                    for(int n=-1;n<=1;n++)
                    {
                        m_game.m_grid[i+n][j].match += 1;
                    }
                }

            }


            if (m_game.m_grid[i][j].kind == m_game.m_grid[i][j+1].kind)
            {
                if (m_game.m_grid[i][j].kind == m_game.m_grid[i][j-1].kind)
                {
                    for(int n=-1;n<=1;n++)
                    {
                        m_game.m_grid[i][j+n].match += 1;
                    }
                }

            }
        }
    }
}

void GameScene::movingAnimation()
{
    m_isMoving = false;
    for (int i = 1; i <= 8; i++)
    {
        for (int j = 1; j<=8; j++)
        {
            Piece &p = m_game.m_grid[i][j];
            int dx,dy;
            for(int n=0;n<4;n++)   // 4 - speed
            {
                dx = p.x-p.col * Game::TILE_SIZE;
                dy = p.y-p.row * Game::TILE_SIZE;
                if (dx)
                {
                    p.x-=dx/abs(dx);
                }
                if (dy)
                {
                    p.y-=dy/abs(dy);
                }
            }
            if (dx||dy)
            {
                m_isMoving = true;
            }
        }
    }
}

void GameScene::removeAnimation()
{
    if (!m_isMoving)
    {
        for (int i=1;i<=8;i++)
        {
            for (int j=1;j<=8;j++)
            {
                if (m_game.m_grid[i][j].match)
                {
                    if (m_game.m_grid[i][j].alpha>10)
                    {
                        m_game.m_grid[i][j].alpha -= 10;
                        m_isMoving = true;
                    }
                }
            }
        }
    }

}



void GameScene::swapBackIfNotMatch()
{
    if (m_isSwap && !m_isMoving)
    {
        if (!m_tmpScore)
        {
            m_game.swap(m_game.m_grid[m_y0][m_x0],m_game.m_grid[m_y][m_x]);
        }
        m_isSwap = false;
    }

}

void GameScene::updateGrid()
{
    // 如果没有方块在移动
    if (!m_isMoving)
    {
        int removedPieces = 0; // 统计被消除的方块数量

        // 遍历游戏网格中的每个方块
        for (int i = 8; i > 0; i--)
        {
            for (int j = 1; j <= 8; j++)
            {
                // 如果方块匹配
                if (m_game.m_grid[i][j].match)
                {
                    // 查找上方未匹配的方块
                    for (int n = i; n > 0; n--)
                    {
                        if (!m_game.m_grid[n][j].match)
                        {
                            // 交换匹配的方块和未匹配的方块
                            m_game.swap(m_game.m_grid[n][j], m_game.m_grid[i][j]);
                            break;
                        }
                    }
                }
            }
        }

        // 遍历游戏网格中的每个方块
        for (int j = 1; j <= 8; j++)
        {
            for (int i = 8, n = 0; i > 0; i--)
            {
                // 如果方块匹配
                if (m_game.m_grid[i][j].match)
                {
                    // 随机生成新的方块类型
                    m_game.m_grid[i][j].kind = rand() % 7;

                    // 设置方块的垂直位置
                    m_game.m_grid[i][j].y = -Game::TILE_SIZE * n++;

                    // 重置方块的匹配标志和透明度
                    m_game.m_grid[i][j].match = 0;
                    m_game.m_grid[i][j].alpha = 255;

                    removedPieces++; // 统计被消除的方块数量
                }
            }
        }

        // 增加分数，匹配消除几个方块就加几分
        m_game.m_score += removedPieces;
        if(removedPieces > 0){
            m_tmpScore = 1;
        }else m_tmpScore =0;
    }
}


void GameScene::removePixmapItems()
{
    for(int i = 0; i < 10; ++i)
    {
        for(int j =0; j < 10; ++j)
        {
            removeItem(&m_pixmapItems[i][j]);
        }
    }
}

void GameScene::drawScore()
{
    QString scoreText = QString::number(m_game.m_score);
    int unityPartVal = 0;
    int decimalPartValue = 0;
    int hendredthPartValue = 0;

    if(scoreText.length() == 1) // 0 - 9
    {
        unityPartVal = scoreText.toInt();
        decimalPartValue = 0;
        hendredthPartValue = 0;
    }
    else if(scoreText.length() == 2) // 10 - 99
    {
        unityPartVal = scoreText.last(1).toInt();
        decimalPartValue = scoreText.first(1).toInt();
        hendredthPartValue = 0;
    }
    else if(scoreText.length() == 3) // 100 - 999
    {
        unityPartVal = scoreText.last(1).toInt();
        hendredthPartValue = scoreText.first(1).toInt();
        QString copyVal = scoreText;
        copyVal.chop(1);
        decimalPartValue = copyVal.last(1).toInt();
    }

    unityPartScoreItem.setPixmap(m_numbersPixmap.copy(unityPartVal*32, 0, 32, 32));
    decimalPartScoreItem.setPixmap(m_numbersPixmap.copy(decimalPartValue*32, 0, 32, 32));
    hundredthPartScoreItem.setPixmap(m_numbersPixmap.copy(hendredthPartValue*32, 0, 32, 32));
}

void GameScene::renderScene()
{
    static int index = 0;
    QString fileName = QDir::currentPath() + QDir::separator() + "screen" + QString::number(index++) + ".png";
    QRect rect = sceneRect().toAlignedRect();
    QImage image(rect.size(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    render(&painter);
    image.save(fileName);
    qDebug() << "saved " << fileName;
}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if (!m_isSwap && !m_isMoving)
        {
            m_click++;
        }
        m_pos = QPoint(event->scenePos().x(), event->scenePos().y()) - Game::OFFSET;
    }
    else if(event->button() == Qt::RightButton)
    {
        m_pos1 = QPoint(event->scenePos().x(), event->scenePos().y()) - Game::OFFSET;
    }
}

void GameScene::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Z:{
        //renderScene();
        break;
    }
    case Qt::Key_H:{
        suggestSwap();
        break;
    }
    case Qt::Key_B:{
        boom();
        break;
    }
    break;
    }

    QGraphicsScene::keyPressEvent(event);
}


// 深度优先
// DFS
bool GameScene::dfs(int x, int y, bool visited[9][9])
{
    // 检查当前坐标是否已经被访问
    if (visited[y][x]) return false;
    visited[y][x] = true;

    int kind = m_game.m_grid[y][x].kind;

    // 水平
    if (x > 1 && x < 8 && kind == m_game.m_grid[y][x-1].kind && kind == m_game.m_grid[y][x+1].kind)
    {
        return true;
    }

    // 垂直
    if (y > 1 && y < 8 && kind == m_game.m_grid[y-1][x].kind && kind == m_game.m_grid[y+1][x].kind)
    {
        return true;
    }

    // 递归检查周围相邻的宝石
    if (x < 8 && dfs(x + 1, y, visited)) return true;
    if (y < 8 && dfs(x, y + 1, visited)) return true;
    if (x > 1 && dfs(x - 1, y, visited)) return true;
    if (y > 1 && dfs(x, y - 1, visited)) return true;

    return false;
}

// 检查三连
bool GameScene::checkMatchAfterSwap(int x1, int y1, int x2, int y2)
{
    // 交换宝石
    std::swap(m_game.m_grid[y1][x1].kind, m_game.m_grid[y2][x2].kind);

    bool visited[9][9] = {{false}}; // 用于记录访问的宝石位置
    bool matchFound = false;

    if (dfs(x1, y1, visited) || dfs(x2, y2, visited)) {
        matchFound = true;
    }

    // 恢复交换
    std::swap(m_game.m_grid[y1][x1].kind, m_game.m_grid[y2][x2].kind);

    if (matchFound == true){
        std::swap(m_game.m_grid[y1][x1].kind, m_game.m_grid[y2][x2].kind);
    }

    return matchFound;
}

// 穷举法
bool GameScene::suggestSwap()
{
    // 遍历
    for (int i = 1; i <= 8; ++i)
    {
        for (int j = 1; j <= 8; ++j)
        {
            // 检查右边和下边的宝石
            if (i < 8 && checkMatchAfterSwap(i, j, i + 1, j))
            {
                m_game.m_score -=3;
                return true;
            }
            if (j < 8 && checkMatchAfterSwap(i, j, i, j + 1))
            {
                m_game.m_score -=3;
                return true;
            }
        }
    }
    return false;  // 没有找到可以交换的宝石对
}



void GameScene::boom() {
    m_click = 0;
    m_x0 = m_pos1.x() / Game::TILE_SIZE + 1;  // 计算点击的行坐标
    m_y0 = m_pos1.y() / Game::TILE_SIZE + 1;  // 计算点击的列坐标

    // 获取当前点击位置的宝石类型
    int kind = m_game.m_grid[m_y0][m_x0].kind;

    // 遍历周围方向
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;

            int x = m_x0 + dx;
            int y = m_y0 + dy;

            // 检查越界
            if (x >= 1 && x <= 8 && y >= 1 && y <= 8) {
                m_game.m_grid[y][x].kind = kind;
            }
        }
    }
    m_game.m_score -= 10;
}

