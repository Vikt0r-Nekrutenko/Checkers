#include "checker.hpp"
#include "gamemodel.hpp"

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
