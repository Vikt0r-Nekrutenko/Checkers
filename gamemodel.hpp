#ifndef GAMEMODEL_HPP
#define GAMEMODEL_HPP

#include "imodel.hpp"
#include "gameboard.hpp"

class GameModel : public stf::smv::BaseModel
{
public:
    GameModel() = default;

    stf::smv::IView* put(stf::smv::IView *sender)
    { return sender; }

    stf::smv::IView *keyEventsHandler(stf::smv::IView *sender, const int key) final
    { return sender; }

    GameBoard board = GameBoard();
};

#endif // GAMEMODEL_HPP
