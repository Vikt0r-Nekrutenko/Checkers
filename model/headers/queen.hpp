#ifndef QUEEN_HPP
#define QUEEN_HPP

#include "players.hpp"

class Queen : virtual public BoardCell
{
public:
    uint8_t view() const override { return 'Q'; }

    Queen()
    {
        directions.push_back({{+1,+1},{+2,+2}}); // right forward move dir & right forward attack dir
        directions.push_back({{-1,+1},{-2,+2}}); // left backward move dir && right backward attack dir
        directions.push_back({{+1,-1},{+2,-2}}); // right forward move dir & right forward attack dir
        directions.push_back({{-1,-1},{-2,-2}}); // left backward move dir && right backward attack dir
    }
};

class WQueen : public Queen, public WhitePlayer
{
    inline int uniqueNumericView() const override
    {
        return +3;
    }
};

class BQueen : public Queen, public BlackPlayer
{
    inline int uniqueNumericView() const override
    {
        return -3;
    }
};

#endif // QUEEN_HPP
