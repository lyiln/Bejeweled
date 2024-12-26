#include "gamescene.h"
#include "database.h"
#include "high_score.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QPainter>
#include <QDir>
#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene{parent}, m_game(), m_click(0), m_isSwap(false), m_isMoving(false), m_tmpScore(0)
    , m_deltaTime(0.f), m_animationTime(0.f), m_animationSpeed(20.f), m_timeLeft(9999)
    ,m_suggestCount(0)
{
    db.connectToDatabase();  // 连接到数据库
    // 连接倒计时计时器
    connect(&m_timerTime, &QTimer::timeout, this, &GameScene::updateTimer);
    m_timerTime.start(1000); // 每秒触发一次
    loadPixmap();            // 加载图片资源
    setSceneRect(0, 0, Game::RESOLUTION.width(), Game::RESOLUTION.height()); // 设置场景尺寸
    setBackgroundBrush(QBrush(QColor(153, 255, 51))); // 设置背景颜色
    init();                  // 初始化场景元素
    draw();                  // 绘制初始场景
    connect(&m_timer, &QTimer::timeout, this, &GameScene::loop); // 连接游戏循环定时器
    m_timer.start(Game::ITERATION_VALUE); // 启动游戏循环定时器
    m_elapsedTimer.start(); // 启动时间计时器
}

void GameScene::loop()
{
    m_deltaTime = m_elapsedTimer.elapsed(); // 获取自上一帧的时间差
    m_elapsedTimer.restart(); // 重置时间计时器

    m_animationTime += m_deltaTime; // 累积动画时间

    if (m_animationTime > m_animationSpeed) // 如果动画时间超过阈值
    {
        m_animationTime -= m_animationSpeed; // 重置动画时间
        mouseClick(); // 处理鼠标点击事件
        matchFinding(); // 检测匹配的宝石
        movingAnimation(); // 处理宝石移动动画
        removeAnimation(); // 处理宝石消除动画
        updateGrid(); // 更新游戏网格
        swapBackIfNotMatch(); // 恢复未匹配的交换
        draw(); // 绘制场景
        drawScore(); // 绘制分数
    }
}

void GameScene::loadPixmap()
{
    if(m_BgPixmap.load(Game::PATH_TO_BG_PIXMAP)) // 加载背景图片
    {
        qDebug() << "BgPixmap is loaded successfully"; // 输出成功信息
    }
    else
    {
        qDebug() << "BgPixmap is not loaded successfully"; // 输出失败信息
    }

    if(m_GemsPixmap.load(Game::PATH_TO_GEMS_PIXMAP)) // 加载宝石图片
    {
        qDebug() << "GemsPixmap is loaded successfully"; // 输出成功信息
    }
    else
    {
        qDebug() << "GemsPixmap is not loaded successfully"; // 输出失败信息
    }

    if(m_framePixmap.load(Game::PATH_TO_FRAME_PIXMAP)) // 加载边框图片
    {
        qDebug() << "FramePixmap is loaded successfully"; // 输出成功信息
    }
    else
    {
        qDebug() << "FramePixmap is not loaded successfully"; // 输出失败信息
    }

}

void GameScene::init()
{
    clear(); // 清除场景中的所有项
    QGraphicsPixmapItem *bgItem = new QGraphicsPixmapItem(m_BgPixmap.scaled(Game::RESOLUTION.width(), Game::RESOLUTION.height())); // 创建背景图片项
    addItem(bgItem); // 添加背景图片项到场景

    QGraphicsPixmapItem *frameItem = new QGraphicsPixmapItem(m_framePixmap); // 创建边框图片项
    frameItem->setPos(Game::OFFSET.x() - 2, Game::OFFSET.y()); // 设置边框位置
    addItem(frameItem); // 添加边框图片项到场景

    for(int i = 0; i < 10; ++i) // 遍历所有宝石位置
    {
        for(int j = 0; j < 10; ++j)
        {
            addItem(&m_pixmapItems[i][j]); // 添加宝石图片项到场景
        }
    }
    // 分数
    m_scoreText = new QGraphicsTextItem(); // 创建分数文本项
    m_scoreText->setPos(520, 80); // 设分数文本位置
    m_scoreText->setDefaultTextColor(Qt::white); // 设置文字颜色
    m_scoreText->setFont(QFont("Arial", 24)); // 设置字体
    addItem(m_scoreText); // 添加分数文本项到场景
    // 倒计时
    m_timerText = new QGraphicsTextItem(); // 创建倒计时文本项
    m_timerText->setPos(520, 50); // 设置倒计时文本位置
    m_timerText->setDefaultTextColor(Qt::white); // 设置文字颜色
    m_timerText->setFont(QFont("Arial", 24)); // 设置字体
    m_timerText->setPlainText(QString::number(m_timeLeft) + "s"); // 设置初始倒计时文本
    addItem(m_timerText); // 添加倒计时文本项到场景
}

void GameScene::setMenuWindow(QWidget *menuWindow)
{
    m_menuWindow = menuWindow; // 设置菜单窗口指针
}
//下面这个可能要删除
void GameScene::setTimeLeft(int timeLeft)
{
    // 停止当前计时器
    m_timerTime.stop();

    // 更新倒计时时间
    m_timeLeft = 30; // 设置倒计时时间为30秒

    // 重新启动计时器，每秒触发一次
    m_timerTime.start(1000);

    // 更新倒计时文本显示
    m_timerText->setPlainText(QString::number(m_timeLeft) + "s"); // 更新倒计时文本
}

void GameScene::updateTimer()
{

    if (m_timeLeft > 0) // 如果倒计时未结束
    {
        m_timeLeft--; // 倒计时减一
        m_timerText->setPlainText(QString::number(m_timeLeft-1) + "s"); // 更新倒计时文本
    }
    else // 如果倒计时结束
    {
        m_timerTime.stop(); // 停止倒计时计时器


        // 创建一个新的界面显示分数和最高分
        QWidget *scoreWindow = new QWidget(); // 创建新的窗口
        scoreWindow->setWindowTitle("Game Over"); // 设置窗口标题
        scoreWindow->resize(300, 200); // 设置窗口大小
        std::string shuying = "lose";

        QVBoxLayout *layout = new QVBoxLayout(scoreWindow); // 创建垂直布局

        //达到三十分过关，未达到三十分则失败
        if(m_game.m_score>= 30)
            shuying = "succeeds";
        QLabel *whetherwin = new QLabel(QString("You %1").arg(QString::fromStdString(shuying)), scoreWindow);
        whetherwin->setFont(QFont("Arial", 16)); // 设置字体
        layout->addWidget(whetherwin); // 添加分数标签到布局


        // 显示分数
        QLabel *scoreLabel = new QLabel(QString("Score: %1").arg(m_game.m_score), scoreWindow); // 创建分数标签
        scoreLabel->setFont(QFont("Arial", 16)); // 设置字体
        layout->addWidget(scoreLabel); // 添加分数标签到布局

        // 显示最高分
        int dataScore = db.getScore(userAccName); // 从数据库获取最高分
        if(dataScore < m_game.m_score){ // 如果当前分数高于最高分
            db.updateScore(userAccName,m_game.m_score); // 更新数据库中的最高分
        }

        QLabel *highScoreLabel = new QLabel(QString("High Score: %1").arg(dataScore), scoreWindow); // 创建最高分标签
        highScoreLabel->setFont(QFont("Arial", 16)); // 设置字体
        layout->addWidget(highScoreLabel); // 添加最高分标签到布局

        // 创建退出按钮
        QPushButton *exitButton = new QPushButton("Exit", scoreWindow); // 创建退出按钮
        layout->addWidget(exitButton); // 添加退出按钮到布局，建议直接叉掉这个窗口，不然可能崩溃

        // 连接退出按钮的点击信号到槽函数
        connect(exitButton, &QPushButton::clicked, this, [this, scoreWindow]() { // 连接退出按钮点击信号
            scoreWindow->close(); // 关闭分数窗口
            // 返回到菜单界面
            if (m_menuWindow) { // 如果菜单窗口存在
                m_menuWindow->show(); // 显示菜单窗口
            }
        });

        scoreWindow->show(); // 显示分数窗口
        m_game.m_score=0;
    }
}

void GameScene::draw()
{
    for (int i=1;i<=8;i++) // 遍历行
    {
        for (int j=1;j<=8;j++) // 遍历列
        {
            Piece p = m_game.m_grid[i][j]; // 获取当前宝石信息
            QImage image = m_GemsPixmap.copy(p.kind*49, 0, 49, 49).toImage().convertToFormat(QImage::Format_ARGB32); // 截取宝石图片
            m_pixmapItems[i][j].setPixmap(QPixmap::fromImage(image)); // 设置宝石图片
            m_pixmapItems[i][j].setPos(p.x, p.y); // 设置宝石位置
            m_pixmapItems[i][j].moveBy(Game::OFFSET.x() - Game::TILE_SIZE, Game::OFFSET.y() - Game::TILE_SIZE); // 调整宝石位置偏移
        }
    }
}

void GameScene::mouseClick()
{
    if (m_click == 1) // 如果是第一次点击
    {
        m_x0 = m_pos.x() / Game::TILE_SIZE+1; // 计算点击的行坐标
        m_y0 = m_pos.y() / Game::TILE_SIZE+1; // 计算点击的列坐标
        if(ifstart == false){ // 如果游戏未开始
            ifstart = true; // 设置游戏开始标志，之前有用，现在可能被优化了

        }
    }
    if (m_click == 2) // 如果是第二次点击
    {
        m_x = m_pos.x() / Game::TILE_SIZE+1; // 计算点击的行坐标
        m_y = m_pos.y() / Game::TILE_SIZE+1; // 计算点击的列坐标
        if ( abs(m_x-m_x0)+abs(m_y-m_y0)==1) // 如果两个点击位置相邻
        {
            m_game.swap(m_game.m_grid[m_y0][m_x0],m_game.m_grid[m_y][m_x]); // 交换两个宝石的位置
            m_isSwap = true; // 设置交换标志
            m_click = 0; // 重置点击计数
        }
        else // 如果两个点击位置不相邻
        {
            m_click = 1; // 保持点击计数为1
        }
    }
}

void GameScene::matchFinding()
{
    for(int i=1;i<=8;i++) // 遍历行
    {
        for(int j=1;j<=8;j++) // 遍历列
        {
            if (m_game.m_grid[i][j].kind == m_game.m_grid[i+1][j].kind) // 检查水平匹配
            {
                if ( m_game.m_grid[i][j].kind == m_game.m_grid[i-1][j].kind) // 检查是否三连
                {
                    for(int n=-1;n<=1;n++) // 标记匹配的宝石
                    {
                        m_game.m_grid[i+n][j].match += 1;
                    }
                }
            }

            if (m_game.m_grid[i][j].kind == m_game.m_grid[i][j+1].kind) // 检查垂直匹配
            {
                if (m_game.m_grid[i][j].kind == m_game.m_grid[i][j-1].kind) // 检查是否三连
                {
                    for(int n=-1;n<=1;n++) // 标记匹配的宝石
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
    m_isMoving = false; // 初始化移动标志
    for (int i = 1; i <= 8; i++) // 遍历行
    {
        for (int j = 1; j<=8; j++) // 遍历列
        {
            Piece &p = m_game.m_grid[i][j]; // 获取当前宝石信息
            int dx,dy; // 计算位置差
            for(int n=0;n<4;n++)   // 4 - speed
            {
                dx = p.x-p.col * Game::TILE_SIZE; // 计算x方向的位置差
                dy = p.y-p.row * Game::TILE_SIZE; // 计算y方向的位置差
                if (dx)
                {
                    p.x-=dx/abs(dx); // 移动x方向
                }
                if (dy)
                {
                    p.y-=dy/abs(dy); // 移动y方向
                }
            }
            if (dx||dy) // 如果有移动
            {
                m_isMoving = true; // 设置移动标志
            }
        }
    }
}

void GameScene::removeAnimation()
{
    if (!m_isMoving) // 如果没有移动
    {
        for (int i=1;i<=8;i++) // 遍历行
        {
            for (int j=1;j<=8;j++) // 遍历列
            {
                if (m_game.m_grid[i][j].match) // 如果宝石被匹配
                {
                    if (m_game.m_grid[i][j].alpha>10) // 如果透明度大于10
                    {
                        m_game.m_grid[i][j].alpha -= 10; // 降低透明度
                        m_isMoving = true; // 设置移动标志
                    }
                }
            }
        }
    }
}

void GameScene::swapBackIfNotMatch()
{
    if (m_isSwap && !m_isMoving) // 如果进行了交换且没有移动
    {
        if (!m_tmpScore) // 如果没有得分
        {
            m_game.swap(m_game.m_grid[m_y0][m_x0],m_game.m_grid[m_y][m_x]); // 恢复交换
        }
        m_isSwap = false; // 重置交换标志
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
            removeItem(&m_pixmapItems[i][j]); // 移除宝石图片项
        }
    }
}

void GameScene::drawScore()
{

    m_scoreText->setPlainText("score:" + QString::number(m_game.m_score)); // 设置初始倒计时文本
    addItem(m_scoreText); // 添加倒计时文本项到场景

}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if (!m_isSwap && !m_isMoving)
        {
            m_click++; // 增加点击计数
        }
        m_pos = QPoint(event->scenePos().x(), event->scenePos().y()) - Game::OFFSET; // 记录点击位置
    }
    else if(event->button() == Qt::RightButton)
    {
        m_pos1 = QPoint(event->scenePos().x(), event->scenePos().y()) - Game::OFFSET; // 记录右键点击位置
    }
}

void GameScene::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_H:{
        if (suggestSwap()) {
            m_suggestCount = 0; // 重置计数器，因为找到了可交换的方块对
        } else {
            m_suggestCount++;
            if (m_suggestCount >= 3) {
                resetGrid(); // 重新随机生成整个页面的方块
                m_suggestCount = 0; // 重置计数器
            }
        }
        break;
    }
    case Qt::Key_B:{
        boom();
        break;
    }
    break;
    }

    QGraphicsScene::keyPressEvent(event); // 处理其他键盘事件
}

void GameScene::resetGrid()
{
    // 遍历整个网格，随机生成每个方块的类型
    for (int i = 1; i <= 8; i++) {
        for (int j = 1; j <= 8; j++) {
            m_game.m_grid[i][j].kind = rand() % 7; // 随机生成宝石类型
            m_game.m_grid[i][j].match = 0;
            m_game.m_grid[i][j].alpha = 255;
            m_game.m_grid[i][j].x = j * Game::TILE_SIZE + Game::OFFSET.x() - Game::TILE_SIZE;
            m_game.m_grid[i][j].y = i * Game::TILE_SIZE + Game::OFFSET.y() - Game::TILE_SIZE;
            m_game.m_grid[i][j].col = j;
            m_game.m_grid[i][j].row = i;
        }
    }
    // 更新游戏场景
    updateGrid();
    draw();
    drawScore();
}

// 深度优先搜索
bool GameScene::dfs(int x, int y, bool visited[9][9])
{
    // 检查当前坐标是否已经被访问
    if (visited[y][x]) return false;
    visited[y][x] = true;

    int kind = m_game.m_grid[y][x].kind;

    // 水平匹配
    if (x > 1 && x < 8 && kind == m_game.m_grid[y][x-1].kind && kind == m_game.m_grid[y][x+1].kind)
    {
        return true;
    }

    // 垂直匹配
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

// 检查交换后的匹配情况
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

// 建议交换宝石对
bool GameScene::suggestSwap()
{
    // 遍历所有宝石位置
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

void GameScene::boom()
{
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
                m_game.m_grid[y][x].kind = kind; // 设置周围宝石类型为当前类型
            }
        }
    }
    m_game.m_score -= 10; // 扣除分数
}

void GameScene::refreshTimer(int newTime)
{
    m_timerTime.stop();
    m_timeLeft = newTime;
    m_timerText->setPlainText(QString::number(m_timeLeft)+"s");
    m_timerTime.start(1000);
    m_game.m_score=0;
}
