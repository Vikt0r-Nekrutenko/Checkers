#ifndef CHECKER_HPP
#define CHECKER_HPP

#include "players.hpp"

class Checker : virtual public BoardCell
{
public:
    uint8_t view() const override { return 'o'; }
    GameTurn* takeNextTurn(GameModel *model, const Cursor &cursor) override;
    GameTurn* isAttackTurnAvailiable(GameModel *model, const stf::Vec2d&pos) const override;
    GameTurn* isNextTurnAreAttack(GameModel *, const Cursor&, const stf::Vec2d&, const stf::Vec2d&) const override;

    stf::Vec2d rMoveFw   = {0,0}, lMoveFw   = {0,0};
    stf::Vec2d rAttackFw = {0,0}, lAttackFw = {0,0};
};

class WChecker : public Checker, public WhitePlayer
{
public:
    WChecker()
    {
        rMoveFw   = {+1,+1}, lMoveFw   = {-1,+1};
        rAttackFw = {+2,+2}, lAttackFw = {-2,+2};
    }
    bool isTransformPossible(GameModel *model) override;
    BoardCell* getTransformPiece() override;
};

class BChecker : public Checker, public BlackPlayer
{
public:
    BChecker()
    {
        rMoveFw   = {+1,-1}, lMoveFw   = {-1,-1};
        rAttackFw = {+2,-2}, lAttackFw = {-2,-2};
    }
    virtual bool isTransformPossible(GameModel *model) override;
    BoardCell* getTransformPiece() override;
};

#endif // CHECKER_HPP
