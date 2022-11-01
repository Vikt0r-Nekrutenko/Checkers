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

BoardCell *GameBoard::getSelectedCell(const Cursor &cursor)
{
    return (*this)[cursor.selectedCell.pos];
}

BoardCell *GameBoard::getSelectableCell(const Cursor &cursor)
{
    return (*this)[cursor.selectableCell.pos];
}
