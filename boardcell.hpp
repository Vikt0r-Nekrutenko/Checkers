#ifndef BOARDCELL_HPP
#define BOARDCELL_HPP


#include "fieldsallocator.hpp"
#include "renderer.hpp"
#include "turns.hpp"

class GameModel;
class WhitePlayer;
class BlackPlayer;
struct Cursor;

class BoardCell
{
public:
    void* operator new(size_t size)
    {
        return _cellAllocator.allocate(size);
    }
    virtual uint8_t view() const { return 0; }
    virtual stf::ColorTable color() const { return stf::ColorTable::Default; }
    virtual GameTurn* takeNextTurn(GameModel *, const Cursor&) { return turns::nothingTurn(); }
    virtual GameTurn* isNextTurnAreAttack(GameModel *, const Cursor&, const stf::Vec2d&, const stf::Vec2d&) { return turns::nothingTurn(); }

    static stf::sdb::DynamicFieldsAllocator _cellAllocator;
};

class EmptyCell : public BoardCell
{
public:
    uint8_t view() const override
    {
        return 'e';
    }
};

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

class Checker : virtual public BoardCell
{
public:
    GameTurn* takeNextTurn(GameModel *model, const Cursor &cursor) override;
    GameTurn* isNextTurnAreAttack(GameModel *, const Cursor&, const stf::Vec2d&, const stf::Vec2d&) override;

    stf::Vec2d rMoveFw   = {0,0}, lMoveFw   = {0,0};
    stf::Vec2d rAttackFw = {0,0}, lAttackFw = {0,0};
};

class Queen : virtual public BoardCell
{
public:
    uint8_t view() const override { return 'Q'; }
    GameTurn* takeNextTurn(GameModel *model, const Cursor &cursor) override;
    GameTurn* isNextTurnAreAttack(GameModel *, const Cursor&, const stf::Vec2d&, const stf::Vec2d&) override;

    stf::Vec2d rMoveFw   = {+1,+1}, lMoveFw   = {-1,+1};
    stf::Vec2d rAttackFw = {+2,+2}, lAttackFw = {-2,+2};

    stf::Vec2d rMoveBw   = {+1,-1}, lMoveBw   = {-1,-1};
    stf::Vec2d rAttackBw = {+2,-2}, lAttackBw = {-2,-2};
};

class WChecker : public Checker, public WhitePlayer
{
public:
    WChecker()
    {
        rMoveFw   = {+1,+1}, lMoveFw   = {-1,+1};
        rAttackFw = {+2,+2}, lAttackFw = {-2,+2};
    }
    uint8_t view() const override { return 'w'; }
};

class BChecker : public Checker, public BlackPlayer
{
public:
    BChecker()
    {
        rMoveFw   = {+1,-1}, lMoveFw   = {-1,-1};
        rAttackFw = {+2,-2}, lAttackFw = {-2,-2};
    }
    uint8_t view() const override { return 'b'; }
};

class WQueen : public Queen, public WhitePlayer {};

class BQueen : public Queen, public BlackPlayer {};

template<typename T> class CellCreator
{
public:
    T* operator ()()
    {
        if(cell == nullptr) {
            return cell = new T();
        } else {
            return cell;
        }
    }

    T* cell = nullptr;
};

#endif // BOARDCELL_HPP
