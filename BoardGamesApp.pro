QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    gamecard/game.cpp \
    gamecard/gamecard.cpp \
    general/infocard.cpp \
    general/itembutton.cpp \
    general/itemlist.cpp \
    general/stylehelper.cpp \
    general/usefulfunctions.cpp \
    main.cpp \
    mainwindow.cpp \
    playercard/player.cpp \
    playercard/playercard.cpp \
    playstab/playslistwidget.cpp \
    playstab/playsminicard.cpp

HEADERS += \
    gamecard/game.h \
    gamecard/gamecard.h \
    general/infocard.h \
    general/itembutton.h \
    general/itemlist.h \
    general/stylehelper.h \
    general/usefulfunctions.h \
    mainwindow.h \
    playercard/player.h \
    playercard/playercard.h \
    playstab/playslistwidget.h \
    playstab/playsminicard.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    BoardGamesApp_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc \
    resources.qrc

DISTFILES += \
    .gitignore \
    sources/fonts/Montserrat-Medium.ttf \
    sources/fonts/Montserrat-Regular.ttf \
    todolist.txt
