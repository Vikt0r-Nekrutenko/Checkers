#include "boardcell.hpp"
#include "gamemodel.hpp"
#include <string>

stf::sdb::DynamicFieldsAllocator BoardCell::_cellAllocator = stf::sdb::DynamicFieldsAllocator();

auto isOpponent = [](GameModel *model, const Cursor& cursor, const stf::Vec2d& direction){
    return model->board[cursor.selectedCell.pos + direction]->color() == model->opponent()->color();
};

BoardCell *BoardCell::transformation(GameModel *model)
{
    return isTransformPossible(model)
            ? getTransformPiece()
            : this;
}

GameTurn* BoardCell::isAttackTurnAvailiable(GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection, const stf::Vec2d& attackDirection) const
{
    try {
        BoardCell *cellBehindOpponent = model->board[cursor.selectedCell.pos + attackDirection];

        if(isOpponent(model, cursor, moveDirection) && cellBehindOpponent == GameBoard::emptyCell())
            return turns::attackTurn();
    } catch(const std::out_of_range& ex) {}
    return turns::nothingTurn();
}

GameTurn *BoardCell::isAttackTurnAvailiable(GameModel *model, const stf::Vec2d &pos, const stf::Vec2d &moveDirection, const stf::Vec2d &attackDirection) const
{
    try {
        BoardCell *cellBehindOpponent = model->board[pos + attackDirection];

        if(model->board[pos + moveDirection]->color() == model->opponent()->color() &&
                cellBehindOpponent == GameBoard::emptyCell())
            return turns::attackTurn();
    } catch(const std::out_of_range& ex) {}
    return turns::nothingTurn();
}

GameTurn* BoardCell::isMultiAttackTurn(GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection, const stf::Vec2d& attackDirection) const
{
    try {
        BoardCell *cellBehindOpponent = model->board[cursor.selectableCell.pos + attackDirection];

        if(model->board[cursor.selectableCell.pos + moveDirection]->color() == model->opponent()->color() &&
                cellBehindOpponent == GameBoard::emptyCell())
            return turns::multiAttatckTurn();
    } catch(const std::out_of_range& ex) {
        return turns::nothingTurn();
    }
    return turns::attackTurn();
}

GameTurn* BoardCell::isAttackTurnPossible(GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection, const stf::Vec2d& attackDirection) const
{
    if(model->board.getSelectableCell(cursor) == GameBoard::emptyCell() &&
       cursor.selectedCell.pos + attackDirection == cursor.selectableCell.pos &&
       isOpponent(model, cursor, moveDirection))
    {
        return turns::attackTurn();
    }
    return turns::nothingTurn();
}

GameTurn* BoardCell::moveIsPossible(GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection) const
{
    if(model->board.getSelectableCell(cursor) == GameBoard::emptyCell() &&
       cursor.selectedCell.pos + moveDirection == cursor.selectableCell.pos)
    {
        return turns::moveTurn();
    }
    return turns::nothingTurn();
}
