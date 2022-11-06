#ifndef PLAYERS_HPP
#define PLAYERS_HPP

#include "boardcell.hpp"

class Player : public ColoredCell, public UniqueNumericCell
{
public:
    virtual inline std::string name() const { return "Player"; }
};

class WhitePlayer : virtual public Player, virtual public BoardCell
{
public:
    inline stf::ColorTable color() const override { return stf::ColorTable::White; }
    inline int uniqueNumericView() const override { return +2; }
    inline std::string name() const override { return "White"; }
};

class BlackPlayer : virtual public Player, virtual public BoardCell
{
public:
    inline stf::ColorTable color() const override { return stf::ColorTable::Red; }
    inline int uniqueNumericView() const override { return -2; }
    inline std::string name() const override { return "Black"; }
};


#endif // PLAYERS_HPP
