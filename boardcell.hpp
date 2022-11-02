#ifndef BOARDCELL_HPP
#define BOARDCELL_HPP


#include "fieldsallocator.hpp"
#include "renderer.hpp"
#include <cstddef>

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
    virtual bool onPlacementHandler(GameModel *, const Cursor&) { return true; }

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
    stf::ColorTable color() const override { return stf::ColorTable::Black; }
};

class Checker : virtual public BoardCell
{
public:
    bool onPlacementHandler(GameModel *model, const Cursor &cursor) override;

    stf::Vec2d rMoveFw   = {0,0}, lMoveFw   = {0,0};
    stf::Vec2d rAttackFw = {0,0}, lAttackFw = {0,0};
};

class Queen : virtual public BoardCell
{
public:
    bool onPlacementHandler(GameModel *model, const Cursor &cursor) override;

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
    using WhitePlayer::color;
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
    using BlackPlayer::color;
};

class WQueen : public Queen, public WhitePlayer
{
public:
    uint8_t view() const override { return 'W'; }
    using WhitePlayer::color;
};

class BQueen : public Queen, public BlackPlayer
{
public:
    uint8_t view() const override { return 'B'; }
    using BlackPlayer::color;
};

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
