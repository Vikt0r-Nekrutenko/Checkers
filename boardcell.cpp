#include "boardcell.hpp"
#include "gamemodel.hpp"
#include <string>

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

auto _moveTurnHandler = [](GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection) -> GameTurn *
{
    if(model->board.getSelectableCell(cursor) == GameBoard::emptyCell()
            && cursor.selectedCell.pos + moveDirection == cursor.selectableCell.pos) {
        return turns::moveTurn();
    }
    return turns::nothingTurn();
};

GameTurn *Checker::getNextTurn(GameModel *model, const Cursor &cursor)
{
    GameTurn *rAttackTurn = attackTurnHandler(model, cursor, rMoveFw, rAttackFw);
    GameTurn *lAttackTurn = attackTurnHandler(model, cursor, lMoveFw, lAttackFw);
    GameTurn *rMoveTurn = _moveTurnHandler(model, cursor, rMoveFw);
    GameTurn *lMoveTurn = _moveTurnHandler(model, cursor, lMoveFw);

    if(rAttackTurn != turns::nothingTurn())
        return rAttackTurn;
    else if(lAttackTurn != turns::nothingTurn())
        return lAttackTurn;
    else if(rMoveTurn != turns::nothingTurn())
        return rMoveTurn;
    else
        return lMoveTurn;
}

GameTurn *Checker::attackTurnHandler(GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection, const stf::Vec2d& attackDirection)
{
    if(_attackIsAvailiable(model, cursor, moveDirection, attackDirection)) {
        if(_attackIsPossible(model, cursor, moveDirection, attackDirection)) {
            return turns::attackTurn();
        }
        return turns::mustBeAttackingTurn();
    }
    return turns::nothingTurn();
}

GameTurn *Queen::getNextTurn(GameModel *model, const Cursor &cursor)
{
    GameTurn *rfwAttackTurn = attackTurnHandler(model, cursor, rMoveFw, rAttackFw);
    GameTurn *lfwAttackTurn = attackTurnHandler(model, cursor, lMoveFw, lAttackFw);

    GameTurn *rbwAttackTurn = attackTurnHandler(model, cursor, rMoveBw, rAttackBw);
    GameTurn *lbwAttackTurn = attackTurnHandler(model, cursor, lMoveBw, lAttackBw);

    GameTurn *rfwMoveTurn = _moveTurnHandler(model, cursor, rMoveFw);
    GameTurn *lfwMoveTurn = _moveTurnHandler(model, cursor, lMoveFw);
    GameTurn *rbwMoveTurn = _moveTurnHandler(model, cursor, rMoveBw);
    GameTurn *lbwMoveTurn = _moveTurnHandler(model, cursor, lMoveBw);

         if(rfwAttackTurn != turns::nothingTurn()) return rfwAttackTurn;
    else if(lfwAttackTurn != turns::nothingTurn()) return lfwAttackTurn;
    else if(rbwAttackTurn != turns::nothingTurn()) return rbwAttackTurn;
    else if(lbwAttackTurn != turns::nothingTurn()) return lbwAttackTurn;
    else if(rfwMoveTurn != turns::nothingTurn()) return rfwMoveTurn;
    else if(lfwMoveTurn != turns::nothingTurn()) return lfwMoveTurn;
    else if(rbwMoveTurn != turns::nothingTurn()) return rbwMoveTurn;
    else if(lbwMoveTurn != turns::nothingTurn()) return lbwMoveTurn;
    return turns::nothingTurn();
}
auto _reattackIsAvailiable = [](GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection, const stf::Vec2d& attackDirection) -> GameTurn*
{
    try {
        if(model->board[cursor.selectableCell.pos + moveDirection]->color() == model->opponent()->color()) {
            if(model->board[cursor.selectableCell.pos + attackDirection] == GameBoard::emptyCell()) {
                return turns::multiplyTurn();
            }
        }
    } catch(const std::out_of_range& ex) {
        return turns::nothingTurn();
    }
    return turns::attackTurn();
};

GameTurn *Queen::attackTurnHandler(GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection, const stf::Vec2d& attackDirection)
{
    if(!_attackIsAvailiable(model, cursor, moveDirection, attackDirection))
        return turns::nothingTurn();

    if(!_attackIsPossible(model, cursor, moveDirection, attackDirection))
        return turns::mustBeAttackingTurn();

    GameTurn *rfw = _reattackIsAvailiable(model, cursor, rMoveFw, rAttackFw);
    GameTurn *lfw = _reattackIsAvailiable(model, cursor, lMoveFw, lAttackFw);

    GameTurn *rbw = _reattackIsAvailiable(model, cursor, rMoveBw, rAttackBw);
    GameTurn *lbw = _reattackIsAvailiable(model, cursor, lMoveBw, lAttackBw);


    if(rfw == turns::multiplyTurn()) return rfw;
    if(rbw == turns::multiplyTurn()) return rbw;
    if(lfw == turns::multiplyTurn()) return lfw;
    if(lbw == turns::multiplyTurn()) return lbw;

    return turns::attackTurn();
}
