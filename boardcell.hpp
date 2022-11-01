#ifndef BOARDCELL_HPP
#define BOARDCELL_HPP


#include "fieldsallocator.hpp"
#include "vec2d.hpp"
#include <cstddef>

class GameModel;
struct Cursor;

class BoardCell
{
public:
    void* operator new(size_t size)
    {
        return _cellAllocator.allocate(size);
    }

    virtual uint8_t view() const = 0;
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
    bool onPlacementHandler(GameModel*, const Cursor&) override
    {
        return true;
    }
};

class WhiteObject : public BoardCell
{
public:
    stf::Vec2d   moveDirectionL { -1, +1 },   moveDirectionR { +1, +1 };
    stf::Vec2d attackDirectionL { -2, +2 }, attackDirectionR { +2, +2 };
};

class BlackObject : public BoardCell
{
public:
    stf::Vec2d   moveDirectionL { -1, -1 },   moveDirectionR { +1, -1 };
    stf::Vec2d attackDirectionL { -2, -2 }, attackDirectionR { +2, -2 };
};

class WChecker : public WhiteObject
{
public:
    bool onPlacementHandler(GameModel*, const Cursor&) override { return true; }
};

class WQueen : public WhiteObject
{
public:
    bool onPlacementHandler(GameModel*, const Cursor&) override { return true; }
};

class BChecker : public BlackObject
{
public:
    bool onPlacementHandler(GameModel*, const Cursor&) override { return true; }
};

class BQueen : public BlackObject
{
public:
    bool onPlacementHandler(GameModel*, const Cursor&) override { return true; }
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
