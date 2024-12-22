#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QWidget>
#include <QPushButton>

class MenuWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MenuWindow(QWidget *parent = nullptr);
    void setGameWindow(QWidget *gameWindow);

private slots:
    void onStartGameClicked();
    void onExitClicked();
    void onOptionsClicked();
private:
    QPushButton *m_startButton;
    QWidget *m_gameWindow;
};

#endif // MENUWINDOW_H
