#ifndef GAMEMODEL_HPP
#define GAMEMODEL_HPP

#include "imodel.hpp"

class GameModel : public stf::smv::BaseModel
{
public:
    GameModel();

    stf::smv::IView* put(stf::smv::IView *sender);

    stf::smv::IView *keyEventsHandler(stf::smv::IView *sender, const int key) final;
};

#endif // GAMEMODEL_HPP
