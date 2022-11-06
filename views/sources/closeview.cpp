#include "closeview.hpp"
#include "gamemodel.hpp"

CloseView::CloseView(GameModel* model)
  : IView(model) {}

bool CloseView::isContinue() const { return false; }

