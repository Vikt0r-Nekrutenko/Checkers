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

auto _reattackIsAvailiable = [](GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection, const stf::Vec2d& attackDirection)
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

auto _moveTurnHandler = [](GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection) -> GameTurn *
{
    if(model->board.getSelectableCell(cursor) == GameBoard::emptyCell()
            && cursor.selectedCell.pos + moveDirection == cursor.selectableCell.pos) {
        model->board.clear(cursor.selectedCell.pos);
        return turns::moveTurn();
    }
    return turns::nothingTurn();
};

auto _attackTurnHandler = [](GameModel *model, const Cursor& cursor, const stf::Vec2d& moveDirection, const stf::Vec2d& attackDirection) -> GameTurn *
{
    if(_attackIsAvailiable(model, cursor, moveDirection, attackDirection)) {
        if(_attackIsPossible(model, cursor, moveDirection, attackDirection)) {
            model->board.clear(cursor.selectedCell.pos);
            model->board.clear(cursor.selectedCell.pos + moveDirection);

            stf::Vec2d reattackPos = cursor.selectableCell.pos + moveDirection;
            Cursor reattackCursor = { { reattackPos, model->board[reattackPos] },
                                      { cursor.selectableCell                  }};

            if(_attackIsAvailiable(model, reattackCursor, moveDirection, attackDirection))
                return turns::reattackTurn();
            return turns::attackTurn();
        }
    }
    return turns::nothingTurn();
};

GameTurn *Checker::onPlacementHandler(GameModel *model, const Cursor &cursor)
{
    GameTurn *rAttackTurn = _attackTurnHandler(model, cursor, rMoveFw, rAttackFw);
    GameTurn *lAttackTurn = _attackTurnHandler(model, cursor, lMoveFw, lAttackFw);
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
//    if(_attackIsAvailiable(model, cursor, rMoveFw, rAttackFw)) {
//        if(_attackIsPossible(model, cursor, rMoveFw, rAttackFw)) {
//            model->board.clear(cursor.selectedCell.pos);
//            model->board.clear(cursor.selectedCell.pos + rMoveFw);
//            return turns::attackTurn();
//        } return turns::nothingTurn();
//    } else if(_attackIsAvailiable(model, cursor, lMoveFw, lAttackFw)) {
//        if(_attackIsPossible(model, cursor, lMoveFw, lAttackFw)) {
//            model->board.clear(cursor.selectedCell.pos);
//            model->board.clear(cursor.selectedCell.pos + lMoveFw);
//            return turns::attackTurn();
//        } return turns::nothingTurn();
//    } else if(_moveIsPossible(model, cursor, rMoveFw) || _moveIsPossible(model, cursor, lMoveFw)) {
//        model->board.clear(cursor.selectedCell.pos);
//        return turns::moveTurn();
//    } return turns::nothingTurn();
}

bool Checker::attackIsAvailiable(GameModel *model, const Cursor &cursor)
{
    return _attackIsAvailiable(model, cursor, rMoveFw, rAttackFw) ||
            _attackIsAvailiable(model, cursor, lMoveFw, lAttackFw);
}

bool Checker::moveIsAvailiable(GameModel *model, const Cursor &cursor)
{
    return !attackIsAvailiable(model, cursor) &&
            (_moveIsPossible(model, cursor, rMoveFw) ||
             _moveIsPossible(model, cursor, lMoveFw));
}

GameTurn *Queen::onPlacementHandler(GameModel *model, const Cursor &cursor)
{
    if(_attackIsAvailiable(model, cursor, rMoveFw, rAttackFw)) {
        if(_attackIsPossible(model, cursor, rMoveFw, rAttackFw)) {
            model->board.clear(cursor.selectedCell.pos);
            model->board.clear(cursor.selectedCell.pos + rMoveFw);
            return turns::attackTurn();
        } return turns::nothingTurn();
    } else if(_attackIsAvailiable(model, cursor, rMoveBw, rAttackBw)) {
        if(_attackIsPossible(model, cursor, rMoveBw, rAttackBw)) {
            model->board.clear(cursor.selectedCell.pos);
            model->board.clear(cursor.selectedCell.pos + rMoveBw);
            return turns::attackTurn();
        } return turns::nothingTurn();
    } else if(_attackIsAvailiable(model, cursor, lMoveFw, lAttackFw)) {
        if(_attackIsPossible(model, cursor, lMoveFw, lAttackFw)) {
            model->board.clear(cursor.selectedCell.pos);
            model->board.clear(cursor.selectedCell.pos + lMoveFw);
            return turns::attackTurn();
        } return turns::nothingTurn();
    } else if(_attackIsAvailiable(model, cursor, lMoveBw, lAttackBw)) {
        if(_attackIsPossible(model, cursor, lMoveBw, lAttackBw)) {
            model->board.clear(cursor.selectedCell.pos);
            model->board.clear(cursor.selectedCell.pos + lMoveBw);
            return turns::attackTurn();
        } return turns::nothingTurn();
    } else if(_moveIsPossible(model, cursor, rMoveFw) ||
              _moveIsPossible(model, cursor, rMoveBw) ||
              _moveIsPossible(model, cursor, lMoveFw) ||
              _moveIsPossible(model, cursor, lMoveBw))
    {
        model->board.clear(cursor.selectedCell.pos);
        return turns::moveTurn();
    } return turns::nothingTurn();
}

bool Queen::attackIsAvailiable(GameModel *model, const Cursor &cursor)
{
    return _attackIsAvailiable(model, cursor, rMoveFw, rAttackFw) || _attackIsAvailiable(model, cursor, lMoveFw, lAttackFw) ||
           _attackIsAvailiable(model, cursor, rMoveBw, rAttackBw) || _attackIsAvailiable(model, cursor, lMoveBw, lAttackBw);
}
