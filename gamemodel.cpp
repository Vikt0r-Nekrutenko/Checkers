#include "gamemodel.hpp"

GameModel::GameModel() : stf::smv::BaseModel()
{
    board.place({2,0}, GameBoard::whiteChecker());
    board.place({3,1}, GameBoard::blackChecker());
    board.place({5,3}, GameBoard::blackChecker());
}

BoardCell *GameModel::opponent() const {
    if (player == GameBoard::blackPlayer())
        return GameBoard::whitePlayer();
    else
        return GameBoard::blackPlayer();
}

stf::smv::IView *GameModel::put(stf::smv::IView *sender)
{
    BoardCell *cell = board.getSelectableCell(cursor);

    if(!cursor.cursorIsEmpty() && cursor.selectableCell.cell->onPlacementHandler(this, cursor)) {
        if(board.objectIsInBlackZone(cursor.selectableCell.pos))
            board.place(cursor.selectableCell.pos, GameBoard::whiteQueen());
        else if(board.objectIsInWhiteZone(cursor.selectableCell.pos))
            board.place(cursor.selectableCell.pos, GameBoard::blackQueen());
        else
            board.place(cursor.selectableCell.pos, cursor.selectableCell.cell);
        cursor.reset();
        player = opponent();
    } else if(cell->color() == player->color()) {
        cursor.select(cell);
    }

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
    case ' ': return put(sender);
    }
    return sender;
}
