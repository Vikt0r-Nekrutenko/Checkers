#ifndef GAMEMODEL_HPP
#define GAMEMODEL_HPP

#include "imodel.hpp"
#include "gameboard.hpp"
#include "cursor.hpp"

class GameModel : public stf::smv::BaseModel
{
public:
    GameModel() : stf::smv::BaseModel()
    {
        board.place({2,0}, GameBoard::blackChecker());
        board.place({3,1}, GameBoard::whiteChecker());
        board.place({5,3}, GameBoard::whiteChecker());
    }

    BoardCell *opponent() const {
        if (player == GameBoard::blackPlayer())
            return GameBoard::whitePlayer();
        else
            return GameBoard::blackPlayer();
    }

    stf::smv::IView* put(stf::smv::IView *sender)
    {
        BoardCell *cell = board.getSelectableCell(cursor);
        Selector &sc = cursor.selectableCell;
        Selector &dc = cursor.selectedCell;

        if(!cursor.cursorIsEmpty() && sc.cell->onPlacementHandler(this, cursor)) {
//            place(sc);
//            sc.cell = dc.cell = BoardCellFactory::emptyCell.create();
            cursor.reset();
            player = opponent();
        } else if(cell->color() == player->color()) {
            cursor.select(cell);
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
    BoardCell *player = GameBoard::blackPlayer();
};

#endif // GAMEMODEL_HPP
