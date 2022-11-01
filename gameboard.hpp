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

    BoardCell* getSelectedCell(const Cursor& cursor);

    const stf::Vec2d Size { 8, 8 };
    std::vector<BoardCell*> board;

    static CellCreator<EmptyCell> emptyCell;
};

#endif // GAMEBOARD_HPP
