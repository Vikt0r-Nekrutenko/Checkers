#ifndef GAMEBOARD_HPP
#define GAMEBOARD_HPP

#include "vec2d.hpp"
#include <stdexcept>
#include <vector>
#include "boardcell.hpp"

class BoardCell;
struct Cursor;

class GameBoard
{
public:
    GameBoard();
    BoardCell* operator[](const stf::Vec2d& p) {
        int i = Size.x * p.y + p.x;
        if(i < 0 || i > Size.x * Size.y - 1)
            throw std::out_of_range(std::string("index = ") + std::to_string(i));
        return board.at(i);
    }

    bool place(const stf::Vec2d& p, BoardCell* cell) {
        int i = Size.x * p.y + p.x;
        if(i < 0 || i > Size.x * Size.y - 1)
            throw std::out_of_range(std::string("index = ") + std::to_string(i));
        else board.at(Size.x * p.y + p.x) = cell;
        return true;
    }

    bool clear(const stf::Vec2d& p) {
        return place(p, emptyCell());
    }

    bool objectIsInBlackZone(const stf::Vec2d& pos) const {
        int indx = Size.x * pos.y + pos.x;
        return indx >= Size.x * Size.y - Size.x && indx < Size.x * Size.y;
    }

    bool objectIsInWhiteZone(const stf::Vec2d& pos) const {
        int indx = Size.x * pos.y + pos.x;
        return indx >= 0 && indx < Size.x;
    }

    BoardCell* getSelectedCell(const Cursor& cursor);
    BoardCell* getSelectableCell(const Cursor& cursor);

    const stf::Vec2d Size { 8, 8 };
    std::vector<BoardCell*> board;

    static CellCreator<EmptyCell>   emptyCell;
    static CellCreator<WhiteObject> whitePlayer;
    static CellCreator<BlackObject> blackPlayer;

    static CellCreator<WChecker>    whiteChecker;
    static CellCreator<BChecker>    blackChecker;
    static CellCreator<WQueen>      whiteQueen;
    static CellCreator<BQueen>      blackQueen;
};

#endif // GAMEBOARD_HPP
