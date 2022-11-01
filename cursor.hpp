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
        return selectedCell.cell == selectableCell.cell && selectableCell.cell == GameBoard::emptyCell();
    }

    void reset() { selectedCell.cell = selectableCell.cell = GameBoard::emptyCell(); }
};

#endif // CURSOR_HPP
