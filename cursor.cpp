#include "cursor.hpp"


bool Cursor::cursorIsEmpty() const
{
    return selectableIsEmpty() && selectedIsEmpty();
}

bool Cursor::selectedIsEmpty() const
{
    return selectedCell.cell == GameBoard::emptyCell();
}

bool Cursor::selectableIsEmpty() const
{
    return selectableCell.cell == GameBoard::emptyCell();
}

void Cursor::reset()
{
    selectedCell.cell = selectableCell.cell = GameBoard::emptyCell();
}

void Cursor::select(BoardCell *cell)
{
    selectedCell.pos = selectableCell.pos;
    selectedCell.cell = selectableCell.cell = cell;
}
