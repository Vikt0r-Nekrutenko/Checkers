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
    virtual uint8_t view() const = 0;
    virtual stf::ColorTable color() const = 0;
    virtual bool onPlacementHandler(GameModel *model, const Cursor& cursor) = 0;

    static stf::sdb::DynamicFieldsAllocator _cellAllocator;
};

class EmptyCell : public BoardCell
{
public:
    uint8_t view() const override
    {
        return 'e';
    }

    stf::ColorTable color() const override
    {
        return stf::ColorTable::Default;
    }

    bool onPlacementHandler(GameModel*, const Cursor&) override
    {
        return true;
    }
};

class Player : public BoardCell
{
public:
    uint8_t view() const override { return 'p'; }
    bool onPlacementHandler(GameModel *, const Cursor &) override { return true; };
};

class WhitePlayer : public Player
{
public:
    stf::ColorTable color() const override { return stf::ColorTable::White; }
};

class BlackPlayer : public Player
{
public:
    stf::ColorTable color() const override { return stf::ColorTable::Black; }
};

class Checker : public BoardCell
{
public:
    bool onPlacementHandler(GameModel *model, const Cursor &cursor) override;

    stf::Vec2d rMoveFw   = {0,0}, lMoveFw   = {0,0};
    stf::Vec2d rAttackFw = {0,0}, lAttackFw = {0,0};
};

class Queen : public BoardCell
{
public:
    bool onPlacementHandler(GameModel *model, const Cursor &cursor) override;

    stf::Vec2d rMoveFw   = {+1,+1}, lMoveFw   = {-1,+1};
    stf::Vec2d rAttackFw = {+2,+2}, lAttackFw = {-2,+2};

    stf::Vec2d rMoveBw   = {+1,-1}, lMoveBw   = {-1,-1};
    stf::Vec2d rAttackBw = {+2,-2}, lAttackBw = {-2,-2};
};

class WChecker : public Checker
{
public:
    WChecker()
    {
        rMoveFw   = {+1,+1}, lMoveFw   = {-1,+1};
        rAttackFw = {+2,+2}, lAttackFw = {-2,+2};
    }
    uint8_t view() const override { return 'w'; }
    stf::ColorTable color() const override { return stf::ColorTable::White; }
};

class BChecker : public Checker
{
public:
    BChecker()
    {
        rMoveFw   = {+1,-1}, lMoveFw   = {-1,-1};
        rAttackFw = {+2,-2}, lAttackFw = {-2,-2};
    }
    uint8_t view() const override { return 'b'; }
    stf::ColorTable color() const override { return stf::ColorTable::Black; }
};

class WQueen : public Queen
{
public:
    uint8_t view() const override { return 'W'; }
    stf::ColorTable color() const override { return stf::ColorTable::White; }
};

class BQueen : public Queen
{
public:
    uint8_t view() const override { return 'B'; }
    stf::ColorTable color() const override { return stf::ColorTable::Black; }
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
