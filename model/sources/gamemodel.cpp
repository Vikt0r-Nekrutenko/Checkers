#include "gamemodel.hpp"
#include <algorithm>

GameModel::~GameModel()
{
    try {
        while(exitWithoutSave)
            saves.pop<GameSaveModel>();
    } catch(const std::string& ex) { }
}

BoardCell *GameModel::opponent() const {
    if (player == GameBoard::blackPlayer())
        return GameBoard::whitePlayer();
    else
        return GameBoard::blackPlayer();
}

auto areSelectedCellValid = [](const std::vector<stf::Vec2d>& possibleAttacks, const Cursor& cursor) -> GameTurn * {
    if(possibleAttacks.empty())
        return turns::nothingTurn();

    for(auto i : possibleAttacks) {
        if(i == cursor.selectedCell.pos) {
            return turns::nothingTurn();
        }
    }
    return turns::mustBeAttackingTurn();
};

stf::smv::IView *GameModel::put(stf::smv::IView *sender)
{
    BoardCell *cell = board.getSelectableCell(cursor);
    auto possibleAttacks = board.findPossibleAttacks(this);
    lastTurn = areSelectedCellValid(possibleAttacks, cursor);

    if(cell->color() == player->color()) {
        cursor.select(cell);
    } else if(lastTurn == turns::nothingTurn()) {
        lastTurn = board.getSelectedCell(cursor)->takeNextTurn(this, cursor);
    } else {
        cursor.reset();
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

    case 'z':
        cursor.reset();
        saves.load();
        break;

    case 'x':
        saves.save();
        break;

    case 'q':
        return nullptr;

    case ' ':
        return put(sender);
    }
    return sender;
}

void GameModel::reset()
{
    cursor.reset();
    board = GameBoard();
    player = GameBoard::blackPlayer();
    lastTurn = turns::nothingTurn();
}

GameSaveModel::GameSaveModel(GameModel *model)
    : stf::sdb::StackModel("checkers_saves.sdb"), mModel(model)
{}

void GameSaveModel::save()
{
    for(size_t i = 0; i < mModel->board.board.size(); ++i) {
        board[i] = mModel->board.board.at(i)->uniqueNumericView();
    }
    player = mModel->player->uniqueNumericView();

    push<GameSaveModel>();
}

void GameSaveModel::load()
{
    pop<GameSaveModel>();

    for(size_t i = 0; i < mModel->board.board.size(); ++i) {
        int uniqueIndx = board[i];
        mModel->board.place(i, GameBoard::restoreFromIntView(uniqueIndx));
    }
    mModel->player = GameBoard().restoreFromIntView(player());

    push<GameSaveModel>();
}
