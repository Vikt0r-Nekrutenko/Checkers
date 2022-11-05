#ifndef GAMEVIEW_HPP
#define GAMEVIEW_HPP

#include <iview.hpp>

class GameModel;

class GameView : public stf::smv::IView
{
public:
    GameView(GameModel *model);

    void show(stf::Renderer &renderer) final;
};

#endif // GAMEVIEW_HPP
