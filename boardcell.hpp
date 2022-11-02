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

class WhiteObject : public BoardCell
{
public:
    uint8_t view() const override { return 'W'; }
    stf::ColorTable color() const override { return stf::ColorTable::White; }
    bool onPlacementHandler(GameModel*, const Cursor&) override
    {
        return true;
    }
};

class BlackObject : public BoardCell
{
public:
    uint8_t view() const override { return 'B'; }
    stf::ColorTable color() const override { return stf::ColorTable::Black; }
    bool onPlacementHandler(GameModel*, const Cursor&) override
    {
        return true;
    }
};

class Queen : public BoardCell
{
public:
    bool onPlacementHandler(GameModel *model, const Cursor &cursor) override;
};

class WChecker : public WhiteObject
{
public:
    uint8_t view() const override { return 'w'; }
    bool onPlacementHandler(GameModel*, const Cursor&) override;
};

class BChecker : public BlackObject
{
public:
    uint8_t view() const override { return 'b'; }
    bool onPlacementHandler(GameModel*, const Cursor&) override;
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
