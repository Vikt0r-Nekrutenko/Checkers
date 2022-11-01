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
    { return sender; }

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
