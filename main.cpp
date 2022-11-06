#include <window.hpp>
#include <iostream>

#include "gamemodel.hpp"
#include "gameview.hpp"
#include "menuview.hpp"
#include "pausemenuview.hpp"

class Game : public stf::Window
{
    int n = 0;
    GameModel gameModel = GameModel();
    GameView gameView = GameView(&gameModel);
    PauseMenuView menuView = PauseMenuView(&gameModel);
    stf::smv::IView *currentView = &menuView;

    bool onUpdate(const float) final
    {
        currentView->show(renderer);
//        return n++ == 1 ? false : true;
        return currentView->isContinue();
    }

    void keyEvents(const int key) final
    {
        currentView = currentView->keyEventsHandler(key);
    }

    void mouseEvents(const stf::MouseRecord&) final
    {

    }
};

int main()
{
    Game().run();
//    BoardCell::_cellAllocator.info();
}
