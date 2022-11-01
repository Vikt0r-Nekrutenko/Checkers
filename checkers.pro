TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        boardcell.cpp \
        gameboard.cpp \
        gamemodel.cpp \
        gameview.cpp \
        main.cpp

LIBS += -L$$PWD/../stf/release/ -lstf

INCLUDEPATH += $$PWD/../stf \
                $$PWD/../stf/smv \
                $$PWD/../stf/sdb \
DEPENDPATH += $$PWD/../stf \
                $$PWD/../stf/smv \
                $$PWD/../stf/sdb \

HEADERS += \
    boardcell.hpp \
    gameboard.hpp \
    gamemodel.hpp \
    gameview.hpp
