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
            renderer.drawPixel({x*3+1, y+2}, model->board[{x,y}]->view());
        }
    }
//    if(*model->m_cursor.selectedCell.cell != EmptyCheckerCell()){
//        renderer.drawPixel({model->m_cursor.selectedCell.pos.x * 3 + 0, model->m_cursor.selectedCell.pos.y + 2}, '{');
//        renderer.drawPixel({model->m_cursor.selectedCell.pos.x * 3 + 2, model->m_cursor.selectedCell.pos.y + 2}, '}');
//    }

//    renderer.drawPixel({model->m_cursor.selectableCell.pos.x * 3 + 0, model->m_cursor.selectableCell.pos.y + 2}, '[');
//    renderer.drawPixel({model->m_cursor.selectableCell.pos.x * 3 + 2, model->m_cursor.selectableCell.pos.y + 2}, ']');

//    renderer.drawPixel({0,10}, model->player->view());
}
