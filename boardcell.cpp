#include "boardcell.hpp"
#include "gamemodel.hpp"
#include <string>

stf::sdb::DynamicFieldsAllocator BoardCell::_cellAllocator = stf::sdb::DynamicFieldsAllocator();

auto isOpponent = [](GameModel *model, const Cursor& cursor, const stf::Vec2d& direction){
    return model->board[cursor.selectedCell.pos + direction]->color() == model->opponent()->color();
};

void *BoardCell::operator new(size_t size)
{
    return _cellAllocator.allocate(size);
}

uint8_t BoardCell::view() const
{
    return 0;
}

stf::ColorTable BoardCell::color() const
{
    return stf::ColorTable::Default;
}

bool BoardCell::isTransformPossible(GameModel *)
{
    return false;
}

BoardCell *BoardCell::getTransformPiece()
{
    return this;
}

BoardCell *BoardCell::transformation(GameModel *model)
{
    return isTransformPossible(model)
            ? getTransformPiece()
            : this;
}

GameTurn * BoardCell::takeNextTurn(GameModel *model, const Cursor &cursor)
{
    for(auto i : directions) {
        GameTurn *attackTurn = isNextTurnAreAttack(model, cursor, i.move, i.attack);
        attackTurn->turnHandler(model, i.move);
        if(attackTurn != turns::nothingTurn())
            return attackTurn;
    }

    GameTurn *moveTurn = turns::nothingTurn();
    for(auto i : directions) {
        moveTurn = moveIsPossible(model, cursor, i.move);
        moveTurn->turnHandler(model, cursor.selectedCell.pos);
    }
    return moveTurn;
}

GameTurn * BoardCell::isNextTurnAreAttack(GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection, const stf::Vec2d& attackDirection) const
{
    if(BoardCell::isAttackTurnAvailiable(model, cursor, moveDirection, attackDirection) == turns::nothingTurn())
        return turns::nothingTurn();

    if(isAttackTurnPossible(model, cursor, moveDirection, attackDirection) == turns::nothingTurn())
        return turns::mustBeAttackingTurn();

    for(auto i : directions) {
        GameTurn *turn = isMultiAttackTurn(model, cursor, i.move, i.attack);
        if(turn == turns::multiAttatckTurn())
            return turns::multiAttatckTurn();
    }
    return turns::attackTurn();
}

GameTurn * BoardCell::isAttackTurnAvailiable(GameModel *model, const stf::Vec2d &pos) const
{
    for(auto i : directions) {
        GameTurn *attackTurn = BoardCell::isAttackTurnAvailiable(model, pos, i.move, i.attack);
        if(attackTurn != turns::nothingTurn())
            return turns::attackTurn();
    }
    return turns::nothingTurn();
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
