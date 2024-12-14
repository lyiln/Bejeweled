#include "game.h"

const QSize Game::RESOLUTION = QSize(740, 480);
const QPoint Game::OFFSET = QPoint(48,24);
const int Game::TILE_SIZE = 54;
const QString Game::PATH_TO_BG_PIXMAP = ":/images/background.png";
const QString Game::PATH_TO_GEMS_PIXMAP = ":/images/gems.png";
const float Game::ITERATION_VALUE = 1000.0f/60.0f;
const QString Game::PATH_TO_FRAME_PIXMAP = ":/images/frame.png";
const QString Game::PATH_TO_SCORE_PIXMAP = ":/images/score.png";
const QString Game::PATH_TO_CURSOR_PIXMAP = ":/images/cursor.png";
const QString Game::PATH_TO_NUMBERS_PIXMAP = ":/images/numbers.png";

Game::Game()
    : m_score(0)
{
    initGrid();
}

void Game::swap(Piece p1,Piece p2)
{
  std::swap(p1.col,p2.col);
  std::swap(p1.row,p2.row);

  m_grid[p1.row][p1.col]=p1;
  m_grid[p2.row][p2.col]=p2;
}

void Game::initGrid()
{
    srand(time(0));
    for (int i=1;i<=8;i++)
    {
        for (int j=1;j<=8;j++)
         {
             m_grid[i][j].kind = rand()%7;
             m_grid[i][j].col = j;
             m_grid[i][j].row = i;
             m_grid[i][j].x = j * TILE_SIZE;
             m_grid[i][j].y = i * TILE_SIZE;
         }
    }
}
