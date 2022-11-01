#include <window.hpp>
#include <iview.hpp>
#include <imodel.hpp>
#include <iostream>

stf::sdb::DynamicFieldsAllocator _cellAlloc;

struct Cursor;
struct Selector;
class GameModel;

class BoardCell
{
public:
    inline virtual uint8_t view() const = 0;
    virtual bool onPlacementHandler(GameModel *model, Cursor& cursor);

    void* operator new(size_t size) { return _cellAlloc.allocate(size); }
    bool operator !=(const BoardCell& cell) const { return view() != cell.view(); }
    bool operator ==(const BoardCell& cell) const { return view() != cell.view(); }
};

class EmptyCheckerCell;
class WhiteCheckerCell;
class BlackCheckerCell;

class CheckerCell : public BoardCell {};

class EmptyCheckerCell : public CheckerCell
{
public:
    inline uint8_t view() const final { return 'e'; }
};


class MovableCheckerCell : public CheckerCell
{
public:
    bool onPlacementHandler(GameModel *model, Cursor& cursor) final;

    bool nextLMoveIsPossible(GameModel *model, const Selector& selected, const Selector& selectable);
    bool nextRMoveIsPossible(GameModel *model, const Selector& selected, const Selector& selectable);
    bool nextLFightIsPossible(GameModel *model, const Selector& selected, const Selector& selectable);
    bool nextRFightIsPossible(GameModel *model, const Selector& selected, const Selector& selectable);
    bool isFightAvailiable(GameModel *model, const Selector& selected) const;

    stf::Vec2d dirL { 0, 0 };
    stf::Vec2d dirR { 0, 0 };

    stf::Vec2d fightDirL { 0, 0 };
    stf::Vec2d fightDirR { 0, 0 };
};

class WhiteCheckerCell : public MovableCheckerCell
{
public:
    WhiteCheckerCell() { dirL      = { -1, +1 };      dirR = { +1, +1 };
                         fightDirL = { -2, +2 }; fightDirR = { +2, +2 }; }
    inline uint8_t view() const final { return 'o'; }
};

class BlackCheckerCell : public MovableCheckerCell
{
public:
    BlackCheckerCell() { dirL      = { -1, -1 };      dirR = { +1, -1 };
                         fightDirL = { -2, -2 }; fightDirR = { +2, -2 }; }
    inline uint8_t view() const final { return '*'; }
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
    static BoardCellCreator<EmptyCheckerCell> emptyCell;
    static BoardCellCreator<WhiteCheckerCell> whiteCell;
    static BoardCellCreator<BlackCheckerCell> blackCell;
};

BoardCellCreator<EmptyCheckerCell> BoardCellFactory::emptyCell = BoardCellCreator<EmptyCheckerCell>();
BoardCellCreator<WhiteCheckerCell> BoardCellFactory::whiteCell = BoardCellCreator<WhiteCheckerCell>();
BoardCellCreator<BlackCheckerCell> BoardCellFactory::blackCell = BoardCellCreator<BlackCheckerCell>();

struct Selector
{
    stf::Vec2d pos { 0, 0 };
    BoardCell *cell = BoardCellFactory::emptyCell.create();
};

struct Cursor
{
    Selector selectableCell;
    Selector selectedCell;
};

bool BoardCell::onPlacementHandler(GameModel *, Cursor&) { return true; }

class GameModel : public stf::smv::BaseModel
{
public:

    GameModel() : stf::smv::BaseModel() {
        for(auto it = board.begin(); it != board.end(); ++it) {
            *it = BoardCellFactory::emptyCell.create();
        }
        board.at(2) = BoardCellFactory::whiteCell.create();
        board.at(11) = BoardCellFactory::blackCell.create();
        board.at(29) = BoardCellFactory::blackCell.create();
    }

    BoardCell* get(const stf::Vec2d& pos) {
        int indx = Size.x * pos.y + pos.x;
        if(indx < 0 || indx > 63)
            return nullptr;
        return board.at(indx);
    }
    void place(const Selector& s) {
        int indx = Size.x * s.pos.y + s.pos.x;
        if(indx < 0 || indx > 63)
            return;
        board.at(indx) = s.cell;
    }
    void place(const stf::Vec2d& p, CheckerCell *cell) {
        int indx = Size.x * p.y + p.x;
        if(indx < 0 || indx > 63)
            return;
        board.at(indx) = cell;
    }
    void switchPlayer()
    {
        if(player == BoardCellFactory::whiteCell.create())
            player = BoardCellFactory::blackCell.create();
        else
            player = BoardCellFactory::whiteCell.create();
    }
    MovableCheckerCell *opponent() const {
        if(player == BoardCellFactory::whiteCell.create())
            return BoardCellFactory::blackCell.create();
        else
            return BoardCellFactory::whiteCell.create();
    }

    stf::smv::IView* put(stf::smv::IView *sender);

    stf::smv::IView *keyEventsHandler(stf::smv::IView *sender, const int key)
    {
        switch (key)
        {
        case 'w': if(m_cursor.selectableCell.pos.y > 0) m_cursor.selectableCell.pos -= stf::Vec2d(0,1); break;
        case 'a': if(m_cursor.selectableCell.pos.x > 0) m_cursor.selectableCell.pos -= stf::Vec2d(1,0); break;
        case 's': if(m_cursor.selectableCell.pos.y < Size.y-1) m_cursor.selectableCell.pos += stf::Vec2d(0,1); break;
        case 'd': if(m_cursor.selectableCell.pos.x < Size.x-1) m_cursor.selectableCell.pos += stf::Vec2d(1,0); break;
        case 'q': return nullptr;
        case ' ': return put(sender);
        }
        return sender;
    }

    const stf::Vec2d Size { 8, 8 };
    std::vector<BoardCell*> board = std::vector<BoardCell*>(Size.x * Size.y);
    Cursor m_cursor;
    MovableCheckerCell *player = BoardCellFactory::blackCell.create();
};

bool MovableCheckerCell::onPlacementHandler(GameModel *model, Cursor& cursor)
{
    Selector &sc = cursor.selectableCell;
    Selector &dc = cursor.selectedCell;

    if(isFightAvailiable(model, dc)) {
        if(nextRFightIsPossible(model, dc, sc)) {
            model->place(dc.pos, BoardCellFactory::emptyCell.create());
            model->place(dc.pos + dirR, BoardCellFactory::emptyCell.create());
            return true;
        } else if(nextLFightIsPossible(model, dc, sc)) {
            model->place(dc.pos, BoardCellFactory::emptyCell.create());
            model->place(dc.pos + dirL, BoardCellFactory::emptyCell.create());
            return true;
        }
    } else if(nextRMoveIsPossible(model, dc, sc) || nextLMoveIsPossible(model, dc, sc)) {
        model->place(dc.pos, BoardCellFactory::emptyCell.create());
        return true;
    }

    return false;
}

bool MovableCheckerCell::isFightAvailiable(GameModel *model, const Selector& selected) const
{
    if((model->get(selected.pos + dirL) == model->opponent() && model->get(selected.pos + fightDirL) == BoardCellFactory::emptyCell.create()) ||
       (model->get(selected.pos + dirR) == model->opponent() && model->get(selected.pos + fightDirR) == BoardCellFactory::emptyCell.create()))
        return true;
    return false;
}

bool MovableCheckerCell::nextLFightIsPossible(GameModel *model, const Selector& selected, const Selector& selectable)
{
    if(model->get(selected.pos + fightDirL) == BoardCellFactory::emptyCell.create() &&
       selected.pos + fightDirL == selectable.pos &&
       model->get(selected.pos + dirL) == model->opponent())
        return true;
    return false;
}

bool MovableCheckerCell::nextRFightIsPossible(GameModel *model, const Selector& selected, const Selector& selectable)
{
    if(model->get(selected.pos + fightDirR) == BoardCellFactory::emptyCell.create() &&
       selected.pos + fightDirR == selectable.pos &&
       model->get(selected.pos + dirR) == model->opponent())
        return true;
    return false;

}

bool MovableCheckerCell::nextLMoveIsPossible(GameModel *model, const Selector& selected, const Selector& selectable)
{
    if(model->get(selected.pos + dirL) == BoardCellFactory::emptyCell.create() && selected.pos + dirL == selectable.pos)
        return true;
    return false;
}

bool MovableCheckerCell::nextRMoveIsPossible(GameModel *model, const Selector& selected, const Selector& selectable)
{
    if(model->get(selected.pos + dirR) == BoardCellFactory::emptyCell.create() && selected.pos + dirR == selectable.pos)
        return true;
    return false;
}

stf::smv::IView* GameModel::put(stf::smv::IView *sender)
{
    BoardCell *cell = get(m_cursor.selectableCell.pos);
    Selector &sc = m_cursor.selectableCell;
    Selector &dc = m_cursor.selectedCell;
    if(sc.cell == dc.cell && sc.cell != BoardCellFactory::emptyCell.create() && sc.cell->onPlacementHandler(this, m_cursor)) {
        place(sc);
        sc.cell = dc.cell = BoardCellFactory::emptyCell.create();
        switchPlayer();
    } else if(get(sc.pos) == player) {
        m_cursor.selectedCell.pos = m_cursor.selectableCell.pos;
        m_cursor.selectedCell.cell = m_cursor.selectableCell.cell = cell;
    }
    return sender;
}



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
        if(*model->m_cursor.selectedCell.cell != EmptyCheckerCell()){
            renderer.drawPixel({model->m_cursor.selectedCell.pos.x * 3 + 0, model->m_cursor.selectedCell.pos.y + 2}, '{');
            renderer.drawPixel({model->m_cursor.selectedCell.pos.x * 3 + 2, model->m_cursor.selectedCell.pos.y + 2}, '}');
        }

        renderer.drawPixel({model->m_cursor.selectableCell.pos.x * 3 + 0, model->m_cursor.selectableCell.pos.y + 2}, '[');
        renderer.drawPixel({model->m_cursor.selectableCell.pos.x * 3 + 2, model->m_cursor.selectableCell.pos.y + 2}, ']');

        renderer.drawPixel({0,10}, model->player->view());
    }
};

class Game : public stf::Window
{
//    int n = 0;
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

    void mouseEvents(const stf::MouseRecord&) final
    {

    }
};

int main()
{
    Game().run();
//    _cellAlloc.info();
}
