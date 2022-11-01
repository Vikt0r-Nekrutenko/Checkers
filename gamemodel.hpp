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
        board.place({2,0}, GameBoard::whiteChecker());
        board.place({3,1}, GameBoard::blackChecker());
        board.place({5,3}, GameBoard::blackChecker());
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
            if(board.objectIsInBlackZone(cursor.selectableCell.pos))
                board.place(cursor.selectableCell.pos, GameBoard::whiteQueen());
            else if(board.objectIsInWhiteZone(cursor.selectableCell.pos))
                board.place(cursor.selectableCell.pos, GameBoard::blackQueen());
            else
                board.place(cursor.selectableCell.pos, cursor.selectableCell.cell);
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
    uint32_t exCount = 0ul;
};

#endif // GAMEMODEL_HPP
