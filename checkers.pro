TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -L$$PWD/../stf/release/ -lstf

INCLUDEPATH += $$PWD/../stf \
                $$PWD/../stf/smv \
                $$PWD/../stf/sdb \
                $$PWD/model/headers \
                $$PWD/views/headers \

DEPENDPATH += $$PWD/../stf \
                $$PWD/../stf/smv \
                $$PWD/../stf/sdb \
                $$PWD/model/headers \
                $$PWD/views/headers \

HEADERS += \
    model/headers/boardcell.hpp \
    model/headers/cellcreator.hpp \
    model/headers/checker.hpp \
    model/headers/cursor.hpp \
    model/headers/gameboard.hpp \
    model/headers/gamemodel.hpp \
    model/headers/players.hpp \
    model/headers/queen.hpp \
    model/headers/turns.hpp \
    views/headers/endview.hpp \
    views/headers/gameview.hpp \
    views/headers/closeview.hpp \
    views/headers/gameview.hpp \
    views/headers/menuview.hpp \
    views/headers/pausemenuview.hpp \
    views/headers/storyview.hpp

SOURCES += \
        main.cpp \
        model/sources/boardcell.cpp \
        model/sources/cellcreator.cpp \
        model/sources/checker.cpp \
        model/sources/cursor.cpp \
        model/sources/gameboard.cpp \
        model/sources/gamemodel.cpp \
        model/sources/turns.cpp \
        views/sources/closeview.cpp \
        views/sources/endview.cpp \
        views/sources/gameview.cpp \
        views/sources/menuview.cpp \
        views/sources/pausemenuview.cpp \
        views/sources/storyview.cpp

DISTFILES += \
    sprs/end.spr \
    sprs/menu.spr \
    sprs/pause_menu.spr \
    sprs/sboard.spr \
    sprs/stats.spr
