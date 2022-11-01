#ifndef GAMEMODEL_HPP
#define GAMEMODEL_HPP

#include "imodel.hpp"
#include "gameboard.hpp"
#include "cursor.hpp"

class GameModel : public stf::smv::BaseModel
{
public:
    GameModel() = default;

    stf::smv::IView* put(stf::smv::IView *sender)
    {
        BoardCell *cell = board.getSelectedCell(cursor);
        Selector &sc = cursor.selectableCell;
        Selector &dc = cursor.selectedCell;

        if(cursor.cursorIsEmpty() && sc.cell->onPlacementHandler(this, cursor)) {
//            place(sc);
//            sc.cell = dc.cell = BoardCellFactory::emptyCell.create();
//            switchPlayer();
//        } else if(get(sc.pos) == player) {
//            m_cursor.selectedCell.pos = m_cursor.selectableCell.pos;
//            m_cursor.selectedCell.cell = m_cursor.selectableCell.cell = cell;
        }

        return sender;
    }

    stf::smv::IView *keyEventsHandler(stf::smv::IView *sender, const int key) final
    {
        switch (key)
        {
        case 'w': if(cursor.selectableCell.pos.y > 0) cursor.selectableCell.pos -= stf::Vec2d(0,1); break;
        case 'a': if(cursor.selectableCell.pos.x > 0) cursor.selectableCell.pos -= stf::Vec2d(1,0); break;
        case 's': if(cursor.selectableCell.pos.y < board.Size.y-1) cursor.selectableCell.pos += stf::Vec2d(0,1); break;
        case 'd': if(cursor.selectableCell.pos.x < board.Size.x-1) cursor.selectableCell.pos += stf::Vec2d(1,0); break;
        case 'q': return nullptr;
        case ' ': return put(sender);
        }
        return sender;
    }

    GameBoard board = GameBoard();
    Cursor cursor = Cursor();
};

#endif // GAMEMODEL_HPP
