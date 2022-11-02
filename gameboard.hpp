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

    BoardCell* operator[](const stf::Vec2d& p);

    bool place(const int i, BoardCell* cell);
    bool place(const stf::Vec2d& p, BoardCell* cell);
    bool clear(const stf::Vec2d& p);

    bool objectIsInBlackZone(const stf::Vec2d& pos) const;
    bool objectIsInWhiteZone(const stf::Vec2d& pos) const;

    BoardCell* getSelectedCell(const Cursor& cursor);
    BoardCell* getSelectableCell(const Cursor& cursor);

    const stf::Vec2d Size { 8, 8 };
    std::vector<BoardCell*> board;

    static CellCreator<EmptyCell>   emptyCell;
    static CellCreator<WhitePlayer> whitePlayer;
    static CellCreator<BlackPlayer> blackPlayer;

    static CellCreator<WChecker>    whiteChecker;
    static CellCreator<BChecker>    blackChecker;
    static CellCreator<WQueen>      whiteQueen;
    static CellCreator<BQueen>      blackQueen;
};

#endif // GAMEBOARD_HPP
