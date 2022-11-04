#include "gamemodel.hpp"

GameModel::GameModel() : stf::smv::BaseModel()
{
//    board.place(1, GameBoard::whiteChecker());
//    board.place(3, GameBoard::whiteChecker());
//    board.place(5, GameBoard::whiteChecker());
//    board.place(7, GameBoard::whiteChecker());

//    board.place(8, GameBoard::whiteChecker());
//    board.place(10, GameBoard::whiteChecker());
//    board.place(12, GameBoard::whiteChecker());
//    board.place(14, GameBoard::whiteChecker());

//    board.place(17, GameBoard::whiteChecker());
//    board.place(19, GameBoard::whiteChecker());
//    board.place(21, GameBoard::whiteChecker());
//    board.place(23, GameBoard::whiteChecker());

//    board.place(39+1, GameBoard::blackChecker());
//    board.place(39+3, GameBoard::blackChecker());
//    board.place(39+5, GameBoard::blackChecker());
//    board.place(39+7, GameBoard::blackChecker());

//    board.place(39+10, GameBoard::blackChecker());
//    board.place(39+12, GameBoard::blackChecker());
//    board.place(39+14, GameBoard::blackChecker());
//    board.place(39+16, GameBoard::blackChecker());

//    board.place(39+17, GameBoard::blackChecker());
//    board.place(39+19, GameBoard::blackChecker());
//    board.place(39+21, GameBoard::blackChecker());
//    board.place(39+23, GameBoard::blackChecker());

    board.place({4,4}, GameBoard::blackChecker());
    board.place({2,4}, GameBoard::blackChecker());

    board.place({3,1}, GameBoard::whiteChecker());
    board.place({1,3}, GameBoard::whiteChecker());
    board.place({3,3}, GameBoard::whiteChecker());
}

BoardCell *GameModel::opponent() const {
    if (player == GameBoard::blackPlayer())
        return GameBoard::whitePlayer();
    else
        return GameBoard::blackPlayer();
}

void GameModel::placementAfterHandling()
{
    if(board.objectIsInBlackZone(cursor.selectableCell.pos) && cursor.selectableCell.cell == GameBoard::whiteChecker())
        board.place(cursor.selectableCell.pos, GameBoard::whiteQueen());
    else if(board.objectIsInWhiteZone(cursor.selectableCell.pos) && cursor.selectableCell.cell == GameBoard::blackChecker())
        board.place(cursor.selectableCell.pos, GameBoard::blackQueen());
    else
        board.place(cursor.selectableCell.pos, cursor.selectableCell.cell);
}

stf::smv::IView *GameModel::put(stf::smv::IView *sender)
{
    BoardCell *cell = board.getSelectableCell(cursor);

    if(cell->color() == player->color())
        cursor.select(cell);
    else {
        cell = board.getSelectedCell(cursor);
        GameTurn *placementResult = cell->getNextTurn(this, cursor);
        placementResult->turnHandler(this);
    }

    return sender;
}

stf::smv::IView *GameModel::select(stf::smv::IView *sender)
{
    return sender;
}

stf::smv::IView *GameModel::keyEventsHandler(stf::smv::IView *sender, const int key)
{
    switch (key)
    {
    case 'w': if(cursor.selectableCell.pos.y > 0) cursor.selectableCell.pos -= stf::Vec2d(0,1); break;
    case 'a': if(cursor.selectableCell.pos.x > 0) cursor.selectableCell.pos -= stf::Vec2d(1,0); break;
    case 's': if(cursor.selectableCell.pos.y < board.Size.y-1) cursor.selectableCell.pos += stf::Vec2d(0,1); break;
    case 'd': if(cursor.selectableCell.pos.x < board.Size.x-1) cursor.selectableCell.pos += stf::Vec2d(1,0); break;
    case 'q': return nullptr;
    case ' ':
        return put(sender);
    }
    return sender;
}
