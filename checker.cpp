#include "checker.hpp"
#include "gamemodel.hpp"

GameTurn *Checker::takeNextTurn(GameModel *model, const Cursor &cursor)
{
//    GameTurn *rAttackTurn = isNextTurnAreAttack(model, cursor, rMoveFw, rAttackFw);
//    GameTurn *lAttackTurn = isNextTurnAreAttack(model, cursor, lMoveFw, lAttackFw);

//    rAttackTurn->turnHandler(model, rMoveFw);
//    lAttackTurn->turnHandler(model, lMoveFw);

//    if(rAttackTurn == turns::nothingTurn() && lAttackTurn == turns::nothingTurn()) {
//        GameTurn *rMoveTurn = moveIsPossible(model, cursor, rMoveFw);
//        GameTurn *lMoveTurn = moveIsPossible(model, cursor, lMoveFw);

//        rMoveTurn->turnHandler(model, cursor.selectedCell.pos);
//        lMoveTurn->turnHandler(model, cursor.selectedCell.pos);
//    }
    bool isAttack = true;
    for(auto i : directions) {
        GameTurn *attackTurn = isNextTurnAreAttack(model, cursor, i.move, i.attack);
        attackTurn->turnHandler(model, i.move);
        isAttack &= (attackTurn == turns::nothingTurn());
    }

    if(isAttack == false)
        return turns::nothingTurn();

    for(auto i : directions) {
        GameTurn *moveTurn = moveIsPossible(model, cursor, i.move);
        moveTurn->turnHandler(model, cursor.selectedCell.pos);
    }

    return turns::nothingTurn();
}

GameTurn *Checker::isAttackTurnAvailiable(GameModel *model, const stf::Vec2d &pos) const
{
    GameTurn *rAttackTurn = BoardCell::isAttackTurnAvailiable(model, pos, rMoveFw, rAttackFw);
    GameTurn *lAttackTurn = BoardCell::isAttackTurnAvailiable(model, pos, lMoveFw, lAttackFw);

    if(rAttackTurn != turns::nothingTurn() || lAttackTurn != turns::nothingTurn())
        return turns::attackTurn();
    return turns::nothingTurn();
}

GameTurn *Checker::isNextTurnAreAttack(GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection, const stf::Vec2d& attackDirection) const
{
    if(BoardCell::isAttackTurnAvailiable(model, cursor, moveDirection, attackDirection) == turns::nothingTurn())
        return turns::nothingTurn();

    if(isAttackTurnPossible(model, cursor, moveDirection, attackDirection) == turns::nothingTurn())
        return turns::mustBeAttackingTurn();

    GameTurn *rfw = isMultiAttackTurn(model, cursor, rMoveFw, rAttackFw);
    GameTurn *lfw = isMultiAttackTurn(model, cursor, lMoveFw, lAttackFw);

     if(rfw == turns::multiAttatckTurn() || lfw == turns::multiAttatckTurn())
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
