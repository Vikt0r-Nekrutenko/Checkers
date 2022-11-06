#include "cursor.hpp"

void Cursor::reset()
{
    selectedCell.cell = selectableCell.cell = GameBoard::emptyCell();
}

void Cursor::select(BoardCell *cell)
{
    selectedCell.pos = selectableCell.pos;
    selectedCell.cell = selectableCell.cell = cell;
}
