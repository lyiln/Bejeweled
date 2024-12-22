greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += multimedia
QT += sql
CONFIG += c++11

HEADERS += \
    game.h \
    gamescene.h \
    menutowindow.h \
    startscene.h \
    view.h

SOURCES += \
    game.cpp \
    gamescene.cpp \
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
    images/退出.png
