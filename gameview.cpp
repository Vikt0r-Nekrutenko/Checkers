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
                renderer.drawPixel({x*3+1, y+2}, c->view());
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

    if(model->board.objectIsInBlackZone(model->cursor.selectableCell.pos))
        renderer.drawText({0,11}, "In black zone.");
    else if(model->board.objectIsInWhiteZone(model->cursor.selectableCell.pos))
        renderer.drawText({0,11}, "In white zone.");

    renderer.draw({0,12}, "Exception counter : %d", model->exCount);
    if(model->isSelect) renderer.draw({0, 14}, "SELECT");
                   else renderer.draw({0, 14}, "PUT!!!");
}
