greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

HEADERS += \
    game.h \
    gamescene.h \
    menutowindow.h \
    view.h

SOURCES += \
    game.cpp \
    gamescene.cpp \
    main.cpp \
    menutowindow.cpp \
    view.cpp

RESOURCES += \
    resource.qrc
