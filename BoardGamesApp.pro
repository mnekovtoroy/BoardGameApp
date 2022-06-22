QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    game.cpp \
    gamecard.cpp \
    infocard.cpp \
    itembutton.cpp \
    main.cpp \
    mainwindow.cpp \
    stylehelper.cpp \
    usefulfunctions.cpp

HEADERS += \
    game.h \
    gamecard.h \
    infocard.h \
    itembutton.h \
    mainwindow.h \
    stylehelper.h \
    usefulfunctions.h

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
    resources.qrc

DISTFILES += \
    .gitignore \
    todolist.txt
