#ifndef MENUVIEW_HPP
#define MENUVIEW_HPP

#include "sprite.hpp"
#include "iview.hpp"

using namespace stf;
using namespace stf::smv;

class GameModel;

class MenuView : public IView
{
  public:

  MenuView(GameModel* model);
  ~MenuView() override = default;
  void show(Renderer& renderer) override;
  IView* keyEventsHandler(const int key) override;
  IView* update(const float) override { return this; }

  protected:

  MenuView(GameModel* model, const char* sprite);

  Sprite m_logo;
  Sprite m_smenu;
  int m_cursor = 0;
  int m_menuItemsCount;

  virtual IView* menuSelectConfirm();
};

#endif // MENUVIEW_HPP
