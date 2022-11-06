#include "storyview.hpp"
#include "gamemodel.hpp"

StoryView::StoryView(GameModel* model, IView* sender)
  : IView(model), m_sender(sender) {}

void StoryView::show(Renderer& renderer)
{
  const std::string s = Time(nullptr).asString() + std::string(" Player has won: \'s\'.");
  Vec2d zerop = renderer.Size / 2 - Vec2d(s.length()/2, 0);
  Vec2d statsp = renderer.Size / 2 - Vec2d(m_stats.Size().x / 2, m_stats.Size().y*2);
  m_stats.show(renderer, false, statsp);

  try {
      stf::sdb::Model::QueryResult *qres = static_cast<GameModel*>(m_model)->results.all();
      int k = 0;
      for(auto it = qres->begin(); it != qres->end(); ++it) {
          GameResultModel* info = qres->get<GameResultModel>(*it);

          BoardCell *winner = UniqueNumericCell::restoreFromIntView(info->winner());
          renderer.draw(zerop + stf::Vec2d(0, k++ * 2), "%s Player has won: \'%s\'",
                        info->gameTime().asString().c_str(),
                        winner == GameBoard::blackPlayer() ? "Black" : "White");
    }

    renderer.drawNumber(statsp + m_stats.markers().at(0), (int)qres->size());

    GameResultModel* linfo = qres->get<GameResultModel>(qres->front());
    renderer.drawNumber(statsp + m_stats.markers().at(1), (int)linfo->bWins());
    renderer.draw(statsp + m_stats.markers().at(3), "%d%c", int(double(linfo->bWins()) / (double)qres->size() * 100), '%');

    renderer.drawNumber(statsp + m_stats.markers().at(2), (int)linfo->wWins());
    renderer.draw(statsp + m_stats.markers().at(4), "%d%c", int(double(linfo->wWins()) / double(qres->size()) * 100), '%');
  } catch(const std::string& ex) {
    std::string s("There are no results here yet");
    Vec2d p = renderer.Size / 2 - Vec2d(s.length() / 2, 0);
    renderer.drawText(p, s.c_str());
  }
}

IView* StoryView::keyEventsHandler(const int)
{
  return m_sender;
}
