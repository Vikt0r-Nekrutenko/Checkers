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

bool Checker::onPlacementHandler(GameModel *model, const Cursor &cursor)
{
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

//bool WChecker::onPlacementHandler(GameModel *model, const Cursor &cursor)
//{
//    stf::Vec2d rMoveFw   = {+1,+1}, lMoveFw   = {-1,+1};
//    stf::Vec2d rAttackFw = {+2,+2}, lAttackFw = {-2,+2};

//    if(attackIsAvailiable(model, cursor, rMoveFw, rAttackFw)) {
//        if(attackIsPossible(model, cursor, rMoveFw, rAttackFw)) {
//            model->board.clear(cursor.selectedCell.pos);
//            model->board.clear(cursor.selectedCell.pos + rMoveFw);
//            return true;
//        } return false;
//    } else if(attackIsAvailiable(model, cursor, lMoveFw, lAttackFw)) {
//        if(attackIsPossible(model, cursor, lMoveFw, lAttackFw)) {
//            model->board.clear(cursor.selectedCell.pos);
//            model->board.clear(cursor.selectedCell.pos + lMoveFw);
//            return true;
//        } return false;
//    } else if(moveIsPossible(model, cursor, rMoveFw) || moveIsPossible(model, cursor, lMoveFw)) {
//        model->board.clear(cursor.selectedCell.pos);
//        return true;
//    } return false;
//}

//bool BChecker::onPlacementHandler(GameModel *model, const Cursor &cursor)
//{
//    stf::Vec2d rMoveFw   = {+1,-1}, lMoveFw   = {-1,-1};
//    stf::Vec2d rAttackFw = {+2,-2}, lAttackFw = {-2,-2};

//    if(attackIsAvailiable(model, cursor, rMoveFw, rAttackFw)) {
//        if(attackIsPossible(model, cursor, rMoveFw, rAttackFw)) {
//            model->board.clear(cursor.selectedCell.pos);
//            model->board.clear(cursor.selectedCell.pos + rMoveFw);
//            return true;
//        } return false;
//    } else if(attackIsAvailiable(model, cursor, lMoveFw, lAttackFw)) {
//        if(attackIsPossible(model, cursor, lMoveFw, lAttackFw)) {
//            model->board.clear(cursor.selectedCell.pos);
//            model->board.clear(cursor.selectedCell.pos + lMoveFw);
//            return true;
//        } return false;
//    } else if(moveIsPossible(model, cursor, rMoveFw) || moveIsPossible(model, cursor, lMoveFw)) {
//        model->board.clear(cursor.selectedCell.pos);
//        return true;
//    } return false;
//}

bool Queen::onPlacementHandler(GameModel *model, const Cursor &cursor)
{
    stf::Vec2d rMoveFw   = {+1,+1}, lMoveFw   = {-1,+1};
    stf::Vec2d rAttackFw = {+2,+2}, lAttackFw = {-2,+2};

    stf::Vec2d rMoveBw   = {+1,-1}, lMoveBw   = {-1,-1};
    stf::Vec2d rAttackBw = {+2,-2}, lAttackBw = {-2,-2};

    if(attackIsAvailiable(model, cursor, rMoveFw, rAttackFw)) {
        if(attackIsPossible(model, cursor, rMoveFw, rAttackFw)) {
            model->board.clear(cursor.selectedCell.pos);
            model->board.clear(cursor.selectedCell.pos + rMoveFw);
            return true;
        } return false;
    } else if(attackIsAvailiable(model, cursor, rMoveBw, rAttackBw)) {
        if(attackIsPossible(model, cursor, rMoveBw, rAttackBw)) {
            model->board.clear(cursor.selectedCell.pos);
            model->board.clear(cursor.selectedCell.pos + rMoveBw);
            return true;
        } return false;
    } else if(attackIsAvailiable(model, cursor, lMoveFw, lAttackFw)) {
        if(attackIsPossible(model, cursor, lMoveFw, lAttackFw)) {
            model->board.clear(cursor.selectedCell.pos);
            model->board.clear(cursor.selectedCell.pos + lMoveFw);
            return true;
        } return false;
    } else if(attackIsAvailiable(model, cursor, lMoveBw, lAttackBw)) {
        if(attackIsPossible(model, cursor, lMoveBw, lAttackBw)) {
            model->board.clear(cursor.selectedCell.pos);
            model->board.clear(cursor.selectedCell.pos + lMoveBw);
            return true;
        } return false;
    } else if(moveIsPossible(model, cursor, rMoveFw) ||
              moveIsPossible(model, cursor, rMoveBw) ||
              moveIsPossible(model, cursor, lMoveFw) ||
              moveIsPossible(model, cursor, lMoveBw))
    {
        model->board.clear(cursor.selectedCell.pos);
        return true;
    } return false;
}
