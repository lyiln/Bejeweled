#ifndef GAME_H
#define GAME_H

#include <QSize>
#include <QPoint>

struct Piece
{
    int x,y,col,row,kind,match,alpha;
    Piece()
    {
        match=0;
        alpha=255;
    }
};

class Game
{
public:
    Game();
    void swap(Piece p1, Piece p2);
    void initGrid();
    static const QSize RESOLUTION;
    static const QPoint OFFSET;
    static const int TILE_SIZE;
    static const QString PATH_TO_BG_PIXMAP;
    static const QString PATH_TO_GEMS_PIXMAP;
    static const QString PATH_TO_FRAME_PIXMAP;
    static const QString PATH_TO_SCORE_PIXMAP;
    static const QString PATH_TO_CURSOR_PIXMAP;
    static const QString PATH_TO_NUMBERS_PIXMAP;
    static const float ITERATION_VALUE;
    Piece m_grid[10][10];
    int m_score;
};

#endif // GAME_H
