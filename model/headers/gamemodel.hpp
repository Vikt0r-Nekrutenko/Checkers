#ifndef GAMEMODEL_HPP
#define GAMEMODEL_HPP

#include "imodel.hpp"
#include "gameboard.hpp"
#include "cursor.hpp"

class GameModel : public stf::smv::BaseModel
{
public:

    BoardCell *opponent() const;
    stf::smv::IView* put(stf::smv::IView *sender);
    stf::smv::IView *keyEventsHandler(stf::smv::IView *sender, const int key) final;
    void reset();

    GameBoard board = GameBoard();
    Cursor cursor = Cursor();
    BoardCell *player = GameBoard::blackPlayer();
    GameTurn *lastTurn = turns::nothingTurn();
};

#endif // GAMEMODEL_HPP
