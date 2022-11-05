#include "checker.hpp"
#include "gamemodel.hpp"

GameTurn *Checker::takeNextTurn(GameModel *model, const Cursor &cursor)
{
    GameTurn *isAttack = turns::attackTurn();

    for(auto i : directions) {
        GameTurn *attackTurn = isNextTurnAreAttack(model, cursor, i.move, i.attack);
        attackTurn->turnHandler(model, i.move);
        isAttack = (attackTurn == turns::nothingTurn())
                ? turns::nothingTurn()
                : attackTurn;
    }

    if(isAttack != turns::attackTurn())
        return turns::nothingTurn();

    for(auto i : directions) {
        GameTurn *moveTurn = moveIsPossible(model, cursor, i.move);
        moveTurn->turnHandler(model, cursor.selectedCell.pos);
    }

    return turns::nothingTurn();
}

GameTurn *Checker::isAttackTurnAvailiable(GameModel *model, const stf::Vec2d &pos) const
{
    for(auto i : directions) {
        GameTurn *attackTurn = BoardCell::isAttackTurnAvailiable(model, pos, i.move, i.attack);
        if(attackTurn != turns::nothingTurn())
            return turns::attackTurn();
    }
    return turns::nothingTurn();
}

GameTurn *Checker::isNextTurnAreAttack(GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection, const stf::Vec2d& attackDirection) const
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
