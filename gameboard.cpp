#include "gameboard.hpp"
#include "cursor.hpp"

CellCreator<EmptyCell>   GameBoard::emptyCell   = CellCreator<EmptyCell>();
CellCreator<WhiteObject> GameBoard::whitePlayer = CellCreator<WhiteObject>();
CellCreator<BlackObject> GameBoard::blackPlayer = CellCreator<BlackObject>();

CellCreator<WChecker>  GameBoard::whiteChecker  = CellCreator<WChecker>();
CellCreator<BChecker>  GameBoard::blackChecker  = CellCreator<BChecker>();
CellCreator<WQueen>    GameBoard::whiteQueen    = CellCreator<WQueen>();
CellCreator<BQueen>    GameBoard::blackQueen    = CellCreator<BQueen>();

GameBoard::GameBoard()
    : board{ std::vector<BoardCell*>(Size.x * Size.y) }
{
    for(auto it = board.begin(); it != board.end(); ++it) {
        *it = emptyCell();
    }
}

BoardCell *GameBoard::operator[](const stf::Vec2d &p) {
    int i = Size.x * p.y + p.x;
    if(i < 0 || i > Size.x * Size.y - 1)
        throw std::out_of_range(std::string("index = ") + std::to_string(i));
    return board.at(i);
}

bool GameBoard::place(const stf::Vec2d &p, BoardCell *cell) {
    int i = Size.x * p.y + p.x;
    if(i < 0 || i > Size.x * Size.y - 1)
        throw std::out_of_range(std::string("index = ") + std::to_string(i));
    else board.at(Size.x * p.y + p.x) = cell;
    return true;
}

bool GameBoard::clear(const stf::Vec2d &p) {
    return place(p, emptyCell());
}

bool GameBoard::objectIsInBlackZone(const stf::Vec2d &pos) const {
    int indx = Size.x * pos.y + pos.x;
    return indx >= Size.x * Size.y - Size.x && indx < Size.x * Size.y;
}

bool GameBoard::objectIsInWhiteZone(const stf::Vec2d &pos) const {
    int indx = Size.x * pos.y + pos.x;
    return indx >= 0 && indx < Size.x;
}

BoardCell *GameBoard::getSelectedCell(const Cursor &cursor)
{
    return (*this)[cursor.selectedCell.pos];
}

BoardCell *GameBoard::getSelectableCell(const Cursor &cursor)
{
    return (*this)[cursor.selectableCell.pos];
}
