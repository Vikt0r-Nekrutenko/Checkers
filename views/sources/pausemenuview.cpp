#include "pausemenuview.hpp"
#include "gameview.hpp"
#include "storyview.hpp"
#include "closeview.hpp"
#include "gamemodel.hpp"

PauseMenuView::PauseMenuView(GameModel* model)
    : MenuView(model, "sprs/pause_menu.spr") { }

void PauseMenuView::show(Renderer &renderer)
{
    IView::show(renderer);
    Vec2d pzero = renderer.Size / 2 - m_smenu.Size() / 2;
    m_logo.show(renderer, true, {0,0}, {0, -m_logo.Size().y - 4});
    m_smenu.show(renderer, true);

    renderer.drawPixel(pzero + m_smenu.markers().at(m_cursor * 2), '>');
    renderer.drawPixel(pzero + m_smenu.markers().at(m_cursor * 2 + 1), '<');
}

IView* PauseMenuView::menuSelectConfirm()
{
  switch(m_cursor)
  {
    case 0: return new GameView(static_cast<GameModel*>(m_model), true);
    case 1: return new GameView(static_cast<GameModel*>(m_model), false);
    case 2:
      try {
        static_cast<GameModel*>(m_model)->saves.save();
      } catch(...) { }
      return this;
    case 3:
      try {
        static_cast<GameModel*>(m_model)->saves.load();
      } catch(...) { }
      return this;
    case 4: return new StoryView(static_cast<GameModel*>(m_model), this);
    case 5: return new CloseView(static_cast<GameModel*>(m_model));
  }
  return this;
}
