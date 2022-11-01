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

    bool cursorIsEmpty() const
    {
        return selectableIsEmpty() && selectedIsEmpty();
    }

    bool selectedIsEmpty() const { return selectedCell.cell == GameBoard::emptyCell(); }
    bool selectableIsEmpty() const { return selectableCell.cell == GameBoard::emptyCell(); }

    void reset() { selectedCell.cell = selectableCell.cell = GameBoard::emptyCell(); }
};

#endif // CURSOR_HPP
