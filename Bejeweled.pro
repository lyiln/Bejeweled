greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += multimedia
QT += sql
CONFIG += c++11

HEADERS += \
    database.h \
    game.h \
    gamescene.h \
    high_score.h \
    menutowindow.h \
    startscene.h \
    view.h

SOURCES += \
    database.cpp \
    game.cpp \
    gamescene.cpp \
    high_score.cpp \
    main.cpp \
    menutowindow.cpp \
    startscene.cpp \
    view.cpp

RESOURCES += \
    resource.qrc

DISTFILES += \
    images/account.png \
    images/loginBackground.png \
    images/password.png \
    images/xianShiModel1.png \
    images/xianShiModel2.png \
    images/退出.png \
    music/musicone.mp3
