#include <window.hpp>
#include <iview.hpp>
#include <imodel.hpp>
#include <iostream>

stf::sdb::DynamicFieldsAllocator _cellAlloc;

struct Cursor;

class BoardCell
{
public:
    inline virtual uint8_t view() const { return 'e'; }
    virtual bool onPlacementHandler(std::vector<BoardCell*>& board, Cursor& cursor);

    void* operator new(size_t size) { return _cellAlloc.allocate(size); }
    bool operator !=(const BoardCell& cell) const { return view() != cell.view(); }
    bool operator ==(const BoardCell& cell) const { return view() != cell.view(); }
};

class CheckerCell : public BoardCell
{
public:
    inline uint8_t view() const final { return 'o'; }
};

class QueenCheckerCell : public BoardCell
{
public:
    inline uint8_t view() const final { return 'O'; }
};

template<typename T> class BoardCellCreator
{
public:
    T *create() { return cell = cell == nullptr ? new T() : cell; }
    T *cell = nullptr;
};

class BoardCellFactory
{
public:
    static BoardCellCreator<BoardCell> boardCell;
    static BoardCellCreator<CheckerCell> checkerCell;
    static BoardCellCreator<QueenCheckerCell> queenCheckerCell;
};

BoardCellCreator<BoardCell> BoardCellFactory::boardCell = BoardCellCreator<BoardCell>();
BoardCellCreator<CheckerCell> BoardCellFactory::checkerCell = BoardCellCreator<CheckerCell>();
BoardCellCreator<QueenCheckerCell> BoardCellFactory::queenCheckerCell = BoardCellCreator<QueenCheckerCell>();

struct Selector
{
    stf::Vec2d pos { 0, 0 };
    BoardCell *cell = BoardCellFactory::boardCell.create();
};

struct Cursor
{
    Selector selectorCell;
    Selector destinationCell;
};

bool BoardCell::onPlacementHandler(std::vector<BoardCell*>& board, Cursor& cursor)
{
    Selector &sc = cursor.selectorCell;
    Selector &dc = cursor.destinationCell;
    BoardCell* cell = board.at(8 * cursor.selectorCell.pos.y + cursor.selectorCell.pos.x);
    if(dc.pos.diff(sc.pos) <= 1.5f) {
        return true;
    }
    return false;
}

class GameModel : public stf::smv::BaseModel
{
public:

    GameModel() : stf::smv::BaseModel() {
        for(auto it = board.begin(); it != board.end(); ++it) {
            *it = BoardCellFactory::boardCell.create();
        }
        board.at(0) = BoardCellFactory::checkerCell.create();
        board.at(1) = BoardCellFactory::queenCheckerCell.create();
    }

    BoardCell* get(const stf::Vec2d& pos) { return board.at(Size.x * pos.y + pos.x); }
    void place(const Selector& s) { board.at(Size.x * s.pos.y + s.pos.x) = s.cell; }

    stf::smv::IView *keyEventsHandler(stf::smv::IView *sender, const int key)
    {
        switch (key)
        {
        case 'w': if(m_cursor.selectorCell.pos.y > 0) m_cursor.selectorCell.pos -= stf::Vec2d(0,1); break;
        case 'a': if(m_cursor.selectorCell.pos.x > 0) m_cursor.selectorCell.pos -= stf::Vec2d(1,0); break;
        case 's': if(m_cursor.selectorCell.pos.y < Size.y-1) m_cursor.selectorCell.pos += stf::Vec2d(0,1); break;
        case 'd': if(m_cursor.selectorCell.pos.x < Size.x-1) m_cursor.selectorCell.pos += stf::Vec2d(1,0); break;
        case 'q': return nullptr;
        case ' ':
            BoardCell *cell = get(m_cursor.selectorCell.pos);
            Selector &sc = m_cursor.selectorCell;
            Selector &dc = m_cursor.destinationCell;
            if(sc.cell == dc.cell && *sc.cell != BoardCell()) {
                if(cell->onPlacementHandler(board, m_cursor)) {
                    place(sc);
                    sc.cell = dc.cell = BoardCellFactory::boardCell.create();
                }
            } else if(*cell != BoardCell() ) {
                m_cursor.destinationCell.pos = m_cursor.selectorCell.pos;
                m_cursor.destinationCell.cell = m_cursor.selectorCell.cell = cell;
            }
            break;
        }
        return sender;
    }

    const stf::Vec2d Size { 8, 8 };
    std::vector<BoardCell*> board = std::vector<BoardCell*>(Size.x * Size.y);
    Cursor m_cursor;
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
                renderer.drawPixel({x*3+1, y+2}, model->get({x,y})->view());
            }
        }
        if(*model->m_cursor.destinationCell.cell != BoardCell()){
            renderer.drawPixel({model->m_cursor.destinationCell.pos.x * 3 + 0, model->m_cursor.destinationCell.pos.y + 2}, '{');
            renderer.drawPixel({model->m_cursor.destinationCell.pos.x * 3 + 2, model->m_cursor.destinationCell.pos.y + 2}, '}');
        }

        renderer.drawPixel({model->m_cursor.selectorCell.pos.x * 3 + 0, model->m_cursor.selectorCell.pos.y + 2}, '[');
        renderer.drawPixel({model->m_cursor.selectorCell.pos.x * 3 + 2, model->m_cursor.selectorCell.pos.y + 2}, ']');

    }
};

class Game : public stf::Window
{
    int n = 0;
    GameModel gameModel = GameModel();
    GameView gameView = GameView(&gameModel);
    stf::smv::IView *currentView = &gameView;

    bool onUpdate(const float) final
    {
        if(currentView == nullptr)
            return false;
        currentView->show(renderer);
//        return n++ == 1 ? false : true;
        return true;
    }

    void keyEvents(const int key) final
    {
        currentView = currentView->keyEventsHandler(key);
    }

    void mouseEvents(const stf::MouseRecord& mr) final
    {

    }
};

int main()
{
    Game().run();
//    _cellAlloc.info();
}
