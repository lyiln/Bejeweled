#include <QApplication>
#include "view.h"
#include "menutowindow.h"
#include "startscene.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    // 创建开始界面窗口，菜单窗口和游戏窗口
    startscene startScene;
    MenuWindow menuWindow;
    View gameWindow;

    // 连接 startscene 的信号到 menuWindow 的显示槽
    QObject::connect(&startScene, &startscene::showMenuWindow, [&]() {
        menuWindow.show();  // 显示菜单窗口
    });


    // 设置游戏窗口，以便菜单窗口可以显示它
    menuWindow.setGameWindow(&gameWindow);

    // 显示菜单窗口
    startScene.show();

    return app.exec();
}
