TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        boardcell.cpp \
        cellcreator.cpp \
        checker.cpp \
        cursor.cpp \
        gameboard.cpp \
        gamemodel.cpp \
        gameview.cpp \
        main.cpp \
        turns.cpp

LIBS += -L$$PWD/../stf/release/ -lstf

INCLUDEPATH += $$PWD/../stf \
                $$PWD/../stf/smv \
                $$PWD/../stf/sdb \
DEPENDPATH += $$PWD/../stf \
                $$PWD/../stf/smv \
                $$PWD/../stf/sdb \

HEADERS += \
    boardcell.hpp \
    cellcreator.hpp \
    checker.hpp \
    cursor.hpp \
    gameboard.hpp \
    gamemodel.hpp \
    gameview.hpp \
    players.hpp \
    queen.hpp \
    turns.hpp
