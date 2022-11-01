#include <window.hpp>
#include <iostream>

#include "gamemodel.hpp"
#include "gameview.hpp"

class Game : public stf::Window
{
    int n = 0;
    GameModel gameModel = GameModel();
    GameView gameView = GameView(&gameModel);
    stf::smv::IView *currentView = &gameView;

    bool onUpdate(const float) final
    {
        if(currentView == nullptr)
            return false;
        currentView->show(renderer);
        return n++ == 1 ? false : true;
//        return true;
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
    BoardCell::_cellAllocator.info();
}
