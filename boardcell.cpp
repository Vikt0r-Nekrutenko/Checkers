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
            && cursor.selectedCell.pos + moveFwL == cursor.selectableCell.pos)
        return true;
    return false;
}

bool MovableObject::rMoveIsPossible(GameModel *model, const Cursor& cursor)
{
    if(model->board.getSelectableCell(cursor) == GameBoard::emptyCell()
            && cursor.selectedCell.pos + moveFwR == cursor.selectableCell.pos)
        return true;
    return false;
}

bool MovableObject::lAttackIsPossible(GameModel *model, const Cursor& cursor)
{
    return attackIsPossible(model, cursor, moveFwL, attackBwL);
}

bool MovableObject::rAttackIsPossible(GameModel *model, const Cursor& cursor)
{
    return attackIsPossible(model, cursor, moveFwR, attackDattackFwRool MovableObject::lAttackAvailiable(GameModel *model, const Cursor& cursor) const
{
    return attackIsAvailiable(model, cursor, moveFwL, attackDirectionL);
}

bool MovableObject::rAttackAvailiable(GameModel *model, const Cursor& cursor) const
{
    return attackIsAvailiable(model, cursor, moveFwR, attackDattackFwRool MovableObject::isFightAvailiable(GameModel *model, const Cursor& cursor) const
{
    try {
        BoardCell *lCellBehindOpponent = model->board[cursor.selectedCell.pos + attackDirectionL];
        BoardCell *rCellBehindOpponent = model->board[cursor.selectedCell.pos + attackFwR];

        if((isOpponent(model, cursor, moveFwL) && lCellBehindOpponent == GameBoard::emptyCell()) ||
           (isOpponent(model, cursor, moveFwR) && rCellBehindOpponent == GameBoard::emptyCell()))
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
            model->board.clear(cursor.selectedCell.pos + moveFwR);
            return true;
        } return false;
    } else if(lAttackAvailiable(model, cursor)) {
        if(lAttackIsPossible(model, cursor)) {
            model->board.clear(cursor.selectedCell.pos);
            model->board.clear(cursor.selectedCell.pos + moveFwL);
            return true;
        } return false;
    } else if(rMoveIsPossible(model, cursor) || lMoveIsPossible(model, cursor)) {
        model->board.clear(cursor.selectedCell.pos);
        return true;
    } return false;
}
