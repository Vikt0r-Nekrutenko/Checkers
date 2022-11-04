#include "boardcell.hpp"
#include "gamemodel.hpp"
#include <string>

stf::sdb::DynamicFieldsAllocator BoardCell::_cellAllocator = stf::sdb::DynamicFieldsAllocator();

auto isOpponent = [](GameModel *model, const Cursor& cursor, const stf::Vec2d& direction){
    return model->board[cursor.selectedCell.pos + direction]->color() == model->opponent()->color();
};

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

    rAttackTurn->turnHandler(model);
    lAttackTurn->turnHandler(model);

    if(rAttackTurn == turns::nothingTurn() && lAttackTurn == turns::nothingTurn()) {
        GameTurn *rMoveTurn = moveIsPossible(model, cursor, rMoveFw);
        GameTurn *lMoveTurn = moveIsPossible(model, cursor, lMoveFw);

        rMoveTurn->turnHandler(model);
        lMoveTurn->turnHandler(model);
    }
    return turns::nothingTurn();
}

GameTurn *Checker::isNextTurnAreAttack(GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection, const stf::Vec2d& attackDirection)
{
    if(isAttackTurnAvailiable(model, cursor, moveDirection, attackDirection) == turns::nothingTurn())
        return turns::nothingTurn();

    if(!isAttackTurnPossible(model, cursor, moveDirection, attackDirection))
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

    rfwAttackTurn->turnHandler(model);
    lfwAttackTurn->turnHandler(model);
    rbwAttackTurn->turnHandler(model);
    lbwAttackTurn->turnHandler(model);

    if(rfwAttackTurn == turns::nothingTurn() && lfwAttackTurn == turns::nothingTurn() &&
       rbwAttackTurn == turns::nothingTurn() && lbwAttackTurn == turns::nothingTurn())
    {
        rfwMoveTurn->turnHandler(model);
        lfwMoveTurn->turnHandler(model);
        rbwMoveTurn->turnHandler(model);
        lbwMoveTurn->turnHandler(model);
    }

    return turns::nothingTurn();
}

GameTurn *Queen::isNextTurnAreAttack(GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection, const stf::Vec2d& attackDirection)
{
    if(isAttackTurnAvailiable(model, cursor, moveDirection, attackDirection) == turns::nothingTurn())
        return turns::nothingTurn();

    if(!isAttackTurnPossible(model, cursor, moveDirection, attackDirection))
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
