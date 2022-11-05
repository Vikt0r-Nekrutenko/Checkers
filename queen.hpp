#ifndef QUEEN_HPP
#define QUEEN_HPP

#include "players.hpp"

class Queen : virtual public BoardCell
{
public:
    uint8_t view() const override { return 'Q'; }
    GameTurn* takeNextTurn(GameModel *model, const Cursor &cursor) override;
    GameTurn* isAttackTurnAvailiable(GameModel *model, const stf::Vec2d&pos) const override;
    GameTurn* isNextTurnAreAttack(GameModel *, const Cursor&, const stf::Vec2d&, const stf::Vec2d&) const override;

    stf::Vec2d rMoveFw   = {+1,+1}, lMoveFw   = {-1,+1};
    stf::Vec2d rAttackFw = {+2,+2}, lAttackFw = {-2,+2};

    stf::Vec2d rMoveBw   = {+1,-1}, lMoveBw   = {-1,-1};
    stf::Vec2d rAttackBw = {+2,-2}, lAttackBw = {-2,-2};
};

class WQueen : public Queen, public WhitePlayer {};

class BQueen : public Queen, public BlackPlayer {};

#endif // QUEEN_HPP
