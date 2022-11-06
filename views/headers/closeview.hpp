#pragma once
#ifndef CLOSEVIEW_HPP
#define CLOSEVIEW_HPP

#include "iview.hpp"

using namespace stf;
using namespace stf::smv;

class GameModel;

class CloseView : public IView
{
  public:

  CloseView(GameModel* model);
  ~CloseView() override = default;
  void show(Renderer&) final { };
  bool isContinue() const final;
  IView* update(const float) override { return this; }
};


#endif // CLOSEVIEW_HPP
