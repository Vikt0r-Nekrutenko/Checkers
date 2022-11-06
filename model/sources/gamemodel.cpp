#include "gamemodel.hpp"
#include "endview.hpp"
#include "pausemenuview.hpp"
#include <algorithm>

GameModel::GameModel()
{
    try {
        results.load(results.header().size - 1);
    } catch(...) { }
}

Player *GameModel::opponent() const {
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

    stf::Vec2d pieceCount = board.calculatePieceCount();
    bPieceCount = pieceCount.x;
    wPieceCount = pieceCount.y;

    if(!bPieceCount) {
        results.gameOverHandler(GameBoard::whitePlayer()->uniqueNumericView(), {1,0});

        results.save();
        return new EndView(this);
    } else if(!wPieceCount) {
        results.gameOverHandler(GameBoard::blackPlayer()->uniqueNumericView(), {0,1});

        results.save();
        return new EndView(this);
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
        return new PauseMenuView(this);

    case ' ':
        return put(sender);
    }
    return sender;
}

IView *GameModel::mouseEventsHandler(stf::smv::IView *sender, const stf::MouseRecord &mr)
{
    if(mr.type == MouseInputType::leftPressed) {
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
    : stf::sdb::StackModel("checkers_saves.sdb"), mModel(model) {}

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
        mModel->board.place(i, UniqueNumericCell::restoreFromIntView(uniqueIndx));
    }
    mModel->player = UniqueNumericCell::restorePlayerIntView(player());
}

GameResultModel::GameResultModel()
    : stf::sdb::Model("checkers_results.sdb") {}

void GameResultModel::gameOverHandler(int winner, const stf::Vec2d &wins){
    gameTime = stf::Time(nullptr);
    wWins = wWins() + wins.x;
    bWins = bWins() + wins.y;
    this->winner = winner;
}
