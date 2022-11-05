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
};

GameTurn* BoardCell::moveIsPossible(GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection) const
{
    if(model->board.getSelectableCell(cursor) == GameBoard::emptyCell() &&
       cursor.selectedCell.pos + moveDirection == cursor.selectableCell.pos)
    {
        return turns::moveTurn();
    }
    return turns::nothingTurn();
};

GameTurn *Checker::takeNextTurn(GameModel *model, const Cursor &cursor)
{
    GameTurn *rAttackTurn = isNextTurnAreAttack(model, cursor, rMoveFw, rAttackFw);
    GameTurn *lAttackTurn = isNextTurnAreAttack(model, cursor, lMoveFw, lAttackFw);

    rAttackTurn->turnHandler(model, rMoveFw);
    lAttackTurn->turnHandler(model, lMoveFw);

    if(rAttackTurn == turns::nothingTurn() && lAttackTurn == turns::nothingTurn()) {
        GameTurn *rMoveTurn = moveIsPossible(model, cursor, rMoveFw);
        GameTurn *lMoveTurn = moveIsPossible(model, cursor, lMoveFw);

        rMoveTurn->turnHandler(model, cursor.selectedCell.pos);
        lMoveTurn->turnHandler(model, cursor.selectedCell.pos);
    }
    return turns::nothingTurn();
}

GameTurn *Checker::isNextTurnAreAttack(GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection, const stf::Vec2d& attackDirection)
{
    if(isAttackTurnAvailiable(model, cursor, moveDirection, attackDirection) == turns::nothingTurn())
        return turns::nothingTurn();

    if(isAttackTurnPossible(model, cursor, moveDirection, attackDirection) == turns::nothingTurn())
        return turns::mustBeAttackingTurn();

    GameTurn *rfw = isMultiAttackTurn(model, cursor, rMoveFw, rAttackFw);
    GameTurn *lfw = isMultiAttackTurn(model, cursor, lMoveFw, lAttackFw);

     if(rfw == turns::multiAttatckTurn() || lfw == turns::multiAttatckTurn())
         return turns::multiAttatckTurn();

    return turns::attackTurn();
}

GameTurn *Queen::takeNextTurn(GameModel *model, const Cursor &cursor)
{
    GameTurn *rfwAttackTurn = isNextTurnAreAttack(model, cursor, rMoveFw, rAttackFw);
    GameTurn *lfwAttackTurn = isNextTurnAreAttack(model, cursor, lMoveFw, lAttackFw);

    GameTurn *rbwAttackTurn = isNextTurnAreAttack(model, cursor, rMoveBw, rAttackBw);
    GameTurn *lbwAttackTurn = isNextTurnAreAttack(model, cursor, lMoveBw, lAttackBw);

    GameTurn *rfwMoveTurn = moveIsPossible(model, cursor, rMoveFw);
    GameTurn *lfwMoveTurn = moveIsPossible(model, cursor, lMoveFw);
    GameTurn *rbwMoveTurn = moveIsPossible(model, cursor, rMoveBw);
    GameTurn *lbwMoveTurn = moveIsPossible(model, cursor, lMoveBw);

    rfwAttackTurn->turnHandler(model, rMoveFw);
    lfwAttackTurn->turnHandler(model, lMoveFw);
    rbwAttackTurn->turnHandler(model, rMoveBw);
    lbwAttackTurn->turnHandler(model, lMoveBw);

    if(rfwAttackTurn == turns::nothingTurn() && lfwAttackTurn == turns::nothingTurn() &&
       rbwAttackTurn == turns::nothingTurn() && lbwAttackTurn == turns::nothingTurn())
    {
        rfwMoveTurn->turnHandler(model, rMoveFw);
        lfwMoveTurn->turnHandler(model, lMoveFw);
        rbwMoveTurn->turnHandler(model, rMoveBw);
        lbwMoveTurn->turnHandler(model, lMoveBw);
    }

    return turns::nothingTurn();
}

GameTurn *Queen::isNextTurnAreAttack(GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection, const stf::Vec2d& attackDirection)
{
    if(isAttackTurnAvailiable(model, cursor, moveDirection, attackDirection) == turns::nothingTurn())
        return turns::nothingTurn();

    if(isAttackTurnPossible(model, cursor, moveDirection, attackDirection) == turns::nothingTurn())
        return turns::mustBeAttackingTurn();

    GameTurn *rfw = isMultiAttackTurn(model, cursor, rMoveFw, rAttackFw);
    GameTurn *lfw = isMultiAttackTurn(model, cursor, lMoveFw, lAttackFw);
    GameTurn *rbw = isMultiAttackTurn(model, cursor, rMoveBw, rAttackBw);
    GameTurn *lbw = isMultiAttackTurn(model, cursor, lMoveBw, lAttackBw);

     if(rfw == turns::multiAttatckTurn() ||
        rbw == turns::multiAttatckTurn() ||
        lfw == turns::multiAttatckTurn() ||
        lbw == turns::multiAttatckTurn())
         return turns::multiAttatckTurn();

    return turns::attackTurn();
}

bool WChecker::isTransformPossible(GameModel *model)
{
    return model->board.isInBlackZone(model->cursor.selectableCell.pos);
}

BoardCell *WChecker::getTransformPiece()
{
    return GameBoard::whiteQueen();
}

bool BChecker::isTransformPossible(GameModel *model)
{
    return model->board.isInWhiteZone(model->cursor.selectableCell.pos);
}

BoardCell *BChecker::getTransformPiece()
{
    return GameBoard::blackQueen();
}
