#ifndef CHECKER_HPP
#define CHECKER_HPP

#include "players.hpp"

class Checker : virtual public BoardCell
{
public:
    inline uint8_t view() const override { return 'o'; }
};

class WChecker : public Checker, public WhitePlayer
{
public:
    WChecker();
    bool isTransformPossible(GameModel *model) override;
    BoardCell* getTransformPiece() override;
};

class BChecker : public Checker, public BlackPlayer
{
public:
    BChecker();
    virtual bool isTransformPossible(GameModel *model) override;
    BoardCell* getTransformPiece() override;
};

#endif // CHECKER_HPP
