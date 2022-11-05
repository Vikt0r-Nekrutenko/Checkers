#ifndef CHECKER_HPP
#define CHECKER_HPP

#include "players.hpp"

class Checker : virtual public BoardCell
{
public:
    uint8_t view() const override { return 'o'; }
};

class WChecker : public Checker, public WhitePlayer
{
public:
    WChecker()
    {
        directions.push_back({{+1,+1},{+2,+2}}); // right move dir & right attack dir
        directions.push_back({{-1,+1},{-2,+2}}); // left move dir && right attack dir
    }
    bool isTransformPossible(GameModel *model) override;
    BoardCell* getTransformPiece() override;
};

class BChecker : public Checker, public BlackPlayer
{
public:
    BChecker()
    {
        directions.push_back({{+1,-1},{+2,-2}}); // right move dir & right attack dir
        directions.push_back({{-1,-1},{-2,-2}}); // left move dir && right attack dir
    }
    virtual bool isTransformPossible(GameModel *model) override;
    BoardCell* getTransformPiece() override;
};

#endif // CHECKER_HPP
