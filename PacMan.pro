QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Blinky.cpp \
    Clyde.cpp \
    Energizer.cpp \
    Ghost.cpp \
    Inky.cpp \
    Pellet.cpp \
    Pinky.cpp \
    Player.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Blinky.h \
    Clyde.h \
    Energizer.h \
    Ghost.h \
    Inky.h \
    Pellet.h \
    Pinky.h \
    Player.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    PacMan_en_GB.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../Desktop/Map.png \
    ../Desktop/Originalpacmaze.webp \
    ../Desktop/Pac-Man/0.png \
    ../Desktop/Pac-Man/1.png \
    ../Desktop/Pac-Man/2.png \
    ../Desktop/Pac-Man/d1.png \
    ../Desktop/Pac-Man/d1B.png \
    ../Desktop/Pac-Man/d1C.png \
    ../Desktop/Pac-Man/d1I.png \
    ../Desktop/Pac-Man/d1P.png \
    ../Desktop/Pac-Man/deadD.png \
    ../Desktop/Pac-Man/deadL.png \
    ../Desktop/Pac-Man/deadR.png \
    ../Desktop/Pac-Man/deadU.png \
    ../Desktop/Pac-Man/energizer.png \
    ../Desktop/Pac-Man/l1.png \
    ../Desktop/Pac-Man/l1B.png \
    ../Desktop/Pac-Man/l1C.png \
    ../Desktop/Pac-Man/l1I.png \
    ../Desktop/Pac-Man/l1P.png \
    ../Desktop/Pac-Man/map_pure.png \
    ../Desktop/Pac-Man/mortal.png \
    ../Desktop/Pac-Man/pacman_down_close.png \
    ../Desktop/Pac-Man/pacman_down_open_1.png \
    ../Desktop/Pac-Man/pacman_down_open_2.png \
    ../Desktop/Pac-Man/pacman_down_open_3.png \
    ../Desktop/Pac-Man/pacman_left_close.png \
    ../Desktop/Pac-Man/pacman_left_open_1.png \
    ../Desktop/Pac-Man/pacman_left_open_2.png \
    ../Desktop/Pac-Man/pacman_left_open_3.png \
    ../Desktop/Pac-Man/pacman_right_close.png \
    ../Desktop/Pac-Man/pacman_right_open_1.png \
    ../Desktop/Pac-Man/pacman_right_open_2.png \
    ../Desktop/Pac-Man/pacman_right_open_3.png \
    ../Desktop/Pac-Man/pacman_up_close.png \
    ../Desktop/Pac-Man/pacman_up_open_1.png \
    ../Desktop/Pac-Man/pacman_up_open_2.png \
    ../Desktop/Pac-Man/pacman_up_open_3.png \
    ../Desktop/Pac-Man/pellet.png \
    ../Desktop/Pac-Man/r1.png \
    ../Desktop/Pac-Man/r1B.png \
    ../Desktop/Pac-Man/r1C.png \
    ../Desktop/Pac-Man/r1I.png \
    ../Desktop/Pac-Man/r1P.png \
    ../Desktop/Pac-Man/u1.png \
    ../Desktop/Pac-Man/u1B.png \
    ../Desktop/Pac-Man/u1C.png \
    ../Desktop/Pac-Man/u1I.png \
    ../Desktop/Pac-Man/u1P.png \
    ../Desktop/Pac-Man/winner_gif.gif \
    ../Desktop/PacMaze.jpg \
    ../Desktop/l1.png
