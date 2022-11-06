#ifndef STORYVIEW_HPP
#define STORYVIEW_HPP

#include "sprite.hpp"
#include "iview.hpp"

using namespace stf;
using namespace stf::smv;

class GameModel;

class StoryView : public IView
{
  public:

  StoryView(GameModel* model, IView* sender);
  ~StoryView() override = default;
  void show(Renderer& renderer) final;
  IView* keyEventsHandler(const int key) override;
  IView* update(const float) override { return this; }

  protected:

  Sprite m_stats = Sprite("sprs/stats.spr");
  IView* m_sender;
};

#endif // STORYVIEW_HPP
