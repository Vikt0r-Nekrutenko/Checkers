#include "gamemodel.hpp"
#include <algorithm>

GameModel::GameModel() : stf::smv::BaseModel()
{
    int n = 0;
    BoardCell *cell = GameBoard::whiteChecker();

    for(int y = 0; y < 8; ++y) {
        for(int x = 1; x < 8; x += 2) {
            if(y == 3) {
                y = 5;
                cell = GameBoard::blackChecker();
            }
            int indx = 8 * y + (x - n);
            board.place(indx, cell);
        }
        n ^= 1;
    }
}

BoardCell *GameModel::opponent() const {
    if (player == GameBoard::blackPlayer())
        return GameBoard::whitePlayer();
    else
        return GameBoard::blackPlayer();
}

auto areSelectedCellValid = [](const std::vector<stf::Vec2d>& possibleAttacks, const Cursor& cursor) {
    if(possibleAttacks.empty())
        return true;

    for(auto i : possibleAttacks) {
        if(i == cursor.selectedCell.pos) {
            return true;
        }
    }
    return false;
};

stf::smv::IView *GameModel::put(stf::smv::IView *sender)
{
    BoardCell *cell = board.getSelectableCell(cursor);
    auto possibleAttacks = board.findPossibleAttacks(this);

    if(cell->color() == player->color()) {
        cursor.select(cell);
    } else if(areSelectedCellValid(possibleAttacks, cursor)) {
        board.getSelectedCell(cursor)->takeNextTurn(this, cursor);
    }

    return sender;
}

stf::smv::IView *GameModel::keyEventsHandler(stf::smv::IView *sender, const int key)
{
    switch (key)
    {
    case 'w':
        if(cursor.selectableCell.pos.y > 0)
            cursor.selectableCell.pos.y -= 1;
        else
            cursor.selectableCell.pos.y = board.Size.y - 1;
        break;

    case 'a':
        if(cursor.selectableCell.pos.x > 0)
            cursor.selectableCell.pos.x -= 1;
        else
            cursor.selectableCell.pos.x = board.Size.x - 1;
        break;

    case 's':
        if(cursor.selectableCell.pos.y < board.Size.y-1)
            cursor.selectableCell.pos.y += 1;
        else
            cursor.selectableCell.pos.y = 0;
        break;

    case 'd':
        if(cursor.selectableCell.pos.x < board.Size.x-1)
            cursor.selectableCell.pos.x += 1;
        else
            cursor.selectableCell.pos.x = 0;
        break;

    case 'q':
        return nullptr;

    case ' ':
        return put(sender);
    }
    return sender;
}
