#include "queen.hpp"
#include "gamemodel.hpp"

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
        rfwMoveTurn->turnHandler(model, cursor.selectedCell.pos);
        lfwMoveTurn->turnHandler(model, cursor.selectedCell.pos);
        rbwMoveTurn->turnHandler(model, cursor.selectedCell.pos);
        lbwMoveTurn->turnHandler(model, cursor.selectedCell.pos);
    }

    return turns::nothingTurn();
}

GameTurn *Queen::isAttackTurnAvailiable(GameModel *model, const stf::Vec2d &pos) const
{
    GameTurn *rAttackTurn = BoardCell::isAttackTurnAvailiable(model, pos, rMoveFw, rAttackFw);
    GameTurn *lAttackTurn = BoardCell::isAttackTurnAvailiable(model, pos, lMoveFw, lAttackFw);

    GameTurn *rbwAttackTurn = BoardCell::isAttackTurnAvailiable(model, pos, rMoveBw, rAttackBw);
    GameTurn *lbwAttackTurn = BoardCell::isAttackTurnAvailiable(model, pos, lMoveBw, lAttackBw);

    if(rAttackTurn != turns::nothingTurn() || lAttackTurn != turns::nothingTurn() ||
        rbwAttackTurn != turns::nothingTurn() || lbwAttackTurn != turns::nothingTurn())
        return turns::attackTurn();
    return turns::nothingTurn();
}

GameTurn *Queen::isNextTurnAreAttack(GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection, const stf::Vec2d& attackDirection) const
{
    if(BoardCell::isAttackTurnAvailiable(model, cursor, moveDirection, attackDirection) == turns::nothingTurn())
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
