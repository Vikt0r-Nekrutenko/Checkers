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

class GameModel : public stf::smv::BaseModel
{

public:
    ~GameModel() override;
    BoardCell *opponent() const;
    stf::smv::IView* put(stf::smv::IView *sender);
    stf::smv::IView *keyEventsHandler(stf::smv::IView *sender, const int key) final;
    void reset();
    bool exitWithoutSave = true;

    GameSaveModel saves = GameSaveModel(this);

    GameBoard board = GameBoard();
    Cursor cursor = Cursor();
    BoardCell *player = GameBoard::blackPlayer();
    GameTurn *lastTurn = turns::nothingTurn();
};

#endif // GAMEMODEL_HPP
