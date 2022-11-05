TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -L$$PWD/../stf/release/ -lstf

INCLUDEPATH += $$PWD/../stf \
                $$PWD/../stf/smv \
                $$PWD/../stf/sdb \
                $$PWD/model/headers \

DEPENDPATH += $$PWD/../stf \
                $$PWD/../stf/smv \
                $$PWD/../stf/sdb \
                $$PWD/model/headers \

HEADERS += \
    model/headers/boardcell.hpp \
    model/headers/cellcreator.hpp \
    model/headers/checker.hpp \
    model/headers/cursor.hpp \
    model/headers/gameboard.hpp \
    model/headers/gamemodel.hpp \
    model/headers/gameview.hpp \
    model/headers/players.hpp \
    model/headers/queen.hpp \
    model/headers/turns.hpp \

SOURCES += \
        main.cpp \
        model/sources/boardcell.cpp \
        model/sources/cellcreator.cpp \
        model/sources/checker.cpp \
        model/sources/cursor.cpp \
        model/sources/gameboard.cpp \
        model/sources/gamemodel.cpp \
        model/sources/gameview.cpp \
        model/sources/turns.cpp \
