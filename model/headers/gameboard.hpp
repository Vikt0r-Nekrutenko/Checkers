#ifndef GAMEBOARD_HPP
#define GAMEBOARD_HPP

#include "vec2d.hpp"
#include <stdexcept>
#include <vector>
#include "checker.hpp"
#include "queen.hpp"
#include "cellcreator.hpp"

#define BOARD_W 8
#define BOARD_H 8

struct Cursor;

class GameBoard
{
public:
    GameBoard();

    BoardCell* operator[](const stf::Vec2d& p);
    BoardCell* operator[](const stf::Vec2d& p)const;

    bool place(const int i, BoardCell* cell);
    bool place(const stf::Vec2d& p, BoardCell* cell);
    bool clear(const stf::Vec2d& p);

    void wCheckerIsInBlackZoneTransform(const stf::Vec2d& pos);
    void bCheckerIsInWhiteZoneTransform(const stf::Vec2d& pos);
    bool isInBlackZone(const stf::Vec2d& pos) const;
    bool isInWhiteZone(const stf::Vec2d& pos) const;
    std::vector<stf::Vec2d> findPossibleAttacks(GameModel *model) const;
    stf::Vec2d calculatePieceCount() const;

    BoardCell* getSelectedCell(const Cursor& cursor);
    BoardCell* getSelectableCell(const Cursor& cursor);

    stf::Vec2d Size { BOARD_W, BOARD_H };
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
