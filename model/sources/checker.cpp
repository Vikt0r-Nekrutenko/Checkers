#include "checker.hpp"
#include "gamemodel.hpp"

WChecker::WChecker()
{
    directions.push_back({{+1,+1},{+2,+2}}); // right move dir & right attack dir
    directions.push_back({{-1,+1},{-2,+2}}); // left move dir && right attack dir
}

bool WChecker::isTransformPossible(GameModel *model)
{
    return model->board.isInBlackZone(model->cursor.selectableCell.pos);
}

BoardCell *WChecker::getTransformPiece()
{
    return GameBoard::whiteQueen();
}

BChecker::BChecker()
{
    directions.push_back({{+1,-1},{+2,-2}}); // right move dir & right attack dir
    directions.push_back({{-1,-1},{-2,-2}}); // left move dir && right attack dir
}

bool BChecker::isTransformPossible(GameModel *model)
{
    return model->board.isInWhiteZone(model->cursor.selectableCell.pos);
}

BoardCell *BChecker::getTransformPiece()
{
    return GameBoard::blackQueen();
}
