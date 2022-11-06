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
        if(currentView != nullptr)
            try {
                stf::sdb::Model::QueryResult *qres = gameModel.results.all();
                int k = 0;
                for(auto it = qres->begin(); it != qres->end(); ++it) {
                  GameResultModel* info = qres->get<GameResultModel>(*it);

                  BoardCell *winner = GameBoard::restoreFromIntView(info->winner());
                renderer.draw(stf::Vec2d{0,12}+stf::Vec2d(0, k++ * 2), "%s Player has won: \'%s\'. Black: %d | White: %d",
                              info->gameTime().asString().c_str(),
                              winner == GameBoard::blackPlayer() ? "Black" : "White",
                              info->bWins(),
                              info->wWins());
                }
            } catch(...) {
                renderer.drawText({0,12}, "Nothing much");
            }

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
