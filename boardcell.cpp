#include "boardcell.hpp"
#include "gamemodel.hpp"

stf::sdb::DynamicFieldsAllocator BoardCell::_cellAllocator = stf::sdb::DynamicFieldsAllocator();

auto isOpponent = [](GameModel *model, const Cursor& cursor, const stf::Vec2d& direction){
    return model->board[cursor.selectedCell.pos + direction]->color() == model->opponent()->color();
};

auto attackIsAvailiable = [](GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection, const stf::Vec2d& attackDirection)
{
    try {
        BoardCell *lCellBehindOpponent = model->board[cursor.selectedCell.pos + attackDirection];

        if(isOpponent(model, cursor, moveDirection) && lCellBehindOpponent == GameBoard::emptyCell())
            return true;
    } catch(const std::out_of_range& ex) {
        ++model->exCount;
        return false;
    }
    return false;
};

auto attackIsPossible = [](GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection, const stf::Vec2d& attackDirection)
{
    if(model->board.getSelectableCell(cursor) == GameBoard::emptyCell() &&
       cursor.selectedCell.pos + attackDirection == cursor.selectableCell.pos &&
       isOpponent(model, cursor, moveDirection))
        return true;
    return false;
};



bool MovableObject::lMoveIsPossible(GameModel *model, const Cursor& cursor)
{
    if(model->board.getSelectableCell(cursor) == GameBoard::emptyCell()
            && cursor.selectedCell.pos + moveDirectionL == cursor.selectableCell.pos)
        return true;
    return false;
}

bool MovableObject::rMoveIsPossible(GameModel *model, const Cursor& cursor)
{
    if(model->board.getSelectableCell(cursor) == GameBoard::emptyCell()
            && cursor.selectedCell.pos + moveDirectionR == cursor.selectableCell.pos)
        return true;
    return false;
}

bool MovableObject::lAttackIsPossible(GameModel *model, const Cursor& cursor)
{
    return attackIsPossible(model, cursor, moveDirectionL, attackDirectionL);
}

bool MovableObject::rAttackIsPossible(GameModel *model, const Cursor& cursor)
{
    return attackIsPossible(model, cursor, moveDirectionR, attackDirectionR);
}

bool MovableObject::lAttackAvailiable(GameModel *model, const Cursor& cursor) const
{
    return attackIsAvailiable(model, cursor, moveDirectionL, attackDirectionL);
}

bool MovableObject::rAttackAvailiable(GameModel *model, const Cursor& cursor) const
{
    return attackIsAvailiable(model, cursor, moveDirectionR, attackDirectionR);
}

bool MovableObject::isFightAvailiable(GameModel *model, const Cursor& cursor) const
{
    try {
        BoardCell *lCellBehindOpponent = model->board[cursor.selectedCell.pos + attackDirectionL];
        BoardCell *rCellBehindOpponent = model->board[cursor.selectedCell.pos + attackDirectionR];

        if((isOpponent(model, cursor, moveDirectionL) && lCellBehindOpponent == GameBoard::emptyCell()) ||
           (isOpponent(model, cursor, moveDirectionR) && rCellBehindOpponent == GameBoard::emptyCell()))
            return true;
    } catch(const std::out_of_range& ex) {
        return false;
    }
    return false;
}

bool MovableObject::onPlacementHandler(GameModel *model, const Cursor &cursor)
{
    if(rAttackAvailiable(model, cursor)) {
        if(rAttackIsPossible(model, cursor)) {
            model->board.clear(cursor.selectedCell.pos);
            model->board.clear(cursor.selectedCell.pos + moveDirectionR);
            return true;
        } return false;
    } else if(lAttackAvailiable(model, cursor)) {
        if(lAttackIsPossible(model, cursor)) {
            model->board.clear(cursor.selectedCell.pos);
            model->board.clear(cursor.selectedCell.pos + moveDirectionL);
            return true;
        } return false;
    } else if(rMoveIsPossible(model, cursor) || lMoveIsPossible(model, cursor)) {
        model->board.clear(cursor.selectedCell.pos);
        return true;
    } return false;
}
