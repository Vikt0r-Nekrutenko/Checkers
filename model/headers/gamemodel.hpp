#ifndef GAMEMODEL_HPP
#define GAMEMODEL_HPP

#include "imodel.hpp"
#include "gameboard.hpp"
#include "cursor.hpp"
#include "stackmodel.hpp"
#include "fields.hpp"

class GameSaveModel : public stf::sdb::StackModel
{
public:
    GameSaveModel(GameModel *model);

    void save();
    void load();

    stf::sdb::IntVecField board = stf::sdb::IntVecField(this, BOARD_W * BOARD_H);
    stf::sdb::IntField    player = stf::sdb::IntField(this);

    GameModel *mModel;
};

class GameResultModel : public stf::sdb::Model
{
public:
    GameResultModel();

    void gameOverHandler(int winner, const stf::Vec2d& wins);;

    stf::sdb::DateTimeField gameTime = stf::sdb::DateTimeField(this);
    stf::sdb::IntField winner = stf::sdb::IntField(this);
    stf::sdb::IntField wWins = stf::sdb::IntField(this);
    stf::sdb::IntField bWins = stf::sdb::IntField(this);
};

class GameModel : public stf::smv::BaseModel
{

public:
    GameModel();
    Player *opponent() const;
    stf::smv::IView* put(stf::smv::IView *sender);
    stf::smv::IView *keyEventsHandler(stf::smv::IView *sender, const int key) final;
    void reset();
    bool exitWithoutSave = true;

    GameSaveModel saves = GameSaveModel(this);
    GameResultModel results = GameResultModel();

    GameBoard board = GameBoard();
    Cursor cursor = Cursor();
    Player *player = GameBoard::blackPlayer();
    GameTurn *lastTurn = turns::nothingTurn();

    uint8_t bPieceCount = 0;
    uint8_t wPieceCount = 0;
};

#endif // GAMEMODEL_HPP
