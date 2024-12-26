#include <QApplication>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>

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

    // 创建QMediaPlayer对象来播放音乐
    QMediaPlayer* musicPlayer = new QMediaPlayer;
    QAudioOutput* audioOutput = new QAudioOutput;

    // 设置音频输出
    musicPlayer->setAudioOutput(audioOutput);

    // 设置音量（可以根据需要调整）
    audioOutput->setVolume(1);

    QFile file("C:\\Users\\Dell\\Desktop\\Bejeweled\\music\\musicone.mp3");
    if (!file.exists()) {
        qDebug() << "File does not exist!";
    }

    // 设置音乐文件路径（替换为你自己的音乐文件路径）
    musicPlayer->setSource(QUrl::fromLocalFile("C:\\Users\\Dell\\Desktop\\Bejeweled\\music\\musicone.mp3"));

    // 连接播放结束信号到重新播放的槽
    QObject::connect(musicPlayer, &QMediaPlayer::mediaStatusChanged, [&]() {
        if (musicPlayer->mediaStatus() == QMediaPlayer::EndOfMedia) {
            musicPlayer->setPosition(0);  // 播放完毕后重置到起始位置
            musicPlayer->play();  // 重新播放音乐
        }
    });

    // 连接错误发生信号，检查播放失败的情况
    QObject::connect(musicPlayer, &QMediaPlayer::errorOccurred, [&]() {
        qDebug() << "Error occurred while playing the music!";
        qDebug() << "Error string: " << musicPlayer->errorString();
    });

    // 开始播放音乐
    musicPlayer->play();

    // 检查播放是否成功
    if (musicPlayer->mediaStatus() == QMediaPlayer::LoadedMedia) {
        qDebug() << "Music loaded successfully and started playing!";
    } else if (musicPlayer->mediaStatus() == QMediaPlayer::InvalidMedia) {
        qDebug() << "Invalid media or error loading music!";
        qDebug() << "Error string: " << musicPlayer->errorString();
    }

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
