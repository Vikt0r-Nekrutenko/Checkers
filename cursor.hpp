#ifndef CURSOR_HPP
#define CURSOR_HPP

#include "gameboard.hpp"
#include "vec2d.hpp"

struct Selector
{
    stf::Vec2d pos = stf::Vec2d( 0, 0 );
    BoardCell *cell = GameBoard::emptyCell();
};

struct Cursor
{
    Selector selectableCell;
    Selector selectedCell;

    bool cursorIsEmpty() const;

    bool selectedIsEmpty() const;
    bool selectableIsEmpty() const;

    void reset();
    void select(BoardCell *cell);
};

#endif // CURSOR_HPP
