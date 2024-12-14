#include <QApplication>
#include "view.h"
#include "menutowindow.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    // 创建菜单窗口和游戏窗口
    MenuWindow menuWindow;
    View gameWindow;

    // 设置游戏窗口，以便菜单窗口可以显示它
    menuWindow.setGameWindow(&gameWindow);

    // 显示菜单窗口
    menuWindow.show();

    return app.exec();
}
