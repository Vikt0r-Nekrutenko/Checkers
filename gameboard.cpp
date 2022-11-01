#include "gameboard.hpp"

CellCreator<EmptyCell> GameBoard::emptyCell = CellCreator<EmptyCell>();

GameBoard::GameBoard()
    : board{ std::vector<BoardCell*>(Size.x * Size.y) }
{
    for(auto it = board.begin(); it != board.end(); ++it) {
        *it = emptyCell();
    }
}
