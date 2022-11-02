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

auto moveIsPossible = [](GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection)
{
    if(model->board.getSelectableCell(cursor) == GameBoard::emptyCell()
            && cursor.selectedCell.pos + moveDirection == cursor.selectableCell.pos)
        return true;
    return false;
};

bool WChecker::onPlacementHandler(GameModel *model, const Cursor &cursor)
{
    if(attackIsAvailiable(model, cursor, {+1,-1}, {+2,-2})) {
        if(attackIsPossible(model, cursor, {+1,-1}, {+2,-2})) {
            model->board.clear(cursor.selectedCell.pos);
            model->board.clear(cursor.selectedCell.pos + stf::Vec2d{+1,-1});
            return true;
        } return false;
    } else if(attackIsAvailiable(model, cursor, {-1,-1}, {-2,-2})) {
        if(attackIsPossible(model, cursor, {-1,-1}, {-2,-2})) {
            model->board.clear(cursor.selectedCell.pos);
            model->board.clear(cursor.selectedCell.pos + stf::Vec2d{-1,-1});
            return true;
        } return false;
    } else if(moveIsPossible(model, cursor, {+1,-1}) || moveIsPossible(model, cursor, {-1,-1})) {
        model->board.clear(cursor.selectedCell.pos);
        return true;
    } return false;
}

bool BChecker::onPlacementHandler(GameModel *model, const Cursor &cursor)
{
    stf::Vec2d rMoveFw   = {+1,-1}, lMoveFw   = {-1,-1};
    stf::Vec2d rAttackFw = {+2,-2}, lAttackFw = {-2,-2};

    if(attackIsAvailiable(model, cursor, rMoveFw, rAttackFw)) {
        if(attackIsPossible(model, cursor, rMoveFw, rAttackFw)) {
            model->board.clear(cursor.selectedCell.pos);
            model->board.clear(cursor.selectedCell.pos + rMoveFw);
            return true;
        } return false;
    } else if(attackIsAvailiable(model, cursor, lMoveFw, lAttackFw)) {
        if(attackIsPossible(model, cursor, lMoveFw, lAttackFw)) {
            model->board.clear(cursor.selectedCell.pos);
            model->board.clear(cursor.selectedCell.pos + lMoveFw);
            return true;
        } return false;
    } else if(moveIsPossible(model, cursor, rMoveFw) || moveIsPossible(model, cursor, lMoveFw)) {
        model->board.clear(cursor.selectedCell.pos);
        return true;
    } return false;
}

bool Queen::onPlacementHandler(GameModel *model, const Cursor &cursor)
{
/*    if(attackIsAvailiable(model, cursor)) {
        if(rAttackIsPossible(model, cursor)) {
            model->board.clear(cursor.selectedCell.pos);
            model->board.clear(cursor.selectedCell.pos + moveFwR);
            return true;
        } return false;
    } else if(attackIsAvailiable(model, cursor, moveBwR, attackBwR)) {
        if(attackIsPossible(model, cursor, moveBwR, attackBwR)) {
            model->board.clear(cursor.selectedCell.pos);
            model->board.clear(cursor.selectedCell.pos + moveBwR);
            return true;
        } return false;
    } else if(lAttackAvailiable(model, cursor)) {
        if(lAttackIsPossible(model, cursor)) {
            model->board.clear(cursor.selectedCell.pos);
            model->board.clear(cursor.selectedCell.pos + moveFwL);
            return true;
        } return false;
    } else if(attackIsAvailiable(model, cursor, moveBwL, attackBwL)) {
        if(attackIsPossible(model, cursor, moveBwL, attackBwL)) {
            model->board.clear(cursor.selectedCell.pos);
            model->board.clear(cursor.selectedCell.pos + moveBwL);
            return true;
        } return false;
    } else if(rMoveIsPossible(model, cursor) || lMoveIsPossible(model, cursor) || moveIsPossible(model, cursor, moveBwR) || moveIsPossible(model, cursor, moveBwL)) {
        model->board.clear(cursor.selectedCell.pos);
        return true;
    }*/ return false;
}
