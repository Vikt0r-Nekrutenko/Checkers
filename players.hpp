#ifndef PLAYERS_HPP
#define PLAYERS_HPP

#include "boardcell.hpp"

class WhitePlayer : virtual public BoardCell
{
public:
    stf::ColorTable color() const override { return stf::ColorTable::White; }
};

class BlackPlayer : virtual public BoardCell
{
public:
    stf::ColorTable color() const override { return stf::ColorTable::Red; }
};


#endif // PLAYERS_HPP
