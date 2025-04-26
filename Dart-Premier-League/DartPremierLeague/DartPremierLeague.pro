QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    audiencegraphicsview.cpp \
    createplayer.cpp \
    dartboard.cpp \
    dartgame.cpp \
    darttable.cpp \
    dartthrow.cpp \
    leg.cpp \
    main.cpp \
    menuscreen.cpp \
    playerparser.cpp \
    playersavemanager.cpp \
    scorerwindow.cpp

HEADERS += \
    audiencegraphicsview.h \
    createplayer.h \
    dartboard.h \
    dartgame.h \
    darttable.h \
    dartthrow.h \
    gamesavemanager.h \
    leg.h \
    mainmanager.h \
    match.h \
    menuscreen.h \
    player.h \
    playerparser.h \
    playersavemanager.h \
    scorerwindow.h

FORMS += \
    ScorerWindow.ui \
    audiencegraphicsview.ui \
    createplayer.ui \
    menuscreen.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES += \
    player_stats_csv.csv
