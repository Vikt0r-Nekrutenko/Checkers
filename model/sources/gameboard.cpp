#include "gameboard.hpp"
#include "cursor.hpp"
#include "boardcell.hpp"
#include "gamemodel.hpp"

CellCreator<EmptyCell>   GameBoard::emptyCell   = CellCreator<EmptyCell>();
CellCreator<WhitePlayer> GameBoard::whitePlayer = CellCreator<WhitePlayer>();
CellCreator<BlackPlayer> GameBoard::blackPlayer = CellCreator<BlackPlayer>();

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

    int n = 0;
    BoardCell *cell = GameBoard::whiteChecker();

    for(int y = 0; y < 8; ++y) {
        for(int x = 1; x < 8; x += 2) {
            if(y == 3) {
                y = 5;
                cell = GameBoard::blackChecker();
            }
            int indx = 8 * y + (x - n);
            place(indx, cell);
        }
        n ^= 1;
    }
}

BoardCell *GameBoard::operator[](const stf::Vec2d &p) {
    int i = Size.x * p.y + p.x;
    if(p.x < 0 || p.y < 0 || p.x >= Size.x || p.y >= Size.y)
        throw std::out_of_range(std::string("index = ") + std::to_string(i));
    return board.at(i);
}

BoardCell *GameBoard::operator[](const stf::Vec2d &p) const {
    int i = Size.x * p.y + p.x;
    if(p.x < 0 || p.y < 0 || p.x >= Size.x || p.y >= Size.y)
        throw std::out_of_range(std::string("index = ") + std::to_string(i));
    return board.at(i);
}

bool GameBoard::place(const int i, BoardCell *cell)
{
    if(i < 0 || i > Size.x * Size.y - 1)
        throw std::out_of_range(std::string("index = ") + std::to_string(i));
    else board.at(i) = cell;
    return true;
}

bool GameBoard::place(const stf::Vec2d &p, BoardCell *cell) {
    int i = Size.x * p.y + p.x;
    if(p.x < 0 || p.y < 0 || p.x >= Size.x || p.y >= Size.y)
        throw std::out_of_range(std::string("index = ") + std::to_string(i));
    else board.at(i) = cell;
    return true;
}

bool GameBoard::clear(const stf::Vec2d &p) {
    return place(p, emptyCell());
}

void GameBoard::wCheckerIsInBlackZoneTransform(const stf::Vec2d &pos) {
    int indx = Size.x * pos.y + pos.x;
    if(indx >= Size.x * Size.y - Size.x && indx < Size.x * Size.y && board.at(indx) == whiteChecker()) {
        board.at(indx) = whiteQueen();
    }
}

void GameBoard::bCheckerIsInWhiteZoneTransform(const stf::Vec2d &pos) {
    int indx = Size.x * pos.y + pos.x;
    if(indx >= 0 && indx < Size.x && board.at(indx) == blackChecker()) {
        board.at(indx) = blackQueen();
    }
}

bool GameBoard::isInBlackZone(const stf::Vec2d &pos) const
{
    int indx = Size.x * pos.y + pos.x;
    return  (indx >= Size.x * Size.y - Size.x && indx < Size.x * Size.y);
}

bool GameBoard::isInWhiteZone(const stf::Vec2d &pos) const
{
    int indx = Size.x * pos.y + pos.x;
    return (indx >= 0 && indx < Size.x);
}

std::vector<stf::Vec2d> GameBoard::findPossibleAttacks(GameModel *model) const
{
    std::vector<stf::Vec2d> possibleAttacks;
    for(int y = 0; y < Size.y; ++y) {
        for(int x = 0; x < Size.x; ++x) {
            try {
                if((*this)[{x,y}]->color() != model->player->color())
                    continue;
                if((*this)[{x,y}]->isAttackTurnAvailiable(model, {x,y}) == turns::nothingTurn())
                    continue;
                possibleAttacks.push_back({x,y});
            } catch(...) {
                continue;
            }
        }
    }
    return possibleAttacks;
}

stf::Vec2d GameBoard::calculatePieceCount() const
{
    stf::Vec2d pieceCount { 0, 0 };

    for(auto i : board)
        if(i->color() == blackPlayer()->color())
            pieceCount.x++;
        else if(i->color() == whitePlayer()->color())
            pieceCount.y++;
    return pieceCount;
}

BoardCell *GameBoard::getSelectedCell(const Cursor &cursor)
{
    return (*this)[cursor.selectedCell.pos];
}

BoardCell *GameBoard::getSelectableCell(const Cursor &cursor)
{
    return (*this)[cursor.selectableCell.pos];
}
