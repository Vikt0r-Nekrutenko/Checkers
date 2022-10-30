#include <window.hpp>
#include <iview.hpp>
#include <imodel.hpp>

stf::sdb::DynamicFieldsAllocator _cellAlloc;

class BoardCell
{
public:
    virtual uint8_t view() const { return 'e'; }

    void* operator new(size_t size)
    {
      void* ptr;
      if(offset == 0xffffffff) {
        ptr = _cellAlloc.allocate(size);
      } else {
        ptr = _cellAlloc.mem() + offset;
      }
      return ptr;
    }

    static uint32_t offset;
};

uint32_t BoardCell::offset = 0xffffffff;

class CheckerCell : public BoardCell
{
public:
    uint8_t view() const final { return 'o'; }
};

class QueenCheckerCell : public BoardCell
{
public:
    uint8_t view() const final { return 'O'; }
};

struct Selector
{
    stf::Vec2d pos { 0, 0 };
    BoardCell *cell = new BoardCell;
};

struct Cursor
{
    Selector selectorCell;
    Selector destinationCell;
};

class GameModel : public stf::smv::BaseModel
{
public:
    GameModel() : stf::smv::BaseModel() {
        for(auto it = board.begin(); it != board.end(); ++it) {
            *it = new BoardCell();
        }
        board.at(0) = new CheckerCell();
        board.at(1) = new QueenCheckerCell();
    }

    BoardCell* get(const stf::Vec2d& pos) { return board.at(Size.x * pos.y + pos.x); }

    stf::Vec2d Size { 8, 8 };
    std::vector<BoardCell*> board = std::vector<BoardCell*>(Size.x * Size.y);
};

class GameView : public stf::smv::IView
{
public:
    GameView(GameModel *model)
        : IView(model)
    {}

    void show(stf::Renderer &renderer) final
    {
        GameModel *model = static_cast<GameModel*>(m_model);
        for(int y = 0; y < model->Size.y; ++y) {
            for(int x = 0; x < model->Size.x; ++x) {
                renderer.drawPixel({x,y+2}, model->get({x,y})->view());
            }
        }
    }
};

class Game : public stf::Window
{
    GameModel gameModel = GameModel();
    GameView gameView = GameView(&gameModel);
    stf::smv::IView *currentView = &gameView;

    bool onUpdate(const float) final
    {
        currentView->show(renderer);
        return true;
    }

    void keyEvents(const int key) final
    {

    }

    void mouseEvents(const stf::MouseRecord& mr) final
    {

    }
};

int main()
{
    return Game().run();
}
