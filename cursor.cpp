#include "cursor.hpp"


bool Cursor::operator ==(const BoardCell *player) const
{
    return selectableCell.cell->color() == player->color();
}

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

bool Cursor::nextTurnCanBeAttack(GameModel *model) const
{
    return selectedCell.cell->attackIsAvailiable(model, *this);
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
