#include "view.h"
#include <QKeyEvent>
#include <QApplication>

View::View()
    : QGraphicsView(), m_gameScene(new GameScene(this))
{
    setScene(m_gameScene);//创建场景，使用scene
    resize(m_gameScene->sceneRect().width() + 2, m_gameScene->sceneRect().height() + 2);
    setCursor(QCursor(QPixmap(":/images/cursor.png")));//光标
}

void View::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_Escape:
        {
            QApplication::instance()->quit();
        }
        break;
    }
    QGraphicsView::keyPressEvent(event);
}
