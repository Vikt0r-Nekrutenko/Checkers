#include "gameview.hpp"
#include "gamemodel.hpp"
#include "boardcell.hpp"

GameView::GameView(GameModel *model)
    : stf::smv::IView(model)
{

}

void GameView::show(stf::Renderer &renderer)
{
    GameModel *model = static_cast<GameModel*>(m_model);
    for(int y = 0; y < model->board.Size.y; ++y) {
        for(int x = 0; x < model->board.Size.x; ++x) {
            auto c = model->board[{x,y}];
            if(c == GameBoard::emptyCell())
                renderer.drawPixel({x*3+1, y+2}, '_');
            else
                renderer.drawPixel({x*3+1, y+2}, c->view(), c->color());
        }
    }

    if(model->cursor.selectedCell.cell != GameBoard::emptyCell())
    {
        renderer.drawPixel({model->cursor.selectedCell.pos.x * 3 + 0, model->cursor.selectedCell.pos.y + 2}, '<');
        renderer.drawPixel({model->cursor.selectedCell.pos.x * 3 + 2, model->cursor.selectedCell.pos.y + 2}, '>');
    }

    renderer.drawPixel({model->cursor.selectableCell.pos.x * 3 + 0, model->cursor.selectableCell.pos.y + 2}, '[');
    renderer.drawPixel({model->cursor.selectableCell.pos.x * 3 + 2, model->cursor.selectableCell.pos.y + 2}, ']');

    model->player->color() == stf::ColorTable::Red
            ? renderer.drawText({0,10}, "Black player turn")
            : renderer.drawText({0,10}, "White player turn");
    renderer.drawText({0, 11}, model->lastTurn->log().c_str());
}
