#include "gameview.hpp"
#include "gamemodel.hpp"

stf::Vec2d pzero{0,0};

auto cell = [](const Sprite& board, const GameModel *gameModel, const stf::Vec2d pos) -> const Vec2d {
    return pzero + board.markers().at(gameModel->board.Size.x * pos.y + pos.x + 1);
};

auto cell1 = [](const Sprite& board, size_t i) -> const Vec2d {
    return pzero + board.markers().at(i + 1);
};

GameView::GameView(BaseModel *model, bool resetTheModel)
    : IView(model)
{
    if(resetTheModel) {
        static_cast<GameModel*>(m_model)->reset();
    }
}

IView *GameView::mouseEventsHandler(const MouseRecord &mr)
{
    Vec2d mp(mr.x, mr.y);
    Vec2d dif = (m_board.Size()-1) / 8;
    Vec2d pos = (mp - pzero) / dif - Vec2d(1,1);

    if(pos.x >= 0 && pos.y >= 0 && pos.x < static_cast<GameModel*>(m_model)->board.Size.x && pos.y < static_cast<GameModel*>(m_model)->board.Size.y)
        static_cast<GameModel*>(m_model)->cursor.selectableCell.pos = pos;
    return m_model->mouseEventsHandler(this, mr);
}

void GameView::drawPlayersScore(Renderer &renderer, GameModel *gameModel) const
{
    renderer.draw(pzero - Vec2d(-2, +2), "Player 'Black': %d", gameModel->bPieceCount);
    renderer.draw(pzero + Vec2d(+21, -2), "Player 'White': %d", gameModel->wPieceCount);
}

void GameView::show(Renderer &renderer)
{
    m_board.show(renderer, true);
    pzero = renderer.Size / 2 - m_board.Size() / 2;

    GameModel *gameModel = static_cast<GameModel*>(m_model);

    renderer.drawText(pzero + m_board.markers().at(0), gameModel->player->name().c_str());

    drawPlayersScore(renderer, gameModel);

    // draw board
    for(size_t i = 0; i < gameModel->board.board.size(); ++i) {
        renderer.drawPixel(cell1(m_board, i), gameModel->board.board[i]->view(), gameModel->board.board[i]->color());
    }

    // draw cursors
    if(gameModel->cursor.selectedCell.cell != GameBoard::emptyCell())
    {
        renderer.drawPixel(cell(m_board, gameModel, gameModel->cursor.selectedCell.pos) - stf::Vec2d(1,0), '<');
        renderer.drawPixel(cell(m_board, gameModel, gameModel->cursor.selectedCell.pos) - stf::Vec2d(-1,0), '>');
    }

    renderer.drawPixel(cell(m_board, gameModel, gameModel->cursor.selectableCell.pos) - stf::Vec2d(1,0), '[');
    renderer.drawPixel(cell(m_board, gameModel, gameModel->cursor.selectableCell.pos) - stf::Vec2d(-1,0), ']');
}
