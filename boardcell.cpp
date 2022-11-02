#include "boardcell.hpp"
#include "gamemodel.hpp"

stf::sdb::DynamicFieldsAllocator BoardCell::_cellAllocator = stf::sdb::DynamicFieldsAllocator();

auto isOpponent = [](GameModel *model, const Cursor& cursor, const stf::Vec2d& direction){
    return model->board[cursor.selectedCell.pos + direction]->color() == model->opponent()->color();
};

auto _attackIsAvailiable = [](GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection, const stf::Vec2d& attackDirection)
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

auto _attackIsPossible = [](GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection, const stf::Vec2d& attackDirection)
{
    if(model->board.getSelectableCell(cursor) == GameBoard::emptyCell() &&
       cursor.selectedCell.pos + attackDirection == cursor.selectableCell.pos &&
       isOpponent(model, cursor, moveDirection))
        return true;
    return false;
};

auto _moveIsPossible = [](GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection)
{
    if(model->board.getSelectableCell(cursor) == GameBoard::emptyCell()
            && cursor.selectedCell.pos + moveDirection == cursor.selectableCell.pos)
        return true;
    return false;
};

bool Checker::onPlacementHandler(GameModel *model, const Cursor &cursor)
{
    if(_attackIsAvailiable(model, cursor, rMoveFw, rAttackFw)) {
        if(_attackIsPossible(model, cursor, rMoveFw, rAttackFw)) {
            model->board.clear(cursor.selectedCell.pos);
            model->board.clear(cursor.selectedCell.pos + rMoveFw);
            return true;
        } return false;
    } else if(_attackIsAvailiable(model, cursor, lMoveFw, lAttackFw)) {
        if(_attackIsPossible(model, cursor, lMoveFw, lAttackFw)) {
            model->board.clear(cursor.selectedCell.pos);
            model->board.clear(cursor.selectedCell.pos + lMoveFw);
            return true;
        } return false;
    } else if(_moveIsPossible(model, cursor, rMoveFw) || _moveIsPossible(model, cursor, lMoveFw)) {
        model->board.clear(cursor.selectedCell.pos);
        return true;
    } return false;
}

bool Checker::attackIsAvailiable(GameModel *model, const Cursor &cursor)
{
    return _attackIsAvailiable(model, cursor, rMoveFw, rAttackFw) ||
           _attackIsAvailiable(model, cursor, lMoveFw, lAttackFw);
}

bool Queen::onPlacementHandler(GameModel *model, const Cursor &cursor)
{
    if(_attackIsAvailiable(model, cursor, rMoveFw, rAttackFw)) {
        if(_attackIsPossible(model, cursor, rMoveFw, rAttackFw)) {
            model->board.clear(cursor.selectedCell.pos);
            model->board.clear(cursor.selectedCell.pos + rMoveFw);
            return true;
        } return false;
    } else if(_attackIsAvailiable(model, cursor, rMoveBw, rAttackBw)) {
        if(_attackIsPossible(model, cursor, rMoveBw, rAttackBw)) {
            model->board.clear(cursor.selectedCell.pos);
            model->board.clear(cursor.selectedCell.pos + rMoveBw);
            return true;
        } return false;
    } else if(_attackIsAvailiable(model, cursor, lMoveFw, lAttackFw)) {
        if(_attackIsPossible(model, cursor, lMoveFw, lAttackFw)) {
            model->board.clear(cursor.selectedCell.pos);
            model->board.clear(cursor.selectedCell.pos + lMoveFw);
            return true;
        } return false;
    } else if(_attackIsAvailiable(model, cursor, lMoveBw, lAttackBw)) {
        if(_attackIsPossible(model, cursor, lMoveBw, lAttackBw)) {
            model->board.clear(cursor.selectedCell.pos);
            model->board.clear(cursor.selectedCell.pos + lMoveBw);
            return true;
        } return false;
    } else if(_moveIsPossible(model, cursor, rMoveFw) ||
              _moveIsPossible(model, cursor, rMoveBw) ||
              _moveIsPossible(model, cursor, lMoveFw) ||
              _moveIsPossible(model, cursor, lMoveBw))
    {
        model->board.clear(cursor.selectedCell.pos);
        return true;
    } return false;
}

bool Queen::attackIsAvailiable(GameModel *model, const Cursor &cursor)
{
    return _attackIsAvailiable(model, cursor, rMoveFw, rAttackFw) || _attackIsAvailiable(model, cursor, lMoveFw, lAttackFw) ||
           _attackIsAvailiable(model, cursor, rMoveBw, rAttackBw) || _attackIsAvailiable(model, cursor, lMoveBw, lAttackBw);
}
