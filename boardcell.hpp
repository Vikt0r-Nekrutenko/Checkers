#ifndef BOARDCELL_HPP
#define BOARDCELL_HPP


#include "fieldsallocator.hpp"
#include "renderer.hpp"
#include <cstddef>

class GameModel;
class WhiteObject;
class BlackObject;
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

class MovableObject : public BoardCell
{
public:
    bool onPlacementHandler(GameModel*, const Cursor&) override;

    virtual bool lMoveIsPossible(GameModel *model, const Cursor& cursor);
    virtual bool rMoveIsPossible(GameModel *model, const Cursor& cursor);

    virtual bool lAttackIsPossible(GameModel *model, const Cursor& cursor);
    virtual bool rAttackIsPossible(GameModel *model, const Cursor& cursor);

    virtual bool isFightAvailiable(GameModel *model, const Cursor& cursor) const;
    virtual bool rAttackAvailiable(GameModel *model, const Cursor& cursor) const;
    virtual bool lAttackAvailiable(GameModel *model, const Cursor& cursor) const;


    stf::Vec2d   moveFwL { 0, 0 },   moveFwR { 0, 0 };
    stf::Vec2d attackFwL { 0, 0 }, attackFwR { 0, 0 };
};

class WhiteObject : public MovableObject
{
public:
    uint8_t view() const override { return 'W'; }
    stf::ColorTable color() const override { return stf::ColorTable::White; }
    WhiteObject() : MovableObject()
    {
        moveFwL   = { -1, +1 };   moveFwR = { +1, +1 };
        attackFwL = { -2, +2 }; attackFwR = { +2, +2 };
    }
};

class BlackObject : public MovableObject
{
public:
    uint8_t view() const override { return 'B'; }
    stf::ColorTable color() const override { return stf::ColorTable::Black; }

    BlackObject() : MovableObject()
    {
        moveFwL   = { -1, -1 },   moveFwR = { +1, -1 };
        attackFwL = { -2, -2 }, attackFwR = { +2, -2 };
    }
};

class WChecker : public WhiteObject
{
public:
    uint8_t view() const override { return 'w'; }
};


class BChecker : public BlackObject
{
public:
    uint8_t view() const override { return 'b'; }
};

class Queen : public MovableObject
{
public:
    stf::Vec2d   moveBwL { 0, 0 },   moveBwR { 0, 0 };
    stf::Vec2d attackBwL { 0, 0 }, attackBwR { 0, 0 };

    bool onPlacementHandler(GameModel *model, const Cursor &cursor) override;
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
