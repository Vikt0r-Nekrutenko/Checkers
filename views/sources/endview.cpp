#include "endview.hpp"
#include "gamemodel.hpp"
#include "menuview.hpp"

EndView::EndView(GameModel* model)
  : IView(model)
{
    model->results.save();
}

void EndView::show(Renderer& renderer)
{
    m_end.show(renderer, true);
    if(!static_cast<GameModel*>(m_model)->bPieceCount) {
        renderer.drawText(renderer.Size / 2 - m_end.Size() / 2 + m_end.markers().at(0), "White");
    } else if(!static_cast<GameModel*>(m_model)->wPieceCount) {
        renderer.drawText(renderer.Size / 2 - m_end.Size() / 2 + m_end.markers().at(0), "Black");
    }
}

IView* EndView::keyEventsHandler(const int)
{
  static_cast<GameModel*>(m_model)->reset();
  return new MenuView(static_cast<GameModel*>(m_model));
}
