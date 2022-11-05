#ifndef PLAYERS_HPP
#define PLAYERS_HPP

#include "boardcell.hpp"

class WhitePlayer : virtual public BoardCell
{
public:
    inline stf::ColorTable color() const override { return stf::ColorTable::White; }
    inline int uniqueNumericView() const override { return +2; }
};

class BlackPlayer : virtual public BoardCell
{
public:
    inline stf::ColorTable color() const override { return stf::ColorTable::Red; }
    inline int uniqueNumericView() const override { return -2; }
};


#endif // PLAYERS_HPP
